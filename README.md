# Ira
A flexible LED controller designed, developed and tested with live theatrical performances in mind
![alt text](https://static1.squarespace.com/static/5b54f2433e2d096995211b77/t/5bca9b79c8302525bdb386ad/1540004748599/20181019_190249.jpg?format=1500w "Over 1000 pixels, used in a recent production")

Ira is an LED lighting control system that can operate via a self-contained website, remote controlled as a DMX slave, or simply used as a standalone device.

Works with all WS28xx series (WS2811, WS2812b, WS2813, etc.) and other protocols supported by the FastLED library.

#### Current Applications
- [Live theatrical productions](https://quaternion.media/ira)
- [Visual status aid for 3D printer](https://github.com/quaternionmedia/OctoPrint-Ira)
- Server rack status indicator
- Halloween and Christmas decorations


## Usage
See [wiki](../../wiki) for full usage. Below is an abbreviated summary.

####  Hardware
In order to use these to control lights, you need several things:
- Lights - WS2811 or compatible series lights
- A signal **Generator**  - This turns the current effect into the proper signal to send to the lights. Usually a microcontroller, such as an Arduino or esp32.
- An information **Controller** - something to tell it what to do and when (this can be the same as the microcontroller, or it might be an external source of information, or both!)

##  Generator
This is the actual WS2811 controller, in charge of constantly generating the right signal for the lights. It can receive information from I2C, Serial, or even a physical button.

See [examples/basic](examples/basic) for a simple version, or [generator](generator) for a full example.

## Controller
The controller might be a DMX lighting board, a python script, a node server with a frontend, an internal preprogrammed set of effects, or anything else that can send instructions. Details for [Serial](../../wiki/Serial), [I2C](../../wiki/), [DMX](../../wiki/), and [Server](../../wiki/) interfaces can be found in the [wiki](../../wiki/)

## **- - - CAUTION - - -**
Electricity is dangerous. Follow all appropriate safety guidelines and obey all electrical and fire safety laws at all times!
