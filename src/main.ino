#include <Arduino.h>
#include <Wire.h>
#include <Trace.h>
#include <Timer.h>
#include <OneButton.h>
#include <Const.h>

// #define NEOPIXEL_STRIPS_DISABLED
// #define POWER_MONITOR_DISABLED
// #define MINI_TFT_DISABLED

#ifndef MINI_TFT_DISABLED
#include <MiniTFT.cpp>
MiniTFT _miniTFT{};
#endif

#ifndef POWER_MONITOR_DISABLED
#include <PowerMonitor.cpp>
PowerMonitor _powerMonitor{};
#endif

#if defined(USE_TINYUSB)
#include <Adafruit_TinyUSB.h> // for Serial
#endif

// config
const String APP_VERSION = "0.8";     // the version of this app
const String APP_NAME = "Glow Totem"; // the name of this app

#ifndef NEOPIXEL_STRIPS_DISABLED
// NEOPIXEL CONFIG
Adafruit_NeoPixel strip(Const::BIG_RING_LED_COUNT, Const::BIG_RING_LED_PIN, NEO_GRB + NEO_KHZ800);
#endif

Trace *_trace = Trace::getInstance();
const String DEBUG_RULE = "=====================================================\n";

Timer timer;

// INTERRUPTS
volatile bool interrupted = false;

// STATE
const byte OFF_STATE = 0;
const byte ON_STATE = 1;
byte _state = OFF_STATE;
byte _queuedState = OFF_STATE;
bool _builtInLEDisOn = false;

/*
 *
 *
 *
 *
 *
 *

 ad88888ba                        88        88
d8"     "8b                ,d     88        88
Y8,                        88     88        88
`Y8aaaaa,     ,adPPYba,  MM88MMM  88        88  8b,dPPYba,
  `"""""8b,  a8P_____88    88     88        88  88P'    "8a
        `8b  8PP"""""""    88     88        88  88       d8
Y8a     a8P  "8b,   ,aa    88,    Y8a.    .a8P  88b,   ,a8"
 "Y88888P"    `"Ybbd8"'    "Y888   `"Y8888Y"'   88`YbbdP"'
                                                88
                                                88
 *
 ***************        SETUP       ****************
 *
 */

void setup()
{
  initMiniTFT();
  _trace->initSerial(Const::SERIAL_BAUDRATE, Const::WAIT_FOR_SERIAL);
  if (Const::USE_ONBOARD_NEOPIXEL)
  {
    _trace->initNeoPixel(1, Const::ONBOARD_NEOPIXEL_PIN);
    _trace->setNeoPixelColor(_trace->YELLOW);
  }
  if (Const::USE_DISPLAY)
    _trace->initDisplay(Const::UPSIDE_DOWN_DISPLAY, Const::OLED_STARTUP_DELAY, Const::OLED_RESET);
  long randomValue = analogRead(A1);
  trace("randomValue" + String(randomValue), true);
  randomSeed(randomValue);
  showStartUpMessage();
  initPins();
  initTimers();
  initBigRingNeoPixelStrip();

#ifndef POWER_MONITOR_DISABLED
  _powerMonitor.init();
  // displayPowerUse();
#endif
}

void initMiniTFT()
{
#ifndef MINI_TFT_DISABLED
  _miniTFT.init();
  trace(_miniTFT.getDisplayTxt());
#endif
}

void loop()
{
  timer.update();
  _trace->loop();
#ifndef MINI_TFT_DISABLED
  _miniTFT.loop();
#endif
  // loopStrandTest();
}

void initBigRingNeoPixelStrip()
{
#ifndef NEOPIXEL_STRIPS_DISABLED
  strip.begin();
  strip.setBrightness(Const::BIG_RING_DEFAULT_BRIGHTNESS);
  fillBigRingNewPixelStripWithSingleRandomColor();
#endif
}

void fillBigRingNewPixelStripWithSingleRandomColor()
{
#ifndef NEOPIXEL_STRIPS_DISABLED
  byte r = random(255);
  byte g = random(255);
  byte b = random(255);
  trace(String("r:" + String(r) + " g:" + String(g) + " b:" + String(b)));
  // _trace->setNeoPixelColor(r, g, b);
  fillStrand(strip.Color(r, g, b));
#endif
}

void initTimers()
{
  timer.every(500, toggleBuiltInLED);
#ifndef NEOPIXEL_STRIPS_DISABLED
  timer.every(2000, fillBigRingNewPixelStripWithSingleRandomColor);
#endif
#ifndef POWER_MONITOR_DISABLED
  timer.every(Const::DURATION_BETWEEN_POWER_USE_UPDATES, displayPowerUse);
#endif
}

void displayPowerUse()
{
#ifndef POWER_MONITOR_DISABLED
  _trace->setNeoPixelColor(_trace->ORANGE, 255);
  trace(_powerMonitor.getPowerUse(), true);
  _trace->setNeoPixelColor(_trace->GREEN);
#endif
}

