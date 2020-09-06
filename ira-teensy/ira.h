#ifndef IRA
#define IRA

#define DEBUG false

#define BAUD 115200
#define STATUS_LED 13

#define NUM_STRIPS 1
#define LED_COUNT 300
#define LED_PIN 15

uint8_t fx;
bool NEWS;

#define DMX_CHANNELS 8
uint8_t DMX[DMX_CHANNELS];

#include <PacketSerial.h>
PacketSerial pSerial;
#define PSERIAL_BAUD 115200

#endif
