#include <FastLED.h>

#define BAUD 115200
#define STATUS_LED 13


#define LED_COUNT 300
#define LED_PIN 15
CRGB color = CRGB( 255, 255, 254 );
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
uint8_t ARG = 10;

uint8_t fx;



CRGBArray <LED_COUNT> LED;

void positionIncrement() {
  lpos = pos;
  lposFract = posFract;
  delta = SPEED - 128;
  if (delta > 0) {
    posFract += delta;
    pos = (pos + (delta >> 6) + LED_COUNT) % LED_COUNT;
    if (posFract < lposFract) {
      pos = (pos + 1) % LED_COUNT;
    }
  } else {
    posFract -= -delta;
    pos = (pos - (-delta >> 6) + LED_COUNT) % LED_COUNT;
    if (posFract > lposFract) {
      pos = (pos - 1 + LED_COUNT) % LED_COUNT;
    }
  }
}

void hueIncrement() {
  colorHSV = rgb2hsv_approximate(color);

  if (HUE_SPEED > 0) {
    colorHSV.h += hue;
  } else {
    hue = 0;
  }
  hueFract += HUE_SPEED;
  if (hueFract < HUE_SPEED) {
    hue++;
  }
}

void fadeall() {
  LED.nscale8(min(ARG + 1, 254));
}
void wash() {
  for (CRGB & pixel : LED(0, LED_COUNT)) {
    pixel = color;
  }
  FastLED.show();
}
void newCylon() {

  if ((pos + EYESIZE) >= LED_COUNT) {
    for (CRGB & pixel : LED(pos, LED_COUNT)) {
      pixel = HUE_SPEED > 0 ? colorHSV : color;
    }
    for (CRGB & pixel : LED(0, (pos + EYESIZE) % LED_COUNT)) {
      pixel = HUE_SPEED > 0 ? colorHSV : color;
    }
  } else {
    for (CRGB & pixel : LED(pos, (pos + EYESIZE) % LED_COUNT)) {
      pixel = HUE_SPEED > 0 ? colorHSV : color;
    }
  }

  LED[(pos + EYESIZE) % LED_COUNT] %= posFract;
  if (delta < 1) {
    LED[pos] %= 255 - posFract;
  }

  FastLED.show();
  fadeall();

}
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, HIGH);
  Serial.begin(BAUD);
  analogWriteResolution(8);
  LEDS.addLeds<WS2812, LED_PIN, GRB>(LED, LED_COUNT);


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
  delay(10);
  

  positionIncrement();
  hueIncrement();
}
