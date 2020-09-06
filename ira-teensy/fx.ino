
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
