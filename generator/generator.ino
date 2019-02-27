#define DEBUG false
#include <EEPROM.h>

#include <Wire.h>
#define STATUS_LED 13
#define I2C_ADDRESS 42

#define DMX_CHANNELS 8
volatile uint8_t DMX[DMX_CHANNELS];
//uint8_t DMX[DMX_CHANNELS];

//#define USE_HSV
//#include <WS2812.h>

#include <FastLED.h>

#define LED_COUNT 300
#define LED_PIN 6

//WS2812 LED(LED_COUNT);
//CRGB *LED[LED_COUNT];
CRGBArray <LED_COUNT> LED;

CRGB black = CRGB(0, 0, 0);
CRGB qblue = CRGB( 163, 183, 228 );
CRGB last;

uint8_t SPEED = 127;
uint8_t ARG = 0;
uint8_t ARG2 = 0;
uint8_t EYESIZE = 10;
int pos = 0;
int lpos = pos;
int delta = 0;
volatile uint8_t hue = 0;
unsigned long lastUpdate = 0;
volatile bool NEWS = false;

#define BAR 70

//#include <cos_fix.h>

#include <PacketSerial.h>
PacketSerial pSerial;
#define PSERIAL_BAUD 1200

void fadeall() {
  for (int i = 0; i < LED_COUNT; i++) {
    LED[i].nscale8(254);
  }
}

void positionIncrement() {
  lpos = pos;
  if (SPEED < 16) {
    delta = SPEED - 17;
  } else if (SPEED >= 16 && SPEED < 128) {
    delta = -1;
    //    delay((SPEED - 64) * 2 );

    wait((SPEED - 16) * 2);
  } else if (SPEED == 128) {
    delta = 0;
    //freeze
  } else if (SPEED > 128 && SPEED < 240) {
    delta = 1;
    wait((240 - SPEED) * 2);
  } else if (SPEED >= 240) {
    delta = SPEED - 240;
  }
  pos = (pos + delta + LED_COUNT) % LED_COUNT;

}

void wait(int t) {
  unsigned long endTime = millis() + t;
  while (millis() <= endTime) {
    //    delay(0);
    pSerial.update();

  }
}
void wash(CRGB color) {
  for (int i = 0; i < LED_COUNT; i++)
  {
    LED[i] = color;
  }
  FastLED.show();
}

void cylon(CRGB color) {
  if (color == black) {
    color = CHSV(hue, 255, 255);
  }
  for (int i = 0; i <= EYESIZE; i++) {
    LED[(pos + i + LED_COUNT) % LED_COUNT] = color;
  }

  if (delta < 0) {
    for (int i = 0; i > delta; i--) {
      LED[(lpos + EYESIZE + i + LED_COUNT) % LED_COUNT] = black;
    }
  } else {
    for (int i = 0; i < delta; i++) {
      LED[(lpos + i + LED_COUNT) % LED_COUNT] = black;
    }
  }

  FastLED.show();
  hue++;
}

void marquee(CRGB color) {
  if (color == black) {
    color = CHSV(hue, 255, 255);
  }
  unsigned long  t = millis() - lastUpdate;
  if ((t >= (255 - SPEED)) || (t - lastUpdate <= 0)) {
    for (int i = 0; i < LED_COUNT; i++) {
      if (i % (ARG + 2) == 0) {
        LED[(i + pos + LED_COUNT) % LED_COUNT] = color;
      }
      else {
        LED[(i + pos + LED_COUNT) % LED_COUNT] = black;

      }
    }
    FastLED.show();
    lastUpdate = millis();
    hue++;
  } else {
    wait((255 - SPEED) * 4);
  }
}

void wipe(CRGB color) {
  for (int i = 0; i < LED_COUNT; i++) {
    LED[(i + pos) % LED_COUNT] = CRGB(
                                   i < map(color.r, 0, 255, 0, LED_COUNT) ? 255 : 0,
                                   i < map(color.g, 0, 255, 0, LED_COUNT) ? 255 : 0,
                                   i < map(color.b, 0, 255, 0, LED_COUNT) ? 255 : 0
                                 );
  }
  FastLED.show();
}

void waves(CRGB color) {
  if (color == black) {
    color = CHSV(hue, 255, 255);
  }
  for (int i = 0; i < LED_COUNT; i++) {
    uint8_t scalar = sin8((float) ARG * 0xFF * i / LED_COUNT);
    int j = (i + pos) % LED_COUNT;
    LED[j] = color;
    LED[j].nscale8(scalar);
  }
  FastLED.show();
  hue++;
}

void rainbow(CRGB color) {
  if (color == black) {
    color = CHSV(hue, 255, 255);
  }
  uint8_t thisHue = hue;
  int cycles = floor(LED_COUNT / EYESIZE);
  for (int i = 0; i <= cycles; i++) {
    for (CRGB & pixel : LED(i*EYESIZE , min((i+1)*EYESIZE, LED_COUNT))) {
      pixel = CHSV((thisHue + (i*ARG)) % 256, 255, ARG2);
    }
    thisHue = (thisHue + ARG) % 255;
  
  }
  hue++;
  FastLED.show();
  //  fadeall();
}

void newRainbow(CRGB color) {
  uint8_t thisHue = hue;
  for (int i = pos; i < LED_COUNT; i++) {
    if (i % EYESIZE == 0) {
    thisHue = (thisHue + ARG) % 256;
    }
    LED[i] = CHSV(thisHue, 255, ARG2);
  }
  for (int i = 0; i < pos; i++) {
    if (i % EYESIZE == 0) {
    thisHue = (thisHue + ARG) % 256;
    }
    LED[i] = CHSV(thisHue, 255, ARG2);
  }
  FastLED.show();
  hue++;
}

