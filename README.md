# Raspberry Pi System Monitor

A standalone system monitor written in C++.  
Very little CPU usage. No external dependencies. No incompatibility with an abandoned python framework or python itself.

## Prerequisites

- Raspberry Pi (tested with [Raspberry Pi 4](https://www.raspberrypi.com/products/raspberry-pi-4-model-b/))
- SSD1306-based OLED module over I2C protocol (tested with only [this 128x64 display](https://www.aliexpress.com/item/4000511247282.html) and [this 128x32 display](https://www.aliexpress.com/item/1005003472541765.html))
- C++11 compiler on Linux

## Usage

1. [Enable I2C on Raspberry Pi](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c)

2. Connect an OLED module (3.3V, GND, SDA, SCL)

3. Modify `config.hpp` if necessary

4. Build
  ```
  $ make        # for 128x64 displays
  $ make lcd=1  # for 128x32 displays
  ```

5. Run
  ```
  $ ./run.sh
  ```

## Screenshots (?)
<img alt="Screenshot 1" src="../assets/1.svg?raw=true" width="320">

## Acknowledges

The source code is roughly based on the following works:

- https://github.com/vadzimyatskevich/SSD1306
- https://stackoverflow.com/a/43598738

## License
Released under the [WTFPL](http://www.wtfpl.net/about/).
