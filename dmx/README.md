#Ira/dmx

### Dependencies
- https://github.com/mathertel/DMXSerial
### Usage
- [Build DMX circuit](http://www.mathertel.de/Arduino/DMXShield.aspx)
- Galvanically isolate power to MAX485 chip
- Optically isolate the serial out of the MAX485 chip to the Arduino RX pin
- Connect the I2C: SDA/SCL wires of the DMX receiver Arduino to the LED signal generating Arduino
- Compile and upload [dmx.ino](dmx/dmx.ino)
- Connect the isolated MAX485 chip to your DMX universe


## DMX Isolation
When working with a preexisting DMX system, it is extremely important to electrically isolate the DMX receiving circuit with both a galvanic isolator and an optocoupler to prevent surges and cross currents from flowing where they shouldn't. See this [dmx shield](http://www.mathertel.de/Arduino/DMXShield.aspx) for a working example.
