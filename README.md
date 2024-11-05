# at32f435-board

[![at32f435 board](doc/at32f435-board/picture_small.webp)](doc/at32f435-board/picture.webp)

This git is the firmware for an arm  development board. The [hardware project](https://oshwlab.com/koendv/at32f435-board) is at oshwlab.

## goal

A small handheld device, with enough free flash, ram, and board space for my projects.

## hardware

- Artery (雅特力) [AT32F435RGT7](https://www.arterychip.com/en/product/AT32F435.jsp), 1024 kbyte flash, 512 kbyte ram, 288 MHz.
- the first 256kbytes flash is zero wait state.
- 280x240 LCD display with capacitive touch
- ambient light sensor to set LCD brightness
- SD card.
- 16 MByte external SPI flash.
- EEPROM for storing settings.
- battery backup for the real-time clock.
- One full-speed USB, 12 MBit/s.
- CAN bus, 1 Mbit/s.
- JST connectors for GPIO, I2C, SPI and CAN.
- between processor and JST connectors there is room to add electronics for a project.

The cable for the connectors is a "JST SH 1.0mm to Dupont".

[easyeda](https://easyeda.com/) was used to draw the schematic and the pcb. [jlcpcb](https://jlcpcb.com/) assembled the board and 3D-printed the enclosure.

## display

The capacitive touch screen is from a smartwatch, and uses [LVGL](https://lvgl.io/) graphics. The gesture decoding is done in the touch screen itself. The interface is limited to clicks and swipes, but has the advantage of being very light on the cpu. A click or swipe is only a single interrupt. There is no touchscreen polling, no gesture decoding in software.

## compiling

Notes about [compiling the firmware](COMPILING.md).

## ordering

Notes about [ordering assembled pcb's](ORDERING.md) and 3d-printed enclosure from jlcpcb.

## changelog
[Changelog](CHANGELOG.md) where I keep track of progress.

## links

- [P169H002](https://aliexpress.com/wholesale?SearchText=P169H002&sortType=total_tranpro_desc) capacitive touch screen
- [easyeda](https://easyeda.com/) CAD software
- [LCSC](https://www.lcsc.com/) and [SZLCSC](https://www.szlcsc.com/) electronics components
- [jlcpcb](https://jlcpcb.com/) pcb assembly and 3D printing
- [oshwlab](https://oshwlab.com) and [oshwhub](https://oshwhub.com/) open hardware projects
