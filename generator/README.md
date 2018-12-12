# generator

LED signal generator. Needs [Controller](../dmx/dmx.ino) to tell it what to do.
### Dependencies
Install the following  Arduino Libraries:
###### FastLED:
- https://github.com/FastLED/FastLED
###### PacketSerial (optional)
- https://github.com/bakercp/PacketSerial

To use, simply connect pin 6 (or your PWM pin of choice) to the data in line of the WS28xx lights. Use a 200-300 ohm resistor between the Arduino and the first pixel.

Then, compile and upload [generator.ino](generator.ino)

By default, the Arduino will listen for I2C and Serial commands, and can use both simultaneously.
