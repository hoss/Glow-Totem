# Glow Totem

An art project to create a colourful light totem that reacts to its environment while creating a recognizable location marker at events.

## Use

Open with PlatformIO inside VS Code and bring the magic. Assumes you are using the following hardware (but can easily be adapted to other similar hardware):

* [Adafruit Feather nRF52840 Sense][1] Edit platform.ini if you want to use a different board.
* [NeoPixel LEDs][2] I'm using a combination of a strip and a ring.
* [INA219 Current Sensor][3] Optional, but useful if like me you're running off battery power and want to see how much power the lighting is using.
* [Adafruit Mini TFT with Joystick Featherwing][4] Optional, but I like a UI to my electronics projects, and the ability to tweak config at runtime.
* [Crave PowerPack 2][5] 185Wh, but weighs 4.15 pounds, so limits portability. Has 2 USB-C PD ports so I can grab a wide range of voltages for increased flexiblity.
* [USB-C to 12V Barrel Jack Cable][6] A really simple way to get 12V from a USB-C PD device. I'm using it to power a 12V NeoPixel strip (note that most NeoPixels are 5V, [the strip I'm using][7] is an outlier).




## Authors
* **[Hoss Gifford][8]** - *Initial work*

## Credits
The giants whose shoulders I stand atop.

* **[Adafruit][9]** - *Many of the code libraries and much of the hardware I use is created by these lovely peeps.*


## License
[Apache License 2.0][10]

[1]:	https://learn.adafruit.com/adafruit-feather-sense?view=all
[2]:	https://learn.adafruit.com/adafruit-neopixel-uberguide?view=all
[3]:	https://learn.adafruit.com/adafruit-ina219-current-sensor-breakout
[4]:	https://learn.adafruit.com/adafruit-mini-tft-featherwing?view=all
[5]:	https://cravedirect.com/products/crave-powerpack-2-50000-mah-dual-usb-qc3-0-dual-power-delivery-charger
[6]:	https://www.adafruit.com/product/5450
[7]:	https://www.adafruit.com/product/3869
[8]:	https://hossgifford.com
[9]:	https://adafruit.com/
[10]:	https://www.apache.org/licenses/LICENSE-2.0

