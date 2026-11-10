#include "message_app.h"

#include "app_config.h"
#include "message_board.h"

/* The message is "Be curious. Be brave. Everybody starts somewhere".
   It is 48 characters, too long for one 16x2 screen, so it is split into
   two pages at word boundaries. To show something else, edit this array. */
static const message_page_t message_pages[] = {
    { "Be curious.",      "Be brave."  },
    { "Everybody starts", "somewhere"  },
};

void message_app_init(void)
{
    message_board_init(message_pages,
                       (uint8_t)(sizeof(message_pages) / sizeof(message_pages[0])),
                       MESSAGE_PAGE_TIME_MS);
}

void message_app_run(void)
{
    message_board_run();
}
