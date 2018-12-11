# Ira
A flexible LED controller designed, developed and tested with live theatrical performances in mind
![alt text](https://static1.squarespace.com/static/5b54f2433e2d096995211b77/t/5bca9b79c8302525bdb386ad/1540004748599/20181019_190249.jpg?format=1500w "Over 1000 lights, used in a recent production")

Ira is an LED lighting control system that can operate via a self-contained website, remote controlled as a DMX slave, or simply used as a standalone device.

Works with all WS28xx series (WS2811, WS2812b, WS2813, etc.) and other protocols supported by the FastLED library.

#### Current Applications
- [Live theatrical productions](https://quaternion.media/ira)
- [Visual status aid for 3D printer](https://github.com/quaternionmedia/OctoPrint-Ira)
- Christmas decorations


## Usage
####  Hardware
There are many ways to configure these lights, but they all need:
- [Lights] - some number of lights
- [Power] - some power supply[s]
- [Generator] - something to generate the data signal (i.e. Arduino)
- [Controller] - something to tell it what to do and when (this can be itself)

### Lights
Any number of WS series lights, connected in series or in parallel

### Power
- Power all pixels using adequately sized power supplies.
- Use a separate power supply for the Arduino[s] than the LEDs.
- **Connect all grounds together** - ensure every circuit has a clear path to ground, including all power suppl[y|ies] used for LEDs as well as the grounds of all Arduino power supplies.


##  Generator
##### the actual WS2811 controller

### Dependencies
Install the following  Arduino Libraries:
###### FastLED:
- https://github.com/FastLED/FastLED
###### PacketSerial (optional)
- https://github.com/bakercp/PacketSerial

- Connect pin 6 (or your PWM pin of choice) to the data in line of the WS28xx lights. Use a 200-300 ohm resistor between the Arduino and the first pixel
- Compile and upload [generator.ino](generator/generator.ino)

By default, the Arduino will listen for I2C and Serial commands, and can use both simultaneously.

## Controller
The controller might be a DMX lighting board, a python script, a node server with a frontend, an internal preprogrammed set of effects, or anything else that can send instructions.

### DMX
When controlling via DMX, a separate Arduino DMX receiver is necessary to translate the commands, due to the timing constraints of interrupt based programming for Arduino.

### Dependencies
- https://github.com/mathertel/DMXSerial
### Usage
- Build DMX circuit (#TODO: instructions)
- Galvinically isolate power to MAX485 chip
- Optically isolate the serial out of the MAX485 chip to the Arduino RX pin
- Connect the I2C: SDA/SCL wires of the DMX receiver Arduino to the signal generating Arduino
- Compile and upload [dmx.ino](dmx/dmx.ino)
- Connect isolated MAX485 chip to DMX universe

## Communication
#### I2C
For performances and mission critical transmissions, I2C is more reliable than Serial and should be preferred.
Simplified example (from [dmx/dmx.ino](dmx/dmx.ino))

```arduino
const byte I2C_ADDRESS = 42;
uint8_t DMX_PACKET[DMX_CHANNELS];

void sendPacket() {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(DMX_PACKET, DMX_CHANNELS);
  while (Wire.endTransmission()) {
  }
}

void loop() {
  readDMX();
  //if (NEWS) {
  sendPacket();
  }//endif
}
```

#### Serial
Using the Serial interface is possible, but more prone to lost packets and message errors due to the heavy use of interrupts. The Serial connection becomes stable only at sufficiently slow baud rates ( <= 1200 ).

Example using packet.py:
```python
import packet
packet.send(PROGRAM, R, G, B, SPEED)

# prints message if received
```
All values sent should be from 0 to 255.

**COLOR** is {**R**, **G**, **B**}
**SPEED** is an optional global speed control value
**PROGRAM** is the effect selector. The following programs are set by default:
- [0-24] wash(COLOR)
- [25-50] cylon(COLOR)
- [51-75] cylonColor(COLOR)
- [76-101] rain(COLOR)
- [102-126] rainbow(COLOR)
- [127-152] glitter(COLOR)
- [153-177] glitterCOLOR(COLOR)
- [178-203] glitterRandom(COLOR)
- [204-228] chaser(COLOR)
- [229-255] progress(PROGRESS, BED, EXTRUDER)

The **progress** function exists for use with the [octoprint plugin](https://github.com/quaternionmedia/OctoPrint-Ira) for 3D printer status functionality.


## Important considerations

**- - - WARNING - - -** *This setup involves multiple separate (but interconnected) circuits, presumably plugged into other electrically separate and more complex circuits. Follow all electrical safety procedures and double check ALL CONNECTIONS, EVERY TIME!*

- **Ensure all circuits are connected to a common ground!**
- Rapidly changing brightness for a large number of lights can create a substantial power supply dip, especially in lower quality power supplies which can cause unpredictable behaviors with the lights, or worse errors in the system.
- Always TEST all lights thoroughly BEFORE installing them!

#### DMX Isolation
When working with a preexisting DMX system, it is extremely important to electrically isolate the DMX receiving circuit with both a galvanic isolator and an optocoupler to prevent surges and cross currents from flowing where they shouldn't. See this [dmx shield](http://www.mathertel.de/Arduino/DMXShield.aspx) for a working example.


#### Scale
In theory, a single WS2812 data line can run any number of pixels on a single strand. But some physical issues stand in the way:
- **Arduino memory** - each pixel takes 3 bytes of SRAM to hold color information, which means an Uno/Nano can run about 300-400 pixels before running out of SRAM. Use a Mega when dealing with large numbers of pixels.
- **Data communication** - single wire communication protocols like WS series lights require communicating to each pixel sequentially, where each pixel takes ~30µs. This means a single data strand with 1024 pixels will have a refresh rate of about 30Hz.
- **Interference** of many kinds can cause packet loss at any phase of transmission. Communications may not always be reliably delivered. Keep this in mind when sending one-off communications to the controller, especially anywhere you expect it to work every time.
- **Power distribution:** - each LED strand should be powered from both ends of the strand. However, strands with more than about 200 pixels will still experience a noticeable dip in brightness in the middle when running at high brightness values (high power draws). This "effect" is generally considered undesirable, but might not be noticeable, depending on your application.



## Live Theatre
This system has been designed from the start to be used in live theatrical venues, attached to existing DMX systems, and operated in realtime with an emphasis on responsiveness and realtime reactivity.

However, even with all considerations adhered to, electricity is tricky, and sometimes even small manufacturing imperfections may cause occasional bizarre and/or unpredictable behaviors such as flickering, incorrect colors, especially when using a large numbers of lights.

Theaters in general are infamous for having inconsistent power, particularly with large rapid changes from moment to moment. Proper electrical isolation for DMX circuits is a must, in addition to following all normal electrical safety measures for high amperage circuits.

Remember no system can ever be 100% reliable, so consider the impact of malfunctioning lights, power, or other components on your project. It may range from unnoticeable to unacceptable, or even catastrophic, so always design systems to fail appropriately for the application.


## **- - - CAUTION - - -**
Electricity is dangerous. Follow all appropriate safety guidelines and obey all electrical and fire safety laws at all times!
