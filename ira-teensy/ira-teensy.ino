 #include "ira.h"
 #include "fx.h"


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, HIGH);
  pSerial.begin(BAUD);
  pSerial.setPacketHandler(&onSerial);
  analogWriteResolution(8);
  FastLED.addLeds<NUM_STRIPS, WS2812B, LED_PIN, GRB>(LED, LED_COUNT);


}

void loop() {
   pSerial.update();
   if (NEWS) {
    fx = DMX[0];
    color = CRGB( DMX[1], DMX[2], DMX[3] );
    SPEED = DMX[4];
    HUE_SPEED = DMX[5];
    EYESIZE = DMX[6];
    ARG = DMX[7];
    
    NEWS = false;
   }
   analogWrite(STATUS_LED, fx);
   if (fx < 25) {
    wash();
   } else if (fx >= 25 && fx < 51) {
    newCylon();
    } else if (fx >= 51 && fx < 76) {
    marquee();
    } else if (fx >= 76 && fx < 102) {
    wipe();
    } else if (fx >= 102 && fx < 127) {
    waves();
    } else if (fx >= 127 && fx < 153) {
    newRainbow();
    } else if (fx >= 153 && fx < 178) {
    glitter();
    } else if (fx >= 178 && fx < 204) {
    chaser();
    } else if (fx >= 204 && fx < 229) {
    hueCycle();
    } else if (fx >= 229) {
//    progress();
    }
  delayMicroseconds(200);
  

  positionIncrement();
  hueIncrement();
}
