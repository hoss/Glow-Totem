#include <Arduino.h>
#include <Trace.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include "Adafruit_miniTFTWing.h"
#include <Const.h>

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

class MiniTFT
{
    Adafruit_ST7789 tft_7789 = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
    Adafruit_ST7735 tft_7735 = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
    Adafruit_ST77xx *tft = NULL; // we'll assign it later
    uint32_t version;
    uint32_t _timeOfLastUpdate = 0;
    bool _active = false;
    String _currentMessage = "nsy";
    uint32_t _displayBackgroundColor = ST7735_MAGENTA;

public:
    void init();
    String getDisplayTxt();
    void loop();
    bool isActive();
    void initDisplay(bool upsideDown, unsigned int startupDelay, byte oledReset);
    void displayNewMessage(String msg);
    void clearScreen();
    void clearScreen(uint16_t color);
    void drawProgressRect(float percWidth, float percHeight, uint16_t color);

private:
    void updateDisplay(String msg);
    Trace *_trace = Trace::getInstance();
    void trace(String message);
};

bool MiniTFT::isActive()
{
    return _active;
}

void MiniTFT::drawProgressRect(float percWidth, float percHeight, uint16_t color)
{
    int16_t w = tft->width();
    int16_t h = tft->height();
    int16_t width = int16_t(percWidth * w);
    int16_t height = int16_t(percHeight * h);
    // tft->fillRect(0, 0, width, height, color);
    tft->fillRect(w - width, 0, width, height, color);
}

void MiniTFT::displayNewMessage(String msg)
{
    uint16_t color = ST77XX_WHITE;
    tft->fillScreen(_displayBackgroundColor);
    tft->setCursor(0, 0);
    tft->setTextColor(color);
    tft->setTextSize(2);
    tft->setTextWrap(true);
    tft->print(msg);
    _currentMessage = msg;
}

void MiniTFT::clearScreen()
{
    clearScreen(ST77XX_BLACK);
}
void MiniTFT::clearScreen(uint16_t color)
{
    tft->fillScreen(color);
    _displayBackgroundColor = color;
}

void MiniTFT::updateDisplay(String msg)
{
    // STUB
}

String MiniTFT::getDisplayTxt()
{
    return _currentMessage;
}

void MiniTFT::init()
{
    _trace->initSerial(Const::SERIAL_BAUDRATE, Const::WAIT_FOR_SERIAL);
    if (!ss.begin())
    {
        trace("   *****   seesaw couldn't be found!");
        return;
    }
    trace("seesaw found!");

    version = ((ss.getVersion() >> 16) & 0xFFFF);
    trace("SeeSaw Version: " + String(version));

    ss.tftReset();                         // reset the display
    ss.setBacklight(TFTWING_BACKLIGHT_ON); // turn off the backlight

    if (version == 3322)
    {
        trace("Version 2 TFT FeatherWing found");
        tft_7789.init(135, 240);
        tft = &tft_7789;
    }
    else
    {
        // Version 1 TFT FeatherWing THAT I'M USING
        trace("Version 1 TFT FeatherWing found");
        tft_7735.initR(INITR_MINI160x80); // initialize a ST7735S chip, mini display
        tft = &tft_7735;
    }
    tft->setRotation(3);
    Serial.println("TFT initialized");

    tft->fillScreen(ST77XX_RED);
    delay(100);
    tft->fillScreen(ST77XX_GREEN);
    delay(100);
    tft->fillScreen(ST77XX_BLUE);
    delay(100);
    tft->fillScreen(ST77XX_BLACK);
}

void MiniTFT::loop()
{
    return;
    uint32_t buttons = ss.readButtons();
    // Serial.println(buttons, BIN);

    uint16_t color;

    color = ST77XX_BLACK;
    if (!(buttons & TFTWING_BUTTON_LEFT))
    {
        Serial.println("LEFT");
        color = ST77XX_WHITE;
    }
    if (version == 3322)
    {
        tft->fillTriangle(200, 45, 200, 85, 220, 65, color);
    }
    else
    {
        tft->fillTriangle(150, 30, 150, 50, 160, 40, color);
    }

    color = ST77XX_BLACK;
    if (!(buttons & TFTWING_BUTTON_RIGHT))
    {
        Serial.println("RIGHT");
        color = ST77XX_WHITE;
    }
    if (version == 3322)
    {
        tft->fillTriangle(120, 45, 120, 85, 100, 65, color);
    }
    else
    {
        tft->fillTriangle(120, 30, 120, 50, 110, 40, color);
    }

    color = ST77XX_BLACK;
    if (!(buttons & TFTWING_BUTTON_DOWN))
    {
        Serial.println("DOWN");
        color = ST77XX_WHITE;
    }
    if (version == 3322)
    {
        tft->fillTriangle(140, 25, 180, 25, 160, 10, color);
    }
    else
    {
        tft->fillTriangle(125, 26, 145, 26, 135, 16, color);
    }

    color = ST77XX_BLACK;
    if (!(buttons & TFTWING_BUTTON_UP))
    {
        Serial.println("UP");
        color = ST77XX_WHITE;
    }
    if (version == 3322)
    {
        tft->fillTriangle(140, 100, 180, 100, 160, 120, color);
    }
    else
    {
        tft->fillTriangle(125, 53, 145, 53, 135, 63, color);
    }

    color = ST77XX_BLACK;
    if (!(buttons & TFTWING_BUTTON_A))
    {
        Serial.println("A");
        color = ST7735_GREEN;
    }
    if (version == 3322)
    {
        tft->fillCircle(40, 100, 20, color);
    }
    else
    {
        tft->fillCircle(30, 57, 10, color);
    }

    color = ST77XX_BLACK;
    if (!(buttons & TFTWING_BUTTON_B))
    {
        Serial.println("B");
        color = ST77XX_YELLOW;
    }
    if (version == 3322)
    {
        tft->fillCircle(40, 30, 20, color);
    }
    else
    {
        tft->fillCircle(30, 18, 10, color);
    }

    color = ST77XX_BLACK;
    if (!(buttons & TFTWING_BUTTON_SELECT))
    {
        Serial.println("SELECT");
        color = ST77XX_RED;
    }
    if (version == 3322)
    {
        tft->fillCircle(160, 65, 20, color);
    }
    else
    {
        tft->fillCircle(80, 40, 7, color);
    }
}

void MiniTFT::trace(String message)
{
    _trace->trace("MiniTFT :: " + message);
}