import serial
from datetime import datetime
import json

ser = serial.Serial('COM6', baudrate = 9600, timeout = 7)

while 1:
        arduinoData = ser.readline().decode("ascii")
        print(arduinoData)
        pdbData = json.loads(arduinoData)

        if 'request' in pdbData:
                if pdbData['request'] == 'time':
                        now = datetime.now()
                        seconds_since_midnight = (now - now.replace(hour=0, minute=0, second=0, microsecond=0)).total_seconds()
                        json_time = json.dumps({'time':int(seconds_since_midnight),'alive':True})
                        ser.write(json_time.encode("ascii"))
                        
        else:
                pdbResponse = json.dumps({"alive":True})
                ser.write(pdbResponse.encode('ascii'))
