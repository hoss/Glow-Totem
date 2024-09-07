#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include "Adafruit_miniTFTWing.h"

class UI
{
  byte _state = 3;

  Adafruit_miniTFTWing ss;
#define TFT_RST -1 // we use the seesaw for resetting to save a pin

#ifdef ESP8266
#define TFT_CS 0
#define TFT_DC 15
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
#define TFT_CS 15
#define TFT_DC 33
#elif defined(TEENSYDUINO)
#define TFT_DC 10
#define TFT_CS 4
#elif defined(ARDUINO_STM32_FEATHER)
#define TFT_DC PB4
#define TFT_CS PA15
#elif defined(ARDUINO_NRF52832_FEATHER) /* BSP 0.6.5 and higher! */
#define TFT_DC 11
#define TFT_CS 31
#elif defined(ARDUINO_MAX32620FTHR) || defined(ARDUINO_MAX32630FTHR)
#define TFT_DC P5_4
#define TFT_CS P5_3
#else
  // Anything else, defaults!
#define TFT_CS 5
#define TFT_DC 6
#endif

  Adafruit_ST7789 tft_7789 = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
  Adafruit_ST7735 tft_7735 = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

  // we'll assign it later
  Adafruit_ST77xx *tft = NULL;
  uint32_t version;

public:
  void setState(byte newState)
  {
    _state = newState;
  }
  byte getState()
  {
    return _state;
  }



  void init()
  {
    // if (!ss.begin())
    // {
    //   Serial.println("seesaw couldn't be found!");
    //   while (1)
    //     ;
    // }

    // version = ((ss.getVersion() >> 16) & 0xFFFF);
    // Serial.print("Version: ");
    // Serial.println(version);
    // if (version == 3322)
    // {
    //   Serial.println("Version 2 TFT FeatherWing found");
    // }
    // else
    // {
    //   Serial.println("Version 1 TFT FeatherWing found");
    // }

    // ss.tftReset();                         // reset the display
    // ss.setBacklight(TFTWING_BACKLIGHT_ON); // turn off the backlight

    // if (version == 3322)
    // {
    //   tft_7789.init(135, 240);
    //   tft = &tft_7789;
    // }
    // else
    // {
    //   tft_7735.initR(INITR_MINI160x80); // initialize a ST7735S chip, mini display
    //   tft = &tft_7735;
    // }
    // tft->setRotation(1);
    // Serial.println("TFT initialized");

    // tft->fillScreen(ST77XX_RED);
    // delay(100);
    // tft->fillScreen(ST77XX_GREEN);
    // delay(100);
    // tft->fillScreen(ST77XX_BLUE);
    // delay(100);
    // tft->fillScreen(ST77XX_BLACK);
  }
};
