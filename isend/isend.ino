#include <Wire.h>
#include <digitalWriteFast.h>

//#include <PacketSerial.h>
//PacketSerial pSerial;


#include <DMXSerial.h>
#define PACKET_LEN 4
#define DMX_ADDRESS 100

#define LED 13
#define DELAY 0
const byte I2C_ADDRESS = 42;
volatile uint8_t VALUE;

uint8_t DMX_PACKET[4] = {0, 1, 2, 3};
uint8_t DMX_LAST[4];
void setup() {
  pinModeFast(13, OUTPUT);
    Wire.begin();

  DMXSerial.init(DMXReceiver);
}

void readDMX() {
  for (int i = 0; i < PACKET_LEN; i++) {
    DMX_PACKET[i] = DMXSerial.read(i + DMX_ADDRESS);
  }
}

void sendPacket() {

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(DMX_PACKET, 4);
  while (Wire.endTransmission()) {
  }
//  digitalWriteFast(LED, HIGH);
//  delay(DELAY);
//  digitalWriteFast(LED, LOW);
//  delay(DELAY);

}
void loop() {

  VALUE += 1;
  VALUE %= 256;
    readDMX();
//  DMX_PACKET[0] = VALUE;
  analogWrite(LED, DMX_PACKET[0]);

  if (memcmp(DMX_PACKET, DMX_LAST, 4) != 0) {
    sendPacket();
    memcpy(DMX_LAST, DMX_PACKET, 4);
  }

}
