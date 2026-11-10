#ifndef MESSAGE_BOARD_H
#define MESSAGE_BOARD_H

/*
 * Layer 2 - presentation: shows any text on the LCD, one page at a time.
 * A page is two rows of text, each centered on the screen. With more than one
 * page the board flips through them in a loop. It does not read any device.
 */

#include "main.h"

typedef struct {
    const char *row0;
    const char *row1;
} message_page_t;

/* Initialize the LCD and show the first page. The pages array must stay valid
   for the whole run, so pass a static const array. */
void message_board_init(const message_page_t *pages, uint8_t page_count,
                        uint32_t page_time_ms);

/* Flip to the next page when page_time_ms has passed. Never blocks. */
void message_board_run(void);

#endif /* MESSAGE_BOARD_H */
