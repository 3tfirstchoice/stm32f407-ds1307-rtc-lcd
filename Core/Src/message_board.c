#include "message_board.h"

#include <string.h>
#include "lcd.h"

static const message_page_t *board_pages;
static uint8_t board_page_count;
static uint32_t board_page_time_ms;
static uint8_t current_page;
static uint32_t last_flip_ms;

/* Copy text into a LCD_COLS wide line with equal spaces on both sides.
   Text longer than the row is cut. */
static void center_text(const char *text, char *line)
{
    size_t length = strlen(text);
    size_t left;

    if (length > LCD_COLS) {
        length = LCD_COLS;
    }
    left = (LCD_COLS - length) / 2;

    memset(line, ' ', LCD_COLS);
    memcpy(line + left, text, length);
    line[LCD_COLS] = '\0';
}

static void show_page(uint8_t index)
{
    char line[LCD_COLS + 1];

    center_text(board_pages[index].row0, line);
    lcd_print_line(0, line);
    center_text(board_pages[index].row1, line);
    lcd_print_line(1, line);
}

void message_board_init(const message_page_t *pages, uint8_t page_count,
                        uint32_t page_time_ms)
{
    lcd_init();

    board_pages = pages;
    board_page_count = page_count;
    board_page_time_ms = page_time_ms;
    current_page = 0;

    if (board_page_count > 0) {
        show_page(current_page);
    }
    last_flip_ms = HAL_GetTick();
}

void message_board_run(void)
{
    if (board_page_count < 2) {
        return;     /* nothing to flip */
    }
    if (HAL_GetTick() - last_flip_ms < board_page_time_ms) {
        return;
    }

    last_flip_ms = HAL_GetTick();
    current_page = (uint8_t)((current_page + 1) % board_page_count);
    show_page(current_page);
}
