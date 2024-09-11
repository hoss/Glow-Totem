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
    // AUKEY PB-N36 claims 74Wh but actually provides 56Wh
    static constexpr float BATTERY_CAPACITY = 56000.0; // mWh
    // AUKEY PB-N36 has max output between 2 USB-A of 3.4mA
    static const uint16_t BATTERY_MAX_LOAD_FOR_ALL_PORTS = 3400; // mA
    // The power monitor doesn't track Feather use so leave headroom.
    static const uint16_t BATTERY_LOAD_FOR_FEATHER = 400; // mA
    static constexpr float BATTERY_MAX_LOAD =
        BATTERY_MAX_LOAD_FOR_ALL_PORTS - BATTERY_LOAD_FOR_FEATHER * 1.0; // mA
    static const unsigned int DURATION_BETWEEN_POWER_USE_UPDATES = 2009;

    // BIG RING LED NEOPIXEL STRIP
    static const byte BIG_RING_DEFAULT_BRIGHTNESS = 175; // (max = 255)
    static const unsigned int BIG_RING_LED_COUNT = 144;
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