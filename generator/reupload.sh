#!/bin/bash
FQBN=esp32:esp32:node32s
# esp8266:esp8266:nodemcuv2
SKETCH=generator.cpp
PORT=/dev/ttyUSB0

BINARY=$SKETCH.$FQBN.bin

arduino-cli compile --fqbn $FQBN $SKETCH && \
# arduino-cli upload -p $PORT --fqbn $FQBN -i $(echo $SKETCH.$FQBN | tr : .).bin
esptool.py -p $PORT write_flash 0x1000 $(echo $BINARY | tr : .)
