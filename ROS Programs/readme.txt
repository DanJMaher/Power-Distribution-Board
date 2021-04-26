power_distro_pub.py
-------------------
Simple program that checks the serial buffer, loads it into a json document, publishes it as the ROS topic "power" and prints it to the console. Responds to the power disto board with the current time if requested, otherwise just pings back to verify that the host is active.