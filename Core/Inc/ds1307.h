#ifndef DS1307_H
#define DS1307_H

/*
 * Layer 1 - device driver for the DS1307 real-time clock chip, I2C address 0x68.
 * It knows the register map of the chip and nothing about the application.
 */

#include "main.h"
#include "datetime.h"

typedef struct {
    I2C_HandleTypeDef *hi2c;    /* I2C peripheral the chip is connected to */
} ds1307_t;

void ds1307_init(ds1307_t *dev, I2C_HandleTypeDef *hi2c);

/* The clock is stopped (bit CH of the seconds register is 1) after the chip
   lost all power, for example when the module has no battery. */
HAL_StatusTypeDef ds1307_is_halted(const ds1307_t *dev, bool *halted);

/* Write the time and start the clock. */
HAL_StatusTypeDef ds1307_set_time(const ds1307_t *dev, const datetime_t *dt);

HAL_StatusTypeDef ds1307_get_time(const ds1307_t *dev, datetime_t *dt);

#endif /* DS1307_H */
