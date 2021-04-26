import serial
from datetime import datetime
import json

ser = serial.Serial('COM9', baudrate = 115200, timeout = 7)

while 1:
        arduinoData = ser.readline().decode("ascii")
        pdbData = json.loads(arduinoData)
        print('From Arduino: ', arduinoData)
        if 'request' in pdbData:
                if pdbData['request'] == 'time':
                        now = datetime.now()
                        seconds_since_midnight = (now - now.replace(hour=0, minute=0, second=0, microsecond=0)).total_seconds()
                        json_time = json.dumps({'time':int(seconds_since_midnight),'alive':True})
                        print('To Arduino: ', json_time.encode("ascii"))
                        ser.write(json_time.encode("ascii"))
                        
        else:
                pdbResponse = json.dumps({"alive":True})
                print('To Arduino: ', pdbResponse)
                ser.write(pdbResponse.encode('ascii'))
                
