#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

#include <Arduino.h>

class Const
{
public:
    static const bool WAIT_FOR_SERIAL = true;
    static const unsigned int SERIAL_BAUDRATE = 115200;

    // BIG RING LED NEOPIXEL STRIP
    static const byte BIG_RING_DEFAULT_BRIGHTNESS = 255; // (max = 255)
    static const unsigned int BIG_RING_LED_COUNT = 20;
    static const byte BIG_RING_LED_PIN = 6;

    // config OLED
    static const bool USE_DISPLAY = false;
    static const bool UPSIDE_DOWN_DISPLAY = false;
    static const byte OLED_RESET = 5;
    static const unsigned int OLED_STARTUP_DELAY = 5000;

    // Onboard Status NeoPixel
    static const bool USE_ONBOARD_NEOPIXEL = false;
    static const byte ONBOARD_NEOPIXEL_PIN = 8;
    // controlling glow speed from here is not implemented yet
    // const unsigned long ONBOARD_NEOPIXEL_SINE_GLOW_SPEED = 500.0; // LOWER numbers for faster glow speed
};

#endif