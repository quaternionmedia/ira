#include <Wire.h>
#define LED 13
const byte ADDRESS = 42;
volatile uint8_t VALUE;
volatile uint8_t DMX[4];

void setup() {
  pinMode(13, OUTPUT);
  Wire.begin(42);
  Wire.onReceive(newData);
  Serial.begin(9600);
}

void newData(int n) {
  for (int i = 0; i < n; i++) {
    DMX[i] = Wire.read();
    
  }
//  Serial.write(DMX, 4);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(LED, DMX[0]);
}
