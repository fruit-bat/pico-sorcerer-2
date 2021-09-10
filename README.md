# pico-sorcerer-2
Exidy Sorcerer for Pico Pi with HDMI and SD card

## Wiring

|       | SPI0  | GPIO  | Pin   | SPI       | MicroSD 0 | Description            | 
| ----- | ----  | ----- | ---   | --------  | --------- | ---------------------- |
| MISO  | RX    | 4     | 6     | DO        | DO        | Master In, Slave Out   |
| CS0   | CSn   | 5     | 7     | SS or CS  | CS        | Slave (or Chip) Select |
| SCK   | SCK   | 2     | 4     | SCLK      | CLK       | SPI clock              |
| MOSI  | TX    | 3     | 5     | DI        | DI        | Master Out, Slave In   |
| CD    |       | 22    | 29    |           | CD        | Card Detect            |
| GND   |       |       | 18,23 |           | GND       | Ground                 |
| 3v3   |       |       | 36    |           | 3v3       | 3.3 volt power         |

