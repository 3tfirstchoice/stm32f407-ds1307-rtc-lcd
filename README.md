# STM32F407 DS1307 RTC Clock on 16x2 LCD

Reads the date and time from a DS1307 real-time clock over I2C and shows them on a 16x2 character LCD, using an STM32F407 Discovery board.

## Hardware

- STM32F407G-DISC1 (STM32F407VG, ARM Cortex-M4)
- Tiny RTC module (DS1307, I2C address 0x68)
- 16x2 character LCD (HD44780), 4-bit mode

## Wiring

| Signal | STM32 pin |
|---|---|
| RTC SCL | PB6 (I2C1_SCL) |
| RTC SDA | PB7 (I2C1_SDA) |
| LCD RS | PE7 |
| LCD E | PE8 |
| LCD D4 | PE9 |
| LCD D5 | PE10 |
| LCD D6 | PE11 |
| LCD D7 | PE12 |
| LCD RW | GND |

## How it works

1. At start-up, the board scans the I2C bus and shows the addresses it finds on the LCD.
2. If the DS1307 has stopped (for example after a power loss), it is set to the firmware build time.
3. Every second, the board reads the time and shows it on the LCD:

```
10/11/2026
22:51:07
```

The green LED (LD4) blinks on each update. If the RTC cannot be read, the LCD shows `RTC read error`.

## Code structure

Upper layers call lower layers, never the other way around.

```
main.c          calls clock_app_init() and clock_app_run()
clock_app       start-up order and refresh period
clock_ui        what text goes on each LCD line
lcd             HD44780 driver, 4-bit bus, DWT microsecond delay
ds1307          DS1307 driver: 7 time registers, BCD conversion, clock-halt bit
i2c_scan        finds devices on the I2C bus
status_led      green LED LD4
datetime        date/time type and string formatting (no hardware)
```

Settings such as the refresh period are in `Core/Inc/app_config.h`.

## Build and run

1. Open the project in STM32CubeIDE.
2. Run **Project > Clean**, then **Build Project**.
3. Flash the board with **Run**.

The RTC is set from the time `clock_app.c` was compiled, so always clean before building when you want to set the clock.
