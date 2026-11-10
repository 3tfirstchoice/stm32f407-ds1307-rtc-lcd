#include "ds1307.h"

#define DS1307_I2C_ADDRESS   (0x68 << 1)  /* HAL wants the 7-bit address shifted left by 1 */
#define DS1307_REG_SECONDS   0x00         /* registers 0x00..0x06 hold the time           */
#define DS1307_TIME_BYTES    7
#define DS1307_BIT_CH        0x80         /* bit 7 of seconds: 1 = clock halted           */
#define DS1307_MASK_SECONDS  0x7F         /* seconds without the CH bit                   */
#define DS1307_MASK_HOURS    0x3F         /* hours in 24-hour format                      */
#define DS1307_TIMEOUT_MS    100

/* The chip stores every field as BCD: the high nibble is the tens digit and
   the low nibble is the units digit, so 0x59 means 59. */
static uint8_t bcd_to_dec(uint8_t bcd)
{
    return (uint8_t)((bcd >> 4) * 10 + (bcd & 0x0F));
}

static uint8_t dec_to_bcd(uint8_t dec)
{
    return (uint8_t)(((dec / 10) << 4) | (dec % 10));
}

void ds1307_init(ds1307_t *dev, I2C_HandleTypeDef *hi2c)
{
    dev->hi2c = hi2c;
}

HAL_StatusTypeDef ds1307_is_halted(const ds1307_t *dev, bool *halted)
{
    uint8_t seconds;
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(dev->hi2c, DS1307_I2C_ADDRESS,
            DS1307_REG_SECONDS, I2C_MEMADD_SIZE_8BIT, &seconds, 1, DS1307_TIMEOUT_MS);

    if (status == HAL_OK) {
        *halted = (seconds & DS1307_BIT_CH) != 0;
    }
    return status;
}

HAL_StatusTypeDef ds1307_set_time(const ds1307_t *dev, const datetime_t *dt)
{
    uint8_t raw[DS1307_TIME_BYTES] = {
        dec_to_bcd(dt->second),     /* CH bit stays 0, so the clock runs */
        dec_to_bcd(dt->minute),
        dec_to_bcd(dt->hour),
        dec_to_bcd(dt->weekday),
        dec_to_bcd(dt->day),
        dec_to_bcd(dt->month),
        dec_to_bcd(dt->year),
    };

    return HAL_I2C_Mem_Write(dev->hi2c, DS1307_I2C_ADDRESS, DS1307_REG_SECONDS,
            I2C_MEMADD_SIZE_8BIT, raw, DS1307_TIME_BYTES, DS1307_TIMEOUT_MS);
}

HAL_StatusTypeDef ds1307_get_time(const ds1307_t *dev, datetime_t *dt)
{
    uint8_t raw[DS1307_TIME_BYTES];
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(dev->hi2c, DS1307_I2C_ADDRESS,
            DS1307_REG_SECONDS, I2C_MEMADD_SIZE_8BIT, raw, DS1307_TIME_BYTES,
            DS1307_TIMEOUT_MS);

    if (status != HAL_OK) {
        return status;
    }

    dt->second  = bcd_to_dec(raw[0] & DS1307_MASK_SECONDS);
    dt->minute  = bcd_to_dec(raw[1]);
    dt->hour    = bcd_to_dec(raw[2] & DS1307_MASK_HOURS);
    dt->weekday = bcd_to_dec(raw[3]);
    dt->day     = bcd_to_dec(raw[4]);
    dt->month   = bcd_to_dec(raw[5]);
    dt->year    = bcd_to_dec(raw[6]);
    return HAL_OK;
}
