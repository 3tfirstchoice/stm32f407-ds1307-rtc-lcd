#include "clock_ui.h"

#include <stdio.h>
#include "lcd.h"

#define ADDRESS_TEXT_WIDTH  3   /* two hex digits and a space */

void clock_ui_init(void)
{
    lcd_init();
}

void clock_ui_show_bus_scan(const uint8_t *addresses, uint8_t count)
{
    char line[LCD_COLS + 1] = {0};
    size_t length = 0;

    for (uint8_t i = 0; i < count && length + ADDRESS_TEXT_WIDTH <= LCD_COLS; i++) {
        length += (size_t)snprintf(line + length, sizeof(line) - length, "%02X ", addresses[i]);
    }

    lcd_clear();
    lcd_print_line(0, "I2C devices:");
    lcd_print_line(1, (length > 0) ? line : "none found");
}

void clock_ui_show_datetime(const datetime_t *dt)
{
    char line[LCD_COLS + 1];

    datetime_format_date(dt, line, sizeof(line));
    lcd_print_line(0, line);
    datetime_format_time(dt, line, sizeof(line));
    lcd_print_line(1, line);
}

void clock_ui_show_rtc_error(void)
{
    lcd_print_line(0, "RTC read error");
    lcd_print_line(1, "check SDA SCL");
}
