Updated 3/29/21 - Dan Maher
(more info to come)

Basic use:
1) Plug in an Arduino, send the code. 
2) run serial.comm.py

When it boots, the Arduino tries to send a JSON message via serial:
{"request":"time"}

The python program responds via serial with the current time since midnight in seconds:
{"time": 70602, "alive": true}

Afterwards, the Arduino sends a status message via serial on the interval programmed in sendInterval in setup.cpp. The python program responds with {"alive": true} so that the Arduino knows that that communication path is good. The Arduino does not send another message until it receives a response.