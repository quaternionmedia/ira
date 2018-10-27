# ira

## DMX to WS2812 system

###  Arduino Libraries needed:
- https://github.com/mathertel/DmxSerial
- https://github.com/cpldcpu/light_ws2812
- https://github.com/NicksonYap/digitalWriteFast
- https://github.com/bakercp/PacketSerial

###  iSend
DMX Receiver, I2C sender

- Compile and upload to any arduino
- Connect the serial out of a MAX485 chip to arduino RX pin
- Connect the I2C SDA and SCL wires to the receiving arduino.

###  iRec
I2C Receiver, WS2812 controller

- Connect pin 6 (or your PWM pin of choice) to the data in line of the WS2812 lights.


### Notes

Important considerations:
####  Hardware

**WARNING**: *This setup involves multiple separate (but connected) circuits, presumably plugged into other electrically separate and more complex circuits. Follow all electrical safety procedures and double check ALL connections EVERY time*

* **Ensure all circuits are connected to a common ground**
- Interference of all kinds can cause packet loss at any phase of transmission. Be aware of the limitations of each, and the consequences of lost, missing, or otherwise corrupt data
- When connecting large numbers of lights together, large fast power draws from sudden changes in supply current can lead to errors in the system. Power all devices with appropriately sized (and preferably dedicated) power supplies.
- If working with a preexisting DMX system, make sure to electrically isolate the circuit to prevent back current from flowing into the DMX data lines. See point #1 above.

#### Software
- The WS28xx controller is limited by the available memory in the arduino. For example, an arduino nano will only have a capacity to operate a maximum of 415 LEDs (don't ask me why) before becoming unresponsive. Use an arduino with more RAM to operate more lights (i.e. MEGA)
- Even at lower LED_COUNTs, communication may not be reliably delivered, especially when using the PacketSerial interface. Keep this in mind when sending one-off communications to the controller, especially anywhere you expect it to work every time. Use I2C and the Wire library for the most reliable communications channel.


### Scale
In theory, a single WS2812 data line can run any number of pixels on a single strand. But some physical issues stand in the way:
- **Power distribution:** - Each LED strand should be powered via both ends of the strand. However, strands with more than about 200 pixels will experience a noticeable dip in brightness in the middle when running at high brightness values (high power draws). This "effect" is generally considered undesirable, but might not be noticeable, depending on your application.
- **Data Communication** -A single data line is also limited by it's speed of communication to all pixels, since the communication is done in serial to each pixel. This means a single data strand with 1024 pixels will have a maximum refresh rate of 30Hz.
