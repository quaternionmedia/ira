#include <Wire.h>
#define STATUS_LED 13
#define I2C_ADDRESS 42

#define DMX_CHANNELS 6
volatile uint8_t DMX[DMX_CHANNELS];

#define USE_HSV
#include <WS2812.h>

#define LED_COUNT 300
#define LED_PIN 6

WS2812 LED(LED_COUNT);
cRGB black = { 0, 0, 0 };
cRGB last;

uint8_t SPEED = 127;
uint8_t EYESIZE = 10;
int position = 0;

#include <cos_fix.h>

void positionIncrement() {
  if (SPEED < 64) {
    position = (position - (64 - SPEED)) % LED_COUNT;
  } else if (SPEED >= 64 && SPEED < 128) {
    position = (position - 1 ) % LED_COUNT;
    delay(SPEED - 64);
  } else if (SPEED == 128) {
    //freeze
  } else if (SPEED > 128 && SPEED < 192) {
    position = (position + 1 ) % LED_COUNT;
    delay(192 - SPEED);
  } else if (SPEED >= 192) {
    position = (position + (SPEED - 192)) % LED_COUNT;
  }
}

void wash(cRGB color) {
  for (int i = 0; i < LED_COUNT; i++)
  {
    LED.set_crgb_at(i, color);
  }
  LED.sync();
}

void cylon(cRGB color) {
  last = color;
  cRGB dim;
  dim.r = int(color.r / 10);
  dim.g = int(color.g / 10);
  dim.b = int(color.b / 10);
  LED.set_crgb_at(position, dim);
  for (int i = 1; i <= EYESIZE; i++) {
    LED.set_crgb_at((position + i) % LED_COUNT, color);
  }
  LED.set_crgb_at((position + EYESIZE ) % LED_COUNT, dim);
  LED.set_crgb_at((position - 1) % LED_COUNT, black);

  LED.sync();
  //position = (position + 1 ) % LED_COUNT;
  //delay(DELAY);
}

void staticRainbow(cRGB color) {

  for (int i = 0; i < LED_COUNT; i++) {
    LED.set_crgb_at(i, { byte(color.g * (1 + sin(2 * PI*i / LED_COUNT)) / 2),
                         byte(color.r * (1 + cos(2 * PI*i / LED_COUNT)) / 2),
                         byte(color.b * (1 + sin((2 * PI * i + 128)) / LED_COUNT)) / 2
                       });
  }
  LED.sync();
}

void rainbow(cRGB color) {
  for (int i = 0; i < LED_COUNT; i++) {
    uint8_t scalar = (1 + sin_fix(2 * PI * i * LED_COUNT)) / 2;
    LED.set_crgb_at(i, {
      (color.g + position) % 256,
      (color.r + position) % 256,
      (color.b - position) % 256
    }) ;
  }
  LED.sync();

}

void rain(cRGB color) {
  for (int i = 0; i < LED_COUNT; i++) {
    uint8_t scalar = byte(255 * (1 + sin_fix(i) / 2));
    LED.set_crgb_at((i + position) % LED_COUNT,
    {
      byte(color.g * scalar),
      byte(color.r * scalar),
      byte(color.b * scalar)
    });
  }
  LED.sync();

  //position = (position - 1 ) % LED_COUNT;
}

void sparkle(cRGB color) {
  //  int t = millis()/(256 - SPEED);
  for (int i = 0; i < LED_COUNT; i++) {
    uint8_t scalar = byte(255 * (1 + sin_fix(position + 2 * PI * i / LED_COUNT)) / 2);
    LED.set_crgb_at(i, {
      byte(color.g * scalar),
      byte(color.r * scalar),
      byte(color.b * scalar)
    });
  }
  LED.sync();
  //position = (position - 1 ) % LED_COUNT;
}

void newData(int n) {
  for (int i = 0; i < n; i++) {
    DMX[i] = Wire.read();
  }
  //  Serial.write(DMX, DMX_CHANNELS);
}


void setup() {
  pinMode(13, OUTPUT);
  Wire.setClock(400000);
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(newData);
  Serial.begin(9600);
  while (!Serial) {
  }
  LED.setOutput(LED_PIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(STATUS_LED, DMX[0]);
  cRGB c = { DMX[2], DMX[1], DMX[3] };
  SPEED = DMX[4];
  EYESIZE = DMX[5];

  if (DMX[0] < 25) {
    wash(c);
  } else if (DMX[0] >= 25 && DMX[0] < 51) {
    cylon(c);
  } else if (DMX[0] >= 51 && DMX[0] < 76) {
    staticRainbow(c);
  } else if (DMX[0] >= 76 && DMX[0] < 102) {
    rainbow(c);
  } else if (DMX[0] >= 102 && DMX[0] < 127) {
    rain(c);
  } else if (DMX[0] >= 127 && DMX[0] < 153) {
    sparkle(c);
  } else if (DMX[0] >= 153 && DMX[0] < 178) {
  } else if (DMX[0] >= 178 && DMX[0] < 204) {
  } else if (DMX[0] >= 204 && DMX[0] < 229) {
  } else if (DMX[0] >= 229 && DMX[0] < 255) {
  } // else {error();}

  //  positionIncrement();
  position += 1;
}
