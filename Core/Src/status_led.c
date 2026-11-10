#include "status_led.h"

void status_led_toggle(void)
{
    HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
}
