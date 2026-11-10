#ifndef CLOCK_APP_H
#define CLOCK_APP_H

/*
 * Layer 3 - application: a clock that reads a DS1307 and shows the time on an LCD.
 * main.c calls only these two functions.
 */

#include "main.h"

/* Show the I2C devices found on the bus, then start the RTC if it needs a time.
   Call once, after the peripherals are initialized. */
void clock_app_init(I2C_HandleTypeDef *hi2c);

/* Refresh the display once per CLOCK_UPDATE_PERIOD_MS. Never blocks for long,
   so call it on every pass of the main loop. */
void clock_app_run(void);

#endif /* CLOCK_APP_H */
