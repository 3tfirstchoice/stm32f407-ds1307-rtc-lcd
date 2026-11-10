#ifndef LCD_H
#define LCD_H

#include "main.h"

#define LCD_COLS 16

/* Driver for a character LCD 1602 (HD44780) wired in 4-bit mode.
   Pin assignment is defined at the top of lcd.c. */

void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_print(const char *text);

/* Print text on one row and pad the rest of the row with spaces,
   so no characters of the previous text are left behind. */
void lcd_print_line(uint8_t row, const char *text);

#endif /* LCD_H */
