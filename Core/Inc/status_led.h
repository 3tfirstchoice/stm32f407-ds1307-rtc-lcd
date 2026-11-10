#ifndef STATUS_LED_H
#define STATUS_LED_H

/*
 * Layer 1 - the green LED LD4 (PD12) on the Discovery board.
 * The pin comes from the label LD4 in CubeMX.
 */

#include "main.h"

void status_led_toggle(void);

#endif /* STATUS_LED_H */
