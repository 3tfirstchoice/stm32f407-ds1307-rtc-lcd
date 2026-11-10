#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/*
 * Settings of the clock application. Change behavior here, not in the code.
 */

/* Which application main.c runs.
   APP_MODE_CLOCK:   read the DS1307 and show the date and time.
   APP_MODE_MESSAGE: show a fixed message and do not use the RTC at all. */
#define APP_MODE_CLOCK           0
#define APP_MODE_MESSAGE         1
#define APP_MODE                 APP_MODE_MESSAGE

#define MESSAGE_PAGE_TIME_MS     3000   /* how long each page of the message stays on the LCD */

#define CLOCK_UPDATE_PERIOD_MS   1000   /* how often the clock is read and shown          */
#define BUS_SCAN_SCREEN_TIME_MS  2000   /* how long the I2C scan result stays on the LCD  */
#define MAX_SCANNED_DEVICES      8      /* how many I2C addresses the scan can report     */

/* 1: always set the RTC to the build time at start-up.
   0: set it only when the clock is halted, which happens after a power loss.
   The build time is the time clock_app.c was compiled, so after changing
   this value or before setting the clock use Project > Clean, then Build. */
#define RTC_FORCE_SET_TIME       1

#endif /* APP_CONFIG_H */
