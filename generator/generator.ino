#define DEBUG false
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "Secrets.h"
// #include "API.h"
#include <AsyncJson.h>
#include <ArduinoJson.h>

#include <Wire.h>
#define STATUS_LED 13
#define I2C_ADDRESS 42

#define DMX_CHANNELS 8
uint8_t DMX[DMX_CHANNELS];
//uint8_t DMX[DMX_CHANNELS];

//#define USE_HSV
//#include <WS2812.h>

#include <FastLED.h>

#define LED_COUNT 300
#define LED_PIN 16

//WS2812 LED(LED_COUNT);
//CRGB *LED[LED_COUNT];
CRGBArray <LED_COUNT> LED;

CRGB color;
CHSV colorHSV;
CRGB black = CRGB(0, 0, 0);
CRGB qblue = CRGB( 163, 183, 228 );
CRGB last;

uint8_t SPEED = 127;
uint8_t HUE_SPEED = 0;
uint8_t EYESIZE = 10;
uint8_t ARG = 0;
int pos = 0;
fract8 posFract = 0;
fract8 hueFract = 0;
int lpos = pos;
fract8 lposFract = posFract;
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
  // for (CRGB & pixel : LED(0, LED_COUNT)) {
  //   pixel.nscale8(254 - ARG);
  // }
  LED.nscale8(min(ARG + 1, 254));
}

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

void wait(int t) {
  unsigned long endTime = millis() + t;
  while (millis() <= endTime) {
    //    delay(0);
    pSerial.update();

  }
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
    wait((255 - SPEED) * 4);
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
int max(int a, int b) {
  return a > b ? a : b;
}
int min(int a, int b) {
  return a > b ? a : b;
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

void onSerial(const uint8_t *buffer, size_t size) {
  memcpy(DMX, &buffer, min(size, DMX_CHANNELS));

  //  analogWrite(STATUS_LED, buffer[0]);
  if (DEBUG) {
    pSerial.send(buffer, size);
  }
  NEWS = true;

}
AsyncWebServer server(80);
AsyncEventSource status("/status");
DynamicJsonDocument doc(200);
JsonArray jdmx = doc.createNestedArray("dmx");


void setup() {
  pinMode(13, OUTPUT);
  //  Wire.setClock(400000);
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(newData);
    Serial.begin(9600);
    while (!Serial) {
    }
//  pSerial.begin(PSERIAL_BAUD);
//  pSerial.setPacketHandler(&onSerial);

  //  LED.setOutput(LED_PIN);
  LEDS.addLeds<WS2812, LED_PIN, RGB>(LED, LED_COUNT);

  //  DMX[1] = 255;
  //  DMX[2] = 255;
  //  DMX[3] = 255;
  for (int i = 0; i < DMX_CHANNELS; i++) {
    EEPROM.get(i, DMX[i]);
  }
  WiFi.begin(WiFiName, WiFiPassword);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());


}

void loop() {
  pSerial.update();
  analogWrite(STATUS_LED, DMX[0]);
  color = CRGB( DMX[2], DMX[1], DMX[3] );
  SPEED = DMX[4];
  HUE_SPEED = DMX[5];
  EYESIZE = DMX[6];
  ARG = DMX[7];

  if (NEWS) {
    for (int i = 0; i < DMX_CHANNELS; i++) {
      EEPROM.put(i, DMX[i]);
      jdmx[i] = DMX[i];
    }
  }
  NEWS = false;

  if (DMX[0] < 25) {
    wash();
  } else if (DMX[0] >= 25 && DMX[0] < 51) {
    newCylon();
  } else if (DMX[0] >= 51 && DMX[0] < 76) {
    lastUpdate = 0;
    marquee();
  } else if (DMX[0] >= 76 && DMX[0] < 102) {
    wipe();
  } else if (DMX[0] >= 102 && DMX[0] < 127) {
    waves();
  } else if (DMX[0] >= 127 && DMX[0] < 153) {
    newRainbow();
  } else if (DMX[0] >= 153 && DMX[0] < 178) {
    glitter();
  } else if (DMX[0] >= 178 && DMX[0] < 204) {
    chaser();
  } else if (DMX[0] >= 204 && DMX[0] < 229) {
    hueCycle();
  } else if (DMX[0] >= 229 && DMX[0] < 255) {
    hueCycle();
  } else if (DMX[0] == 255) {
    progress(DMX[1], DMX[2], DMX[3]);
    //  // else {error();}
  }
  //
  positionIncrement();
  hueIncrement();
  //  pos = (pos + 1 ) % LED_COUNT;
}
