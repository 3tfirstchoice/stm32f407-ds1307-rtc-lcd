#ifndef DATETIME_H
#define DATETIME_H

/*
 * Layer 0 - pure logic, no hardware.
 * A calendar date and time of day, plus the conversions between it and text.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t second;     /* 0..59                    */
    uint8_t minute;     /* 0..59                    */
    uint8_t hour;       /* 0..23, 24-hour format    */
    uint8_t weekday;    /* 1..7, 1 = Monday         */
    uint8_t day;        /* 1..31                    */
    uint8_t month;      /* 1..12                    */
    uint8_t year;       /* 0..99, years since 2000  */
} datetime_t;

/* Build a datetime_t from the compiler strings __DATE__ ("Oct 29 2026") and
   __TIME__ ("21:30:05"). Returns false when the strings cannot be parsed. */
bool datetime_from_build(const char *build_date, const char *build_time, datetime_t *dt);

/* "29/10/2026" needs 11 bytes. */
void datetime_format_date(const datetime_t *dt, char *buffer, size_t size);

/* "21:30:05" needs 9 bytes. */
void datetime_format_time(const datetime_t *dt, char *buffer, size_t size);

#endif /* DATETIME_H */
