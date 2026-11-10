#ifndef I2C_SCAN_H
#define I2C_SCAN_H

/*
 * Layer 1 - bus utility: find which devices answer on an I2C bus.
 */

#include "main.h"

/* Probe every 7-bit address and store the ones that answer with an ACK.
   Returns how many addresses were stored, at most max_count. */
uint8_t i2c_scan(I2C_HandleTypeDef *hi2c, uint8_t *addresses, uint8_t max_count);

#endif /* I2C_SCAN_H */
