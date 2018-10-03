# ira

## DMX to WS2812 system

###  Arduino Libraries needed:
https://github.com/mathertel/DmxSerial
https://github.com/cpldcpu/light_ws2812
https://github.com/NicksonYap/digitalWriteFast
https://github.com/bakercp/PacketSerial

###  iSend
DMX Receiver, I2C sender

-Compile and upload to any arduino
-Connect the serial out of a MAX485 chip to arduino RX pin
-Connect the I2C SDA and SCL wires to the receiving arduino.

###  iRec
I2C Receiver, WS2812 controller

-Connect pin 6 (or your PWM pin of choice) to the data in line of the WS2812 lights.


### Notes

Important considerations:

*This setup involves multiple seperate (but connected) circuits, presumably plugged into other electrically seperate and more complex circuits. Follow all electrical safety procedures and double check ALL connections EVERY time*
*Ensure all circuits are connected to a common ground*
-Interference of all kinds can cause packet loss at any phase of transmission. Be aware of the limitations of each, and the consequences of lost, missing, or otherwise corrupt data
-When connecting large numbers of lights together, large fast power draws from sudden changes in supply current can lead to errors in the system. Power all devices with approprately sized (and preferably dedicated) power supplies.
-If working with a preexisting DMX system, make sure to electrically isolate the circuit to prevent back current from flowing into the DMX data lines. See point #1 above.

### Scale
In theory, a single WS2812 data line can run any number of pixels on a single strand. The major issue is power distribution, as strands with more tha 200 pixels will start to noticably change color in the middle at high brightness (power draws). This "effect" is generally considered undesireable.

A single data line is also limited by it's speed of communication to all pixels, since the communication is done in serial to each pixel. This means a single data strand with 1024 pixels will have a maximum refresh rate of 30Hz.