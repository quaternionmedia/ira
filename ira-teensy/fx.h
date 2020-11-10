
#ifndef FX
#define FX

#ifndef ARDUINO
int max(int a, int b) {
  return a > b ? a : b;
}
int min(int a, int b) {
  return a > b ? a : b;
}
#endif

#include "ira.h"

#include <FastLED.h>
CRGBArray <LED_COUNT> LED;

CRGB color = CRGB( 0, 0, 24 );
CRGB black = CRGB(0, 0, 0);
CHSV colorHSV;
int pos = 0;
int lpos = pos;
fract8 posFract = 0;
fract8 lposFract = posFract;
volatile uint8_t hue = 0;
fract8 hueFract = 0;
int delta = 0;
int lastUpdate;


uint8_t SPEED = 200;
uint8_t HUE_SPEED = 0;
uint8_t EYESIZE = 20;
uint8_t ARG = 255;

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


void marquee() {
  // if (color == black) {
  //   color = CHSV(hue, 255, ARG + 1);
  // }
  int p = 0;
  unsigned long  t = millis() - lastUpdate;
  if ((t >= (255 - SPEED)) || (t - lastUpdate <= 0)) {
    for (int i = 0; i < LED_COUNT; i++) {
      if (i % (EYESIZE + 2) == 0) {
        p = (i + pos + LED_COUNT) % LED_COUNT;
        LED[p] = HUE_SPEED > 0 ? colorHSV : color;
        if (ARG > 0) {
          if (delta < 0) {
            LED[p].nscale8(255 - posFract);
          } else {
            LED[p].nscale8(posFract);
          }
        }
      }
      // else {
      // ARG == 0 ? LED[p] = black;
      // }
    }
    FastLED.show();
    lastUpdate = millis();
  } else {
//    wait((255 - SPEED) * 4);
  }
  fadeall();

}

void wipe() {
  for (int i = 0; i < LED_COUNT; i++) {
    LED[(i + pos) % LED_COUNT] = CRGB(
      i < map(color.r, 0, 255, 0, LED_COUNT) ? 255 : 0,
      i < map(color.g, 0, 255, 0, LED_COUNT) ? 255 : 0,
      i < map(color.b, 0, 255, 0, LED_COUNT) ? 255 : 0
    );
  }
  FastLED.show();
  fadeall();
}

void waves() {
  // if (color == black) {
  //   color = CHSV(hue, 255, 255);
  // }
  for (int i = 0; i < LED_COUNT; i++) {
    uint8_t scalar = sin8((float) ARG * 0xFF * i / LED_COUNT);
    int j = (i + pos) % LED_COUNT;
    LED[j] = colorHSV;
    LED[j].nscale8(scalar);
  }
  FastLED.show();
}

void rainbow() {
  //  if (color == black) {
  //    color = CHSV(hue, 255, 255);
  //  }
  uint8_t thisHue = hue;
  int cycles = floor(LED_COUNT / EYESIZE);
  for (int i = 0; i <= cycles; i++) {
    for (CRGB & pixel : LED(i * EYESIZE , min((i + 1)*EYESIZE, LED_COUNT))) {
      pixel = CHSV((thisHue + (i * ARG)) % 256, 255, HUE_SPEED + 1);
    }
    thisHue = (thisHue + ARG) % 256;
  }
  FastLED.show();
  //  fadeall();
}

void newRainbow() {
  uint8_t thisHue = hue;
  //  int i = 0;
  for (int i = 0; i < LED_COUNT; i++) {
    if ((i + pos) % EYESIZE == 0) {
      //      thisHue = (thisHue + ARG) % 256;
      thisHue += ARG;
    }
    LED[(i + pos) % LED_COUNT] = CHSV(thisHue, 255, max(ARG, 1));
  }
  //  for (int i = 0; i < pos; i++) {
  //    if ((i + pos) % EYESIZE == 0) {
  //      //      thisHue = (thisHue + ARG) % 256;
  //      thisHue += ARG;
  //    }
  //    //for (CRGB & pixel : LED(0, LED_COUNT)) {
  //    LED[i] = CHSV(thisHue, 255, HUE_SPEED);
  //    if (i ) {
  //
  //    }
  //    i++
  //  }
  FastLED.show();
  //  hue++;
}

void glitter() {
  if ( random8() <= SPEED) {
    if (color == black) {
      //when input == black, use hue cycle
      LED[ random16(LED_COUNT) ] += CHSV(hue, random8(), random8());
    } else {
      // else use color
      LED[ random16(LED_COUNT) ] += HUE_SPEED > 0 ? colorHSV : color;
    }
  }
  FastLED.show();
  fadeall();
}

void glitterRandom() {
  if ( random16(383) <= SPEED + delta + 64) {
    LED[ random16(LED_COUNT) ] += CHSV(random8(), 255, random8());
  }
  FastLED.show();
  fadeall();
}

void chaser() {
  fadeToBlackBy( LED, LED_COUNT, 20);
  for ( int i = 0; i < 2; i++) {
    LED[beatsin16( i + 7, 0, LED_COUNT - 1 )] |= color;
  }
  FastLED.show();

}

void hueCycle() {
  //  for (CRGB & pixel : LED(0, LED_COUNT)) {
  //    pixel = CHSV(hue, ARG, HUE_SPEED);
  //  }
  fill_rainbow(LED, LED_COUNT, hue);
  fadeall();
  FastLED.show();

}


#endif
