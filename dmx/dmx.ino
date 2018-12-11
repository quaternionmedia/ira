#include <Wire.h>
#include <digitalWriteFast.h>

#include <DMXSerial.h>

#define DMX_ADDRESS 100
#define DMX_CHANNELS 6

#define LED 13

const byte I2C_ADDRESS = 42;
//volatile uint8_t VALUE;

uint8_t DMX_PACKET[DMX_CHANNELS] = {0, 1, 2, 3, 4, 5};
uint8_t DMX_LAST[DMX_CHANNELS];

void setup() {
  pinModeFast(13, OUTPUT);

//  Wire.setClock(400000);
  Wire.begin();

  DMXSerial.init(DMXReceiver);
}

void readDMX() {
  for (int i = 0; i < DMX_CHANNELS; i++) {
    DMX_PACKET[i] = DMXSerial.read(i + DMX_ADDRESS);
  }
}

void sendPacket() {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(DMX_PACKET, DMX_CHANNELS);
  while (Wire.endTransmission()) {
  }
}

void loop() {

  //  VALUE += 1;
  //  VALUE %= 256;
  //  DMX_PACKET[0] = VALUE;

  readDMX();
  analogWrite(LED, DMX_PACKET[0]);

  if (memcmp(DMX_PACKET, DMX_LAST, DMX_CHANNELS) != 0) {
    sendPacket();
    memcpy(DMX_LAST, DMX_PACKET, DMX_CHANNELS);
  }

}
