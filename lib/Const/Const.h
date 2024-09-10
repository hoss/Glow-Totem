#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

#include <Arduino.h>

class Const
{
public:
    static const bool WAIT_FOR_SERIAL = false;
    static const unsigned int SERIAL_BAUDRATE = 115200;
    static const byte ONBOARD_NEOPIXEL_DEFAULT_BRIGHTNESS = 20;

    // POWER
    static const unsigned int DURATION_BETWEEN_POWER_USE_UPDATES = 2009;
    static constexpr float BATTERY_CAPACITY = 74000.0;  // mWh
    static constexpr float BATTERY_MAX_LOAD = 2400.0; // mA

    // BIG RING LED NEOPIXEL STRIP
    static const byte BIG_RING_DEFAULT_BRIGHTNESS = 9; // (max = 255)
    static const unsigned int BIG_RING_LED_COUNT = 20;
    static const byte BIG_RING_LED_PIN = 9;

    // config OLED
    static const bool USE_DISPLAY = false;
    static const bool UPSIDE_DOWN_DISPLAY = false;
    static const byte OLED_RESET = 5;
    static const unsigned int OLED_STARTUP_DELAY = 5000;

    // Onboard Status NeoPixel
    static const bool USE_ONBOARD_NEOPIXEL = true;
    static const byte ONBOARD_NEOPIXEL_PIN = 8;
    // controlling glow speed from here is not implemented yet
    // const unsigned long ONBOARD_NEOPIXEL_SINE_GLOW_SPEED = 500.0; // LOWER numbers for faster glow speed
};

#endif