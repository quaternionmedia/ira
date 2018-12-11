# Ira

### WS2811 Lighting controller
Designed, developed and tested with live theatre performances in mind, Ira is an LED lighting controller that can work via a self-contained website, remote controlled as a DMX slave, or used simply as a standalone device.

Works with WS2811 and similar protocols (WS2812, WS2812b, WS2813, etc.) and all other protocols supported by the FastLED library.

#### Current Applications
- Live theatrical productions
- Visual status aid (3D printer)
- Christmas decorations

## Usage
There are many ways to configure these lights, but they all need:
- something to generate the data signal (i.e. Arduino)
- a controller to tell it what to do (can be itself)

The controller might be a DMX lighting board, a python script, a node server, an internal preprogrammed set of effects, or anything else that can send instructions.

### Install
#####  Arduino Libraries needed:
###### LED control:
- https://github.com/FastLED/FastLED
###### Serial handling (optional)
- https://github.com/bakercp/PacketSerial
###### DMX control (optional)
- https://github.com/mathertel/DmxSerial


##  Hardware

###  iRec
##### I2C Receiver, WS2811 controller

- Connect pin 6 (or your PWM pin of choice) to the data in line of the WS28xx lights

## Important considerations

**- - - WARNING - - -** *This setup involves multiple separate (but interconnected) circuits, presumably plugged into other electrically separate and more complex circuits. Follow all electrical safety procedures and double check ALL CONNECTIONS, EVERY TIME!*

**Remember to TEST all lights thoroughly BEFORE installing them!**

## Power
* **Ensure all circuits are connected to a common ground**
- Power all devices with appropriately sized (and preferably dedicated) power supplies.
- Rapidly changing brightness for a large number of lights can create a substantial power supply dip, especially in lower quality power supplies which can cause unpredictable behaviors with the lights, or worse errors in the system.
- When working with a preexisting DMX system, make sure to electrically isolate the DMX receiving circuit with a galvanic isolator and an optocoupler to prevent surges and cross currents from flowing where they shouldn't. See this [dmx shield](http://www.mathertel.de/Arduino/DMXShield.aspx) for a working example.


###  DMX
When controlling via DMX, a separate Arduino DMX receiver is necessary to translate the commands, due to the timing constraints of interrupt based programming for Arduino.
###### DMX Receiver
- Build DMX circuit (#TODO: instructions)
- Galvinically isolate power to MAX485 chip
- Connect the serial out of a MAX485 chip to Arduino RX pin through optocoupler
- Connect the I2C: SDA/SCL wires of the DMX receiver Arduino to the signal generating Arduino
- Compile and upload DMX receiver to Arduino
- Connect DMX to MAX485 chip

## Software
- The WS28xx controller is limited by the available memory in the Arduino. For example, an Arduino nano will only have a capacity to operate a maximum of around 400 LEDs before becoming unresponsive. Use a device with more RAM to operate more lights (i.e. MEGA)
- Interference of many kinds can cause packet loss at any phase of transmission. Communications may not always be reliably delivered. Keep this in mind when sending one-off communications to the controller, especially anywhere you expect it to work every time.
- I2C is more reliable than Serial. Serial connection is more stable at slower baud rates ( <= 1200 ).


## Scale
In theory, a single WS2812 data line can run any number of pixels on a single strand. But some physical issues stand in the way:
- **Power distribution:** - Each LED strand should be powered via both ends of the strand. However, strands with more than about 200 pixels will experience a noticeable dip in brightness in the middle when running at high brightness values (high power draws). This "effect" is generally considered undesirable, but might not be noticeable, depending on your application.
- **Data Communication** -A single data line is also limited by it's speed of communication to all pixels, since the communication is done in serial to each pixel. This means a single data strand with 1024 pixels will have a maximum refresh rate of 30Hz.

## Live Theatre
This system has been designed from the start to be used in live theatrical venues, attached to existing DMX systems, and operated in realtime with an emphasis on responsiveness and realtime reactivity.

However, even with all considerations adhered to, electricity is tricky, and sometimes even small manufacturing imperfections may cause occasional bizarre and/or unpredictable behaviors such as flickering, incorrect colors, or especially when using a large numbers of lights.

Remember no system can ever be 100% reliable, so consider the impact of malfunctioning lights on your project. It may range from unnoticeable to unacceptable, but in any case, always design systems that when they fail, will fail appropriately for the application.
