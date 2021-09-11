# pico-sorcerer-2
Exidy Sorcerer for Pico Pi with HDMI and SD card

<a href="http://oldcomputers.net/sorcerer.html">
<img src="http://oldcomputers.net/pics/sorcerer.jpg" width="400"/>
</a>

## Features
* DVI over HDMI output<br/>
* 4 emulated disk units with read/write to SD card
* 2 eumlated tape units with read from SD card
* USB Keyboard input

## Prototype
<img src="docs/pico_sorcerer_prototype_1.jpg" width="400"/>

## Wiring

|       | SPI0  | GPIO  | Pin   | SPI       | MicroSD 0 | HDMI/DVI  |      Description       | 
| ----- | ----  | ----- | ---   | --------  | --------- | --------- | ---------------------- |
| MISO  | RX    | 4     | 6     | DO        | DO        |           | Master In, Slave Out   |
| CS0   | CSn   | 5     | 7     | SS or CS  | CS        |           | Slave (or Chip) Select |
| SCK   | SCK   | 2     | 4     | SCLK      | CLK       |           | SPI clock              |
| MOSI  | TX    | 3     | 5     | DI        | DI        |           | Master Out, Slave In   |
| CD    |       | 22    | 29    |           | CD        |           | Card Detect            |
| GND   |       |       | 3     |           | GND       |           | Ground                 |
| 3v3   |       |       | 36    |           | 3v3       |           | 3.3 volt power         |
| GND   |       |       | 18,23 |           |           | GND       | Ground                 |
| GP16  |       | 16    | 21    |           |           | TX2+      | Data channel 2+        |
| GP17  |       | 17    | 22    |           |           | TX2-      | Data channel 2-        |
| GP18  |       | 18    | 24    |           |           | TX1+      | Data channel 1+        |
| GP19  |       | 19    | 25    |           |           | TX1-      | Data channel 1+        |
| GP12  |       | 12    | 16    |           |           | TX0+      | Data channel 0+        |
| GP13  |       | 13    | 17    |           |           | TX0-      | Data channel 0+        |
| GP14  |       | 14    | 19    |           |           | TXC+      | Clock +                |
| GP15  |       | 15    | 20    |           |           | TXC-      | Clock -                |

![image](https://www.raspberrypi.org/documentation/rp2040/getting-started/static/64b50c4316a7aefef66290dcdecda8be/Pico-R3-SDK11-Pinout.svg "Pinout")

## Components 
<a href="https://buyzero.de/products/raspberry-pi-pico-dvi-sock-videoausgabe-fur-den-pico">
<img src="https://cdn.shopify.com/s/files/1/1560/1473/products/Raspberry-Pi-Pico-Video-Output-DVI-Sock-topview_1200x.jpg" width="300"/>
</a>

<a href="https://thepihut.com/products/adafruit-dvi-breakout-board-for-hdmi-source-devices">
<img src="https://cdn.shopify.com/s/files/1/0176/3274/products/67e1a982-7668-48e2-acaf-df9de6f6e6fe_600x.jpg" width="300"/>
</a>

<a href="https://thepihut.com/products/adafruit-micro-sd-spi-or-sdio-card-breakout-board-3v-only">
<img src="https://cdn.shopify.com/s/files/1/0176/3274/products/4682-01_dcdcf68d-19aa-4deb-b758-471e8e7baf62_600x.jpg" width="300"/>
</a>
  
## Resources
  [Exidy Sorcerer Software Library](https://www.classic-computers.org.nz/blog/2017-01-23-software-for-real-sorcerers.htm)<br/>
  [Trailing Edge - Exidy Sorcerer monitor commands](http://www.trailingedge.com/exidy/exidymon.html)<br/>
  [Wren's Amazing PicoDVI](https://github.com/Wren6991/PicoDVI)<br/>
  [CarlK's Super no OS FAT FS for Pico](https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico)<br/>
  [Z88dk example for Sorcerer](https://github.com/z88dk/z88dk/wiki/Platform---Sorcerer)<br/>
  [FAT FS Documentation](http://www.elm-chan.org/fsw/ff/00index_e.html)<br/>
  [USB HID Keycodes](https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2)<br/>

  
  