void toggleBuiltInLED()
{
  if (_builtInLEDisOn)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  _builtInLEDisOn = !_builtInLEDisOn;
  // trace("_builtInLEDisOn=" + String(_builtInLEDisOn), true);
}

/*
 *
 *
 *
 *

 ad88888ba                                                        88  888888888888
d8"     "8b  ,d                                                   88       88                           ,d
Y8,          88                                                   88       88                           88
`Y8aaaaa,  MM88MMM  8b,dPPYba,  ,adPPYYba,  8b,dPPYba,    ,adPPYb,88       88   ,adPPYba,  ,adPPYba,  MM88MMM
  `"""""8b,  88     88P'   "Y8  ""     `Y8  88P'   `"8a  a8"    `Y88       88  a8P_____88  I8[    ""    88
        `8b  88     88          ,adPPPPP88  88       88  8b       88       88  8PP"""""""   `"Y8ba,     88
Y8a     a8P  88,    88          88,    ,88  88       88  "8a,   ,d88       88  "8b,   ,aa  aa    ]8I    88,
 "Y88888P"   "Y888  88          `"8bbdP"Y8  88       88   `"8bbdP"Y8       88   `"Ybbd8"'  `"YbbdP"'    "Y888


 ***************        STRANDTEST       ****************
 *
 */

#ifndef NEOPIXEL_STRIPS_DISABLED

void fillStrand(uint32_t color)
{
  for (int i = 0; i < strip.numPixels(); i++)
  {                                // For each pixel in strip...
    strip.setPixelColor(i, color); //  Set pixel's color (in RAM)
    strip.show();                  //  Update strip to match
    // trace(String("filled NeoPixel #" + String(i) + " with " + String(color)));
    delay(51); //  Pause for a moment
  }
}

void loopStrandTest()
{
  // Fill along the length of the strip in various colors...
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue

  // Do a theater marquee effect in various colors...
  theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
  theaterChase(strip.Color(127, 0, 0), 50);     // Red, half brightness
  theaterChase(strip.Color(0, 0, 127), 50);     // Blue, half brightness

  rainbow(10);             // Flowing rainbow cycle along the whole strip
  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
}

// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait)
{
  for (int i = 0; i < strip.numPixels(); i++)
  {                                // For each pixel in strip...
    strip.setPixelColor(i, color); //  Set pixel's color (in RAM)
    strip.show();                  //  Update strip to match
    delay(wait);                   //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait)
{
  for (int a = 0; a < 10; a++)
  { // Repeat 10 times...
    for (int b = 0; b < 3; b++)
    {                //  'b' counts from 0 to 2...
      strip.clear(); //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3)
      {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait)
{
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256)
  {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait)
{
  int firstPixelHue = 0; // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++)
  { // Repeat 30 times...
    for (int b = 0; b < 3; b++)
    {                //  'b' counts from 0 to 2...
      strip.clear(); //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for (int c = b; c < strip.numPixels(); c += 3)
      {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int hue = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color);                       // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
#else
//  NEOPIXEL STRANDS ARE DISBLED
#endif

/*
 *
 *
 *

88        88           88  88
88        88    ,d     ""  88
88        88    88         88
88        88  MM88MMM  88  88  ,adPPYba,
88        88    88     88  88  I8[    ""
88        88    88     88  88   `"Y8ba,
Y8a.    .a8P    88,    88  88  aa    ]8I
 `"Y8888Y"'     "Y888  88  88  `"YbbdP"'

 *
 *
 *
 ***************        BOOTSTRAP UTILITIES       ****************
 *
 */

void trace(String message)
{
  trace(message, false);
}

void trace(String message, bool showOnDisplay)
{
  _trace->trace(message);
#ifndef MINI_TFT_DISABLED
  if (showOnDisplay)
    _miniTFT.displayNewMessage(message);
#endif
}

void showStartUpMessage()
{
  delay(300);
  digitalWrite(LED_BUILTIN, HIGH);
  _trace->setNeoPixelColor(_trace->MAGENTA_DIM);
  String msg = APP_NAME;
  trace(msg, true);
  delay(900);
  msg += ("\nv" + APP_VERSION);
  trace(msg, true);
  delay(900);
  digitalWrite(LED_BUILTIN, LOW);
  _miniTFT.clearScreen();
}

void initButtons()
{
  // // link the button 1 functions.
  // buttonA.attachClick(click1);
  // buttonA.attachDoubleClick(doubleclick1);
  // buttonA.attachLongPressStart(longPressStart1);
  // buttonA.attachLongPressStop(longPressStop1);
  // buttonA.attachDuringLongPress(longPress1);
}

void initPins()
{
  // setLowOutput(LIGHT_SWITCH_PIN);
  setHighOutput(LED_BUILTIN);
}

void setPulledUpInput(byte pin)
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void setHighOutput(byte pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}

void setLowOutput(byte pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}
