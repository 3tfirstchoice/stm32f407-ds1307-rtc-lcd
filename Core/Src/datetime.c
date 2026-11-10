#include "datetime.h"

#include <stdio.h>
#include <string.h>

#define MONTH_NAME_LENGTH   3
#define YEAR_BASE           2000

/* Weekday for a date, 1 = Monday ... 7 = Sunday (Sakamoto's method). */
static uint8_t weekday_of(uint16_t year, uint8_t month, uint8_t day)
{
    static const uint8_t month_offset[12] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    uint8_t weekday;

    if (month < 3) {
        year--;
    }
    weekday = (uint8_t)((year + year / 4 - year / 100 + year / 400 + month_offset[month - 1] + day) % 7);
    return (weekday == 0) ? 7 : weekday;    /* the formula gives 0 for Sunday */
}

bool datetime_from_build(const char *build_date, const char *build_time, datetime_t *dt)
{
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    char month_name[MONTH_NAME_LENGTH + 1] = {0};
    int day, year, hour, minute, second;
    const char *month_position;

    if (sscanf(build_date, "%3s %d %d", month_name, &day, &year) != 3 ||
        sscanf(build_time, "%d:%d:%d", &hour, &minute, &second) != 3) {
        return false;
    }

    month_position = strstr(month_names, month_name);
    if (month_position == NULL || (month_position - month_names) % MONTH_NAME_LENGTH != 0) {
        return false;
    }

    dt->second  = (uint8_t)second;
    dt->minute  = (uint8_t)minute;
    dt->hour    = (uint8_t)hour;
    dt->day     = (uint8_t)day;
    dt->month   = (uint8_t)((month_position - month_names) / MONTH_NAME_LENGTH + 1);
    dt->year    = (uint8_t)(year - YEAR_BASE);
    dt->weekday = weekday_of((uint16_t)year, dt->month, dt->day);
    return true;
}

void datetime_format_date(const datetime_t *dt, char *buffer, size_t size)
{
    snprintf(buffer, size, "%02u/%02u/%04u",
             (unsigned)dt->day, (unsigned)dt->month, (unsigned)(YEAR_BASE + dt->year));
}

void datetime_format_time(const datetime_t *dt, char *buffer, size_t size)
{
    snprintf(buffer, size, "%02u:%02u:%02u",
             (unsigned)dt->hour, (unsigned)dt->minute, (unsigned)dt->second);
}
