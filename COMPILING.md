# compiling rt-thread for at32f435

[![at32f435 board](doc/at32f435-board/picture_small.webp)](https://raw.githubusercontent.com/koendv/at32f435-board/refs/heads/main/doc/at32f435-board/picture.webp)

This page lists the steps needed to compile rt-thread for the at32f435-board.

The RT-Thread operating system has an IDE, [RT-Thread Studio](https://www.rt-thread.io/studio.html),  and a command-line build system, [env](https://github.com/RT-Thread/env). This document uses the command line on linux, but using the IDE on windows works too.

### source tree

Set up the source tree. The patches are explained in `patches/README.md`.

```sh
git clone https://github.com/RT-Thread/rt-thread
cd rt-thread/
# rt-thread version
git checkout ebe2926cd610661e210b70be1a22bac13923f4fb
cd bsp/at32/
git clone https://github.com/koendv/at32f435-board/
cd at32f435-board
pkgs --update
cd packages/LVGL-v9.1.0/
patch -p0 < ../../patches/lvgl-9.1.0-event-double-clicked.patch
patch -p0 < ../../patches/lvgl-9.1.0-rtthread.patch
cd ../micropython-v1.13.0/
patch -p1 <  ../../patches/micropython-v1.13.0.patch
# back to rt-thread top
cd ../../../../..
patch -p1 < bsp/at32/at32f435-board/patches/at32f435_437_can.patch
patch -p1 < bsp/at32/at32f435-board/patches/usb_dc_dwc2.patch
```
### compiling
Set up the [xpack](https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack/releases/tag/v12.3.1-1.2) arm-none-eabi-gcc compiler.
Set up the  [env](https://github.com/RT-Thread/env) build environment.

Compile  rtthread.elf.

```sh
cd rt-thread/bsp/at32/at32f435-board/
scons
```

## installing

The firmware can be installed using device firmware upgrade DFU, openocd or black magic probe.

### DFU

 using [dfu-util](https://dfu-util.sourceforge.net/). To install using dfu-util:

- connect the board to usb
- push reset and boot buttons at the same time
- wait one second
- release the reset button
- wait one second
- release the boot button

At this point, the at32f435 ought to be in DFU mode:
```
$ lsusb
...
Bus 003 Device 012: ID 2e3c:df11 Artery-Tech DFU in FS Mode
```

With the firmware ``rtthread.bin`` in your current directory, execute the following command:

```
dfu-util -a 0 -d 2e3c:df11 --dfuse-address 0x08000000 -D rtthread.bin
...
Downloading element to address = 0x08000000, size = 643844
Erase   	[=========================] 100%       643844 bytes
Erase    done.
Download	[=========================] 100%       643844 bytes
Download done.
File downloaded successfully
```
Push the reset button. The display initializes, and a usb device appears.
```
$ lsusb
Bus 003 Device 035: ID 0ffe:0001 RT-Thread Team. RTT Composite Device
```

Linux users: If dfu-util does not see the at32, execute dfu-util as root with ``sudo`` or set up udev-rules:

```
# Artery AT32 DFU
# Copy this file to /etc/udev/rules.d/99-at32.rules and run: sudo udevadm control -R
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2e3c", ATTRS{idProduct}=="df11", TAG+="uaccess", MODE="0664", GROUP="plugdev"
```

Windows users: If dfu-util does not see the at32, download [zadig](https://zadig.akeo.ie/), install the WinUSB driver and run dfu-util again.

### OpenOCD

Using [openocd](https://github.com/ArteryTek/openocd) from ArteryTek. Connect a CMSIS-DAP probe to the AT32F435 SWD port.  Command line:
```sh
openocd -f interface/cmsis-dap.cfg -f scripts/target/at32f435xx.cfg
```

### Black Magic Probe

Using [black magic probe](https://black-magic.org/index.html).  With the file `rtthread.elf` in your current directory, connect black magic probe to the AT32F435 SWD port. Command line:

```sh
$ arm-none-eabi-gdb -q
(gdb) tar ext /dev/ttyACM0
(gdb) mon swd
Available Targets:
No. Att Driver
 1      AT32F435 M4
(gdb) at 1
(gdb) file rtthread.elf
(gdb) lo
Start address 0x080013f8, load size 1009488
Transfer rate: 24 KB/sec, 975 bytes/write.
(gdb) r
```

# console output

When the board boots, console output is on the serial port. If the board is connected to usb, the console then switches to the usb serial.

rt-thread also supports console output on SEGGER RTT.

_not truncated_
