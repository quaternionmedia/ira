#ifndef IRA
#define IRA

#define DEBUG false

#define BAUD 1200
#define STATUS_LED 13

#define NUM_STRIPS 1
#define LED_COUNT 50
#define LED_PIN 6


#define DMX_CHANNELS 8
uint8_t DMX[DMX_CHANNELS];

uint8_t fx;
bool NEWS;

#include <PacketSerial.h>

void wait(int t);


#endif
