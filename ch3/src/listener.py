#!/usr/bin/env python
# license removed for brevity
import rospy
from std_msgs.msg import String

def callback(msg):
    print("I heard {}".format(msg.data))


rospy.init_node("listener")
rospy.Subscriber("chatter", String, callback)

rospy.spin()

