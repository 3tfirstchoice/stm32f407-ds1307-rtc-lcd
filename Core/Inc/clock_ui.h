#ifndef CLOCK_UI_H
#define CLOCK_UI_H

/*
 * Layer 2 - presentation: what the user sees on the LCD.
 * It decides the text and the rows. It does not read any device.
 */

#include "main.h"
#include "datetime.h"

void clock_ui_init(void);

/* Row 0 "I2C devices:", row 1 the addresses in hex, or "none found". */
void clock_ui_show_bus_scan(const uint8_t *addresses, uint8_t count);

/* Row 0 the date, row 1 the time of day. */
void clock_ui_show_datetime(const datetime_t *dt);

void clock_ui_show_rtc_error(void);

#endif /* CLOCK_UI_H */
