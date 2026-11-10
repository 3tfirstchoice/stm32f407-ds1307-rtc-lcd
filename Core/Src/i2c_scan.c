#include "i2c_scan.h"

#define I2C_SCAN_FIRST_ADDRESS  0x01    /* 0x00 is the general call address */
#define I2C_SCAN_LAST_ADDRESS   0x7F    /* largest 7-bit address            */
#define I2C_SCAN_TRIALS         1
#define I2C_SCAN_TIMEOUT_MS     5

uint8_t i2c_scan(I2C_HandleTypeDef *hi2c, uint8_t *addresses, uint8_t max_count)
{
    uint8_t count = 0;

    for (uint8_t address = I2C_SCAN_FIRST_ADDRESS; address <= I2C_SCAN_LAST_ADDRESS; address++) {
        /* HAL wants the 7-bit address shifted left by 1 */
        if (HAL_I2C_IsDeviceReady(hi2c, (uint16_t)(address << 1),
                I2C_SCAN_TRIALS, I2C_SCAN_TIMEOUT_MS) == HAL_OK && count < max_count) {
            addresses[count++] = address;
        }
    }
    return count;
}
