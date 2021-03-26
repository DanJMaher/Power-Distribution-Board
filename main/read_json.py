import serial
import time
import json

ser = serial.Serial('COM6', baudrate = 9600, timeout = 2)

while 1:
        arduinoData = ser.readline().decode("ascii")
        pdbData = json.loads(arduinoData)

        print(pdbData)
        #print("11.1V rail: ", pdbData["voltage"][0], " volts")
        #print("22.2V rail: ", pdbData["voltage"][1], " volts")
        #print("11.1V relay: ", pdbData["relay"][0])
        #print("22.2V relay: ", pdbData["relay"][1])
        print()
        
        pdbResponse = json.dumps({"alive":True})
        ser.write(pdbResponse.encode('ascii'))
