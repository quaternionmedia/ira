#include <Wire.h>
#define STATUS_LED 13
const byte I2C_ADDRESS = 42;
volatile uint8_t VALUE;
volatile uint8_t DMX[4];

#define USE_HSV
#include <WS2812.h>

#define LED_COUNT 1800
#define LED_PIN 6

WS2812 LED(LED_COUNT);
cRGB black = { 0, 0, 0 };
cRGB last;

#define DELAY 20
#define EYESIZE 10
int position = 0;

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
  for (int i = 1; i <= EYESIZE-2; i++) {
    LED.set_crgb_at((position+i) % LED_COUNT, color);
  }
  LED.set_crgb_at((position + EYESIZE - 1 ) % LED_COUNT, dim);
  LED.set_crgb_at((position - 1) % LED_COUNT, black);

  LED.sync();
  position = (position + 1 ) % LED_COUNT;
  //delay(DELAY);
}

void rainbow(cRGB color) {
  for (int i = 0; i < LED_COUNT; i++) {
    LED.set_crgb_at(i, { byte(255*sin(2*PI*i/LED_COUNT)), byte(255*cos(2*PI*i/LED_COUNT)), byte(255*sin((2*PI*i+128)/LED_COUNT)) });
  }
  LED.sync();
}

void setup() {
  pinMode(13, OUTPUT);
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(newData);
  Serial.begin(9600);
   while (!Serial) {
  }
  LED.setOutput(LED_PIN);

}

void newData(int n) {
  for (int i = 0; i < n; i++) {
    DMX[i] = Wire.read();
    
  }
//  Serial.write(DMX, 4);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(STATUS_LED, DMX[0]);
  cRGB c = { DMX[2], DMX[1], DMX[3] };
  switch (DMX[0]) {
    case 0:
      // pSerial.send()
      wash(c);
      break;
    case 1:
      cylon(c);
      break;
    case 2:
      rainbow(c);
      break;
    default:
//      error();
      break;
  }
  
}
