#!/usr/bin/env python

import rospy
import serial
import json
from datetime import datetime
from std_msgs.msg import String

ser = serial.Serial('/dev/ttyACM0', baudrate = 115200, timeout = 7)

def power_monitor():
    pub = rospy.Publisher('power', String, queue_size=10)
    rospy.init_node('power_monitor', anonymous=True)
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        powerData = getTransmission()
        
        if 'request' in powerData:
            updateTime(powerData)
            
        sendResponse(powerData)
        rospy.loginfo(powerData)
        pub.publish(powerData)
        rate.sleep()

def getTransmission():
    bufferData = ser.readline().decode("ascii")
    powerJson = json.loads(bufferData)
    return powerJson

def updateTime(jsonDoc):
    if jsonDoc['request'] == 'time':
            now = datetime.now()
            seconds_since_midnight = (now - now.replace(hour=0, minute=0, second=0, microsecond=0)).total_seconds()
            json_time = json.dumps({'time':int(seconds_since_midnight),'alive':True})
            ser.write(json_time.encode("ascii"))

def sendResponse(jsonDoc):
        pdbResponse = json.dumps({'alive':True})
        ser.write(pdbResponse.encode("ascii"))
        
if __name__ == '__main__':
    try:
        power_monitor()
    except rospy.ROSInterruptException:
        pass
