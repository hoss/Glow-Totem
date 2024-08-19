#include <Arduino.h>
#include <Trace.h>
#include <Timer.h>
#include <OneButton.h>

#if defined(USE_TINYUSB)
#include <Adafruit_TinyUSB.h> // for Serial
#endif


// config
const String APP_VERSION = "0.1";     // the version of this app
const String APP_NAME = "Glow Totem"; // the name of this app
const bool WAIT_FOR_SERIAL = true;
const unsigned int SERIAL_BAUDRATE = 115200;


// NEOPIXEL CONFIG
#define LED_PIN   6
#define LED_COUNT 60
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


Trace trace = Trace();
const String DEBUG_RULE = "=====================================================\n";

Timer timer;

// config OLED
const bool USE_DISPLAY = false;
const bool UPSIDE_DOWN_DISPLAY = false;
const byte OLED_RESET = 5;
const unsigned int OLED_STARTUP_DELAY = 1000;

// Onboard Status NeoPixel
const bool USE_ONBOARD_NEOPIXEL = true;
const byte ONBOARD_NEOPIXEL_PIN = 8;
const uint32_t DEFAULT_NEOPIXEL_COLOR = trace.GREEN;
// controlling glow speed from here is not implemented yet
// const unsigned long ONBOARD_NEOPIXEL_SINE_GLOW_SPEED = 500.0; // LOWER numbers for faster glow speed

// PINS
// const byte SENSOR_PIN = A5;
// const byte LIGHT_SWITCH_PIN = A0;
// NeoPixel Pins are defined in their own config section above

// INTERRUPTS
volatile bool interrupted = false;
// int _lightLevel = 0; not yet used

// STATE
const byte OFF_STATE = 0;
const byte ON_STATE = 1;
byte _state = OFF_STATE;
byte _queuedState = OFF_STATE;
bool _builtInLEDisOn = false;

void setup()
{
  trace.initSerial(SERIAL_BAUDRATE, WAIT_FOR_SERIAL);
  if (USE_ONBOARD_NEOPIXEL)
    trace.initNeoPixel(1, ONBOARD_NEOPIXEL_PIN);
  trace.setNeoPixelColor(trace.YELLOW);
  showStartUpMessage();
  initPins();
  initTimers();
  initNeoPixelStrip();
}

void loop()
{
  timer.update();
  trace.loop();
  // loopStrandTest();
}

void initNeoPixelStrip()
{
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(100); // Set BRIGHTNESS to about 1/5 (max = 255)
  fillStrand(strip.Color(255,   0,   128));
  strip.show();            // Turn OFF all pixels ASAP
}

void initTimers()
{
  timer.every(500, toggleBuiltInLED);
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
  trace.trace("Built in LED is " + String(_builtInLEDisOn ? "on." : "off."));
}





/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 ***************        STRANDTEST       ****************
 *
 */

void fillStrand(uint32_t color) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match                          //  Pause for a moment
  }
}

void loopStrandTest() {
  // Fill along the length of the strip in various colors...
  colorWipe(strip.Color(255,   0,   0), 50); // Red
  colorWipe(strip.Color(  0, 255,   0), 50); // Green
  colorWipe(strip.Color(  0,   0, 255), 50); // Blue

  // Do a theater marquee effect in various colors...
  theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
  theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness

  rainbow(10);             // Flowing rainbow cycle along the whole strip
  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
}


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
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
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}






/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 ***************        BOOTSTRAP UTILITIES       ****************
 *
 */

void showStartUpMessage()
{
  delay(300);
  digitalWrite(LED_BUILTIN, HIGH);
  trace.setNeoPixelColor(trace.MAGENTA_DIM);
  trace.trace(APP_NAME);
  delay(900);
  trace.trace("v " + APP_VERSION);
  delay(900);
  digitalWrite(LED_BUILTIN, LOW);
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
