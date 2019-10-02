#include <FastLED.h>

#define LED_COUNT 300
#define LED_PIN 6

#define DMX_CHANNELS 3
volatile uint8_t DMX[DMX_CHANNELS];


CRGBArray <LED_COUNT> LED;

#include <PacketSerial.h>
PacketSerial pSerial;

#define PSERIAL_BAUD 1200

void wash(CRGB color) {
  for (int i = 0; i < LED_COUNT; i++)
  {
    LED[i] = color;
  }
  FastLED.show();
}

void onSerial(const uint8_t* buffer, size_t size) {
  memcpy(DMX, buffer, min(size, DMX_CHANNELS));
}

void setup() {
  pSerial.begin(PSERIAL_BAUD);
  pSerial.setPacketHandler(&onSerial);
  LEDS.addLeds<WS2812, LED_PIN, GRB>(LED, LED_COUNT);

}

void loop() {
  pSerial.update();
  CRGB c = { DMX[0], DMX[1], DMX[2] };
  wash(c);
}
