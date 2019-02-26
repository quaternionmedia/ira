# generator

LED signal generator. Needs [Controller](../dmx/dmx.ino) to tell it what to do.
### Dependencies
Install the following  Arduino Libraries:
##### FastLED:
- https://github.com/FastLED/FastLED

##### PacketSerial (optional)
- https://github.com/bakercp/PacketSerial


### Usage
Connect pin 6 (or your PWM pin of choice) to the data in line of the WS28xx lights. Use a 200-300 ohm resistor between the Arduino and the first pixel.

Then, compile and upload [generator.ino](generator.ino)

### Controlling
By default, the Arduino will listen over the `I2C` and `pSerial` interfaces, and can use both simultaneously.

Effects can be called by sending any number of 0-255 values. Default parameters are:
```
DMX[0] = program
DMX[1] = red
DMX[2] = green
DMX[3] = blue
DMX[4] = SPEED
DMX[5] = ARG
```

`ARG` can be any additional parameter needed by your function. For example, in `waves()` and `marquee()`, it sets the number of pixels per period of the effect.

### SPEED
`positionIncrement()` changes every `loop()` based on the incoming value of `SPEED`.
- 0-63 - step backwards more
- 64-127 - step backwards and delay
- 128 - pause (do nothing)
- 129-192 - step forwards and delay
- 193-255 - step forwards more

### FXs
Creating new effects can be as simple as:

```
void newFX(CRGB color) {
  // FX maths here
  for (int i = 0; i < LED_COUNT; i++) {
    // pixel maths here
    LED[i] = color;
  }
  FastLED.show();
}
```

### Timimg
In order to be practical for theatrical performances, effect functions need to stay reactive to incoming messages, and should not take longer than (ideally) a single DMX frame (~33ms). Longer effects (animations) should be executed in short steps, after which `pSerial.update()` will check for new messages, and set the `NEWS` flag until the new instruction can be executed.

For this reason, most effects make use of the position variables `pos` and `lpos` (previous position), which is incremented separately, based on the SPEED input variable. For example:
```
// animates one pixel across the strand

void cycle(CRGB color) {
  LED[pos] = color;
  LED[lpos] = black;
  FastLED.show();
}
```
This function describes a complete animation in terms of `pos`. It completes very quickly, after which the `positionIncrement()` function will change `pos` according to `SPEED`.