void glitter(CRGB color) {
  if ( random8() <= SPEED) {
    if (color == black) {
      //when input == black, use hue cycle
      LED[ random16(LED_COUNT) ] += CHSV(hue++, 255, random8());
    } else {
      // else use color
      LED[ random16(LED_COUNT) ] += color;
    }
  }
  FastLED.show();
  fadeall();
}

void glitterRandom(CRGB color) {
  if ( random16(383) <= SPEED + delta + 64) {
    LED[ random16(LED_COUNT) ] += CHSV(random8(), 255, random8());
  }
  FastLED.show();
  fadeall();
}

void chaser(CRGB color) {
  fadeToBlackBy( LED, LED_COUNT, 20);
  for ( int i = 0; i < 2; i++) {
    LED[beatsin16( i + 7, 0, LED_COUNT - 1 )] |= color;
  }
  FastLED.show();

}

void newCylon(CRGB color) {
    if (color == black) {
    color = CHSV(hue, 255, 255);
  }
  if ((pos + EYESIZE) >= LED_COUNT) {
    for (CRGB & pixel : LED(pos, LED_COUNT)) {
      pixel = color;
    }
    for (CRGB & pixel : LED(0, (pos + EYESIZE) % LED_COUNT)) {
      pixel = color;
    }
  } else {
    for (CRGB & pixel : LED(pos, (pos + EYESIZE) % LED_COUNT)) {
      pixel = color;
    }
  }
  LED.fadeToBlackBy(ARG);
  FastLED.show();
  hue++;
}

void progress(uint8_t p, uint8_t b, uint8_t t) {
  int split = BAR * p / 100;
  int remainder = (BAR * p / 100 - split) * 1000;
  for (int i = 0; i < split; i++) {
    LED[i] = CRGB::Green;
  }
  LED[split] = CRGB (map( remainder, 0, 1000, 0, 255), 0, 0);

  for (int i = split + 1; i < BAR; i++) {
    LED[i] = black;
  }
  for (int i = BAR; i < 172; i++) {
    LED[i] = qblue;
  }
  for (int i = 172; i < 190; i++) {
    LED[i] = CRGB (map(b, 20, 60, 0 , 127), map(b, 20, 60, 0, 255), map(b, 20, 60, 255, 0) );
  }
  for (int i = 190; i < 208; i++) {
    LED[i] = CRGB ( 0, map(t, 20, 220, 0, 255), map(t, 20, 220, 255, 0) );
  }
  for (int i = 208; i < LED_COUNT; i++) {
    LED[i] = black;
  }

  FastLED.show();

}

void newData(int n) {
  for (int i = 0; i < n; i++) {
    DMX[i] = Wire.read();
  }
  //  if (DEBUG) {
  //    Serial.write(DMX, DMX_CHANNELS);
  //  }
}

void onSerial(const uint8_t* buffer, size_t size) {
  memcpy(DMX, buffer, min(size, DMX_CHANNELS));

  //  analogWrite(STATUS_LED, buffer[0]);
  if (DEBUG) {
    pSerial.send(buffer, size);
  }
  NEWS = true;

}

void setup() {
  pinMode(13, OUTPUT);
  //  Wire.setClock(400000);
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(newData);
  //  Serial.begin(9600);
  //  while (!Serial) {
  //  }
  pSerial.begin(PSERIAL_BAUD);
  pSerial.setPacketHandler(&onSerial);

  //  LED.setOutput(LED_PIN);
  LEDS.addLeds<WS2812, LED_PIN, RGB>(LED, LED_COUNT);

  //  DMX[1] = 255;
  //  DMX[2] = 255;
  //  DMX[3] = 255;
  for (int i = 0; i < DMX_CHANNELS; i++) {
    EEPROM.get(i, DMX[i]);
  }
}

void loop() {
  pSerial.update();
  analogWrite(STATUS_LED, DMX[0]);
  CRGB c = { DMX[2], DMX[1], DMX[3] };
  SPEED = DMX[4];
  EYESIZE = DMX[5];
  ARG = DMX[6];
  ARG2 = DMX[7];

  if (NEWS) {
    for (int i = 0; i < DMX_CHANNELS; i++) {
      EEPROM.put(i, DMX[i]);
    }
  }
  NEWS = false;

  if (DMX[0] < 25) {
    wash(c);
  } else if (DMX[0] >= 25 && DMX[0] < 51) {
    cylon(c);
  } else if (DMX[0] >= 51 && DMX[0] < 76) {
    lastUpdate = 0;
    marquee(c);
  } else if (DMX[0] >= 76 && DMX[0] < 102) {
    wipe(c);
  } else if (DMX[0] >= 102 && DMX[0] < 127) {
    waves(c);
  } else if (DMX[0] >= 127 && DMX[0] < 153) {
    newRainbow(c);
  } else if (DMX[0] >= 153 && DMX[0] < 178) {
    glitter(c);
  } else if (DMX[0] >= 178 && DMX[0] < 204) {
    glitterRandom(c);
  } else if (DMX[0] >= 204 && DMX[0] < 229) {
    chaser(c);
  } else if (DMX[0] >= 229 && DMX[0] < 255) {
    newCylon(c);
  } else if (DMX[0] == 255) {
    progress(DMX[1], DMX[2], DMX[3]);
    //  // else {error();}
  }
  //
  positionIncrement();
  //  pos = (pos + 1 ) % LED_COUNT;
}
