#!/bin/bash
FQBN=esp8266com:esp8266:generic
SKETCH=generator.cpp
PORT=/dev/ttyUSB0


arduino-cli compile --fqbn $FQBN $SKETCH && \
arduino-cli upload -p $PORT --fqbn $FQBN -i $(echo $SKETCH.$FQBN | tr : .).bin
