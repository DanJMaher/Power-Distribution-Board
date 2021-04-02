Updated 3/29/21 - Dan Maher
(more info to come)

Basic use:
1) Plug in an Arduino, send the code. 
2) run serial_comm.py (for basic communication)

When it boots, the Arduino tries to send a JSON message via serial:
{"request":"time"}

The python program responds via serial with the current time since midnight in seconds:
{"time": 70602, "alive": true}

Afterwards, the Arduino sends a status message via serial on the interval programmed in sendInterval in setup.cpp. The python program responds with {"alive": true} so that the Arduino knows that that communication path is good. The Arduino does not send another message until it receives a response.

The serial_comm.py program is just for troubleshooting, and copies the incoming/outgoing serial messages to the console so the user can see what's being sent/received.
---------------------------------------------------------------------------
REMOTE JSON COMMANDS
---------------------------------------------------------------------------
{"relay":[bool, bool]}		-		Sets individual relays to on/off
{"time":unsigned long}	-		Sync system time with long (in seconds)
{"shutdown":"now"}		-		Shuts down the system with code "remote"
---------------------------------------------------------------------------
INCOMING JSON MESSAGES
---------------------------------------------------------------------------
{"time":unsigned long}		-		Current timestamp
{"voltage:[float,float]}	-		Current voltages (11.1, 22.2)
{"relay":[bool,bool]}		-		Current relay status (11.1, 22.2)
{"comm status"[bool, bool]}	-		Current comm status path 1/2 (false is communication failure)
{"SHUTDOWN":string} 		-		System is shutting down for reason:
						"auto" - low voltage
						"remote" - remote serial command
						"button" - power button was pushed		