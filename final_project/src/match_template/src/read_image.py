#!/usr/bin/env python
import sys
import rospy
import cv2
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import time

def call_back(data):
	bridge = CvBridge()
	cv_image = bridge.imgmsg_to_cv2(data, 'rgb8')

	cv2.imshow('src', cv_image)
	cv2.waitKey(1)

if __name__=='__main__':

	rospy.init_node('take_photo', anonymous = True)

	img_topic ='/camera/rgb/image_raw'
	image_sub = rospy.Subscriber(img_topic, Image, call_back)

	rospy.sleep(1)

	rospy.spin()

