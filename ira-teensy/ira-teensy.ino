#include <FastLED.h>

#define BAUD 115200
#define STATUS_LED 13

#define NUM_STRIPS 1
#define LED_COUNT 300
#define LED_PIN 15
CRGB color = CRGB( 0, 0, 24 );
CHSV colorHSV;
int pos = 0;
int lpos = pos;
fract8 posFract = 0;
fract8 lposFract = posFract;
volatile uint8_t hue = 0;
fract8 hueFract = 0;
int delta = 0;

uint8_t SPEED = 200;
uint8_t HUE_SPEED = 0;
uint8_t EYESIZE = 20;
uint8_t ARG = 255;

uint8_t fx;



CRGBArray <LED_COUNT> LED;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, HIGH);
  Serial.begin(BAUD);
  analogWriteResolution(8);
  LEDS.addLeds<NUM_STRIPS, WS2812B, LED_PIN, GRB>(LED, LED_COUNT);


}

void loop() {
  analogWrite(STATUS_LED, fx);
   if (Serial.available()) {
    fx = Serial.read();
    Serial.print(fx);
   }
   switch (fx) {
      case 0: wash();
//      Serial.println("washing");
      break;
      case 1: newCylon();
//      Serial.println("cylon");
      break;
    }
  delayMicroseconds(200);
  

  positionIncrement();
  hueIncrement();
}
