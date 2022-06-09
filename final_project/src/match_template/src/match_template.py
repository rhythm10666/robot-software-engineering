#!/usr/bin/env python
import sys
import rospy
import cv2
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

def call_back(data):
    bridge = CvBridge()
    cv_image = bridge.imgmsg_to_cv2(data,'bgr8')

    cv2.imshow('src', cv_image)
    cv2.waitKey(1)

    # template matching
    template = cv2.imread('/home/longway/turtlebot_ws/src/match_template/src/template.jpg')
    h, w = template.shape[:2]
    template = cv2.resize(template, (int(w/10), int(h/10)))
    cv2.imshow('template', template)
    cv2.waitKey(2)

    h, w = template.shape[:2]

    res = cv2.matchTemplate(cv_image, template, cv2.TM_SQDIFF)

    min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)

    top_left = min_loc
    bottom_right =(top_left[0]+ w, top_left[1]+ h)
    cv2.rectangle(cv_image, top_left, bottom_right,(0, 0, 255), 2)

    cv2.imshow('dst', cv_image)
    cv2.waitKey(1)


if __name__=='__main__':

	rospy.init_node('take_photo', anonymous = False)

	img_topic ='/camera/rgb/image_raw'
	image_sub = rospy.Subscriber(img_topic, Image, call_back)

	rospy.sleep(1)

	rospy.spin()
