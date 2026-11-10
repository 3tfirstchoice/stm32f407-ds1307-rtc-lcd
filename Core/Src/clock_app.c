#include "clock_app.h"

#include "app_config.h"
#include "clock_ui.h"
#include "datetime.h"
#include "ds1307.h"
#include "i2c_scan.h"
#include "status_led.h"

static ds1307_t rtc;
static uint32_t last_update_ms;

static void show_bus_scan(I2C_HandleTypeDef *hi2c)
{
    uint8_t addresses[MAX_SCANNED_DEVICES];
    uint8_t count = i2c_scan(hi2c, addresses, MAX_SCANNED_DEVICES);

    clock_ui_show_bus_scan(addresses, count);
    HAL_Delay(BUS_SCAN_SCREEN_TIME_MS);
}

/* A DS1307 without battery comes up halted after every power cycle. */
static void set_rtc_to_build_time_if_needed(void)
{
    bool halted = false;
    bool needs_time = RTC_FORCE_SET_TIME;
    datetime_t build_time;

    if (ds1307_is_halted(&rtc, &halted) == HAL_OK && halted) {
        needs_time = true;
    }

    if (needs_time && datetime_from_build(__DATE__, __TIME__, &build_time)) {
        ds1307_set_time(&rtc, &build_time);
    }
}

static void refresh_display(void)
{
    datetime_t now;

    if (ds1307_get_time(&rtc, &now) != HAL_OK) {
        clock_ui_show_rtc_error();
        return;
    }

    clock_ui_show_datetime(&now);
    status_led_toggle();    /* the green LED blinks once per update */
}

void clock_app_init(I2C_HandleTypeDef *hi2c)
{
    clock_ui_init();
    ds1307_init(&rtc, hi2c);

    show_bus_scan(hi2c);
    set_rtc_to_build_time_if_needed();

    last_update_ms = HAL_GetTick() - CLOCK_UPDATE_PERIOD_MS;    /* update right away */
}

void clock_app_run(void)
{
    if (HAL_GetTick() - last_update_ms < CLOCK_UPDATE_PERIOD_MS) {
        return;
    }
    last_update_ms = HAL_GetTick();
    refresh_display();
}
