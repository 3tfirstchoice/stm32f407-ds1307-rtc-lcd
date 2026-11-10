#ifndef MESSAGE_APP_H
#define MESSAGE_APP_H

/*
 * Layer 3 - application: shows a fixed message on the LCD and never touches
 * the I2C bus or the RTC. main.c calls only these two functions.
 */

void message_app_init(void);

/* Call on every pass of the main loop. */
void message_app_run(void);

#endif /* MESSAGE_APP_H */
