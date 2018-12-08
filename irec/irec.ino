#define DEBUG true

#include <Wire.h>
#define STATUS_LED 13
#define I2C_ADDRESS 42

#define DMX_CHANNELS 6
volatile uint8_t DMX[DMX_CHANNELS];
//uint8_t DMX[DMX_CHANNELS];

//#define USE_HSV
//#include <WS2812.h>

#include <FastLED.h>

#define LED_COUNT 300
#define LED_PIN 6

//WS2812 LED(LED_COUNT);
CRGB LED[LED_COUNT];



CRGB qblue = CRGB( 163, 183, 228 );
CRGB last;

uint8_t SPEED = 127;
uint8_t EYESIZE = 10;
int pos = 0;
int lpos = pos;
int delta = 0;

#define BAR 70

#include <cos_fix.h>

#include <PacketSerial.h>
PacketSerial pSerial;
#define PSERIAL_BAUD 1200

void positionIncrement() {
  lpos = pos;
  if (SPEED < 64) {
    delta = SPEED - 65;
    } else if (SPEED >= 64 && SPEED < 128) {
    delta = -1;
    delay((SPEED - 64) *2 );
  } else if (SPEED == 128) {
    delta = 0;
    //freeze
  } else if (SPEED > 128 && SPEED <= 192) {
    delta = 1;
    delay((192 - SPEED) *2 );
  } else if (SPEED > 192) {
    delta = SPEED - 191;
    }
    pos = (pos + delta + LED_COUNT) % LED_COUNT;
  
}

void wash(CRGB color) {
  for (int i = 0; i < LED_COUNT; i++)
  {
    LED[i] = color;
  }
  FastLED.show();
}

void cylon(CRGB color) {

  for (int i = 0; i <= EYESIZE; i++) {
    LED[(pos + i + LED_COUNT) % LED_COUNT] = color; 
  }
  
  if (delta < 0) {
    for (int i = 0; i > delta; i--) {
      LED[(lpos + EYESIZE + i + LED_COUNT) % LED_COUNT] = CRGB::Black;
    }
  } else {
    for (int i = 0; i < delta; i++) {
      LED[(lpos + i + LED_COUNT) % LED_COUNT] = CRGB::Black;
    }
  }

  FastLED.show();
  }

void staticRainbow(CRGB color) {

  for (int i = 0; i < LED_COUNT; i++) {
    LED[i] = CRGB (byte(color.g * (1 + sin_fix(2 * PI*i / LED_COUNT)) / 2),
                         byte(color.r * (1 + cos_fix(2 * PI*i / LED_COUNT)) / 2),
                         byte(color.b * (1 + sin_fix((2 * PI * i + 128)) / LED_COUNT)) / 2
                       );
  }
  FastLED.show();
}

void rainbow(CRGB color) {
  for (int i = 0; i < LED_COUNT; i++) {
    uint8_t scalar = (1 + sin_fix(2 * PI * i * LED_COUNT)) / 2;
    LED[i] = CRGB(
      (color.g + pos) % 256,
      (color.r + pos) % 256,
      (color.b - pos) % 256
    );
  }
  FastLED.show();

}

void rain(CRGB color) {
  for (int i = 0; i < LED_COUNT; i++) {
    uint16_t scalar = (1 + sin_fix(i*2*PI/LED_COUNT)) / 2;
    LED[(i + pos) % LED_COUNT] =
      CRGB(
        (uint8_t) color.g * scalar,
        (uint8_t) color.r * scalar,
        (uint8_t) color.b * scalar
      );
  }
  FastLED.show();

}

void sparkle(CRGB color) {
  //  int t = millis()/(256 - SPEED);
  for (int i = 0; i < LED_COUNT; i++) {
    float scalar = (1 + sin_fix( 2 * PI * i / LED_COUNT)) / 2;
    LED[i]  = CRGB(
      (uint8_t) color.g * scalar,
      (uint8_t) color.r * scalar,
      (uint8_t) color.b * scalar
    );
  }
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
    LED[i] = CRGB::Black;
  }
  for (int i = BAR; i < 172; i++) {
    LED[i] = qblue;
  }
  for (int i = 172; i < 190; i++) {
    LED[i] = CRGB (map(b, 20, 60, 0 ,127), map(b, 20, 60, 0, 255), map(b, 20,60,255,0) );
  }
  for (int i = 190; i < 208; i++) {
  LED[i] = CRGB ( 0, map(t, 20, 220, 0, 255), map(t, 20,220,255,0) );
  }
  for (int i = 208; i < LED_COUNT; i++) {
    LED[i] = CRGB::Black;
  }
  
  FastLED.show();

}

void newData(int n) {
  for (int i = 0; i < n; i++) {
    DMX[i] = Wire.read();
  }
  //  Serial.write(DMX, DMX_CHANNELS);
}

void onSerial(const uint8_t* buffer, size_t size) {
  memcpy(DMX, buffer, min(size, DMX_CHANNELS));
//  for (int i = 0; i < size; i++) {
//    DMX[i] = buffer[i];
//  }
//  analogWrite(STATUS_LED, buffer[0]);
  if (DEBUG) {
   pSerial.send(buffer, size);
  }

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
  LEDS.addLeds<WS2812,LED_PIN,RGB>(LED,LED_COUNT);

  DMX[1] = 255;
  DMX[2] = 255;
  DMX[3] = 255;
}

void loop() {
  pSerial.update();
  analogWrite(STATUS_LED, DMX[0]);
  CRGB c = { DMX[2], DMX[1], DMX[3] };
  SPEED = DMX[4];
//  EYESIZE = DMX[5];

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
  } else if (DMX[0] >= 229 && DMX[0] <= 255) {
    progress(DMX[1], DMX[2], DMX[3]);
//  // else {error();}
  }
//
    positionIncrement();
//  pos = (pos + 1 ) % LED_COUNT;
}
