#include <string.h>
#include <stdio.h>
#include "stdint.h"

#define UNIX_TIME_YEAR       			1970
#define UNIX_TIME_MONTH   				1
#define UNIX_TIME_DATE  				1
#define UNIX_TIME_HOUR  				0
#define UNIX_TIME_MINUTE  				0
#define UNIX_TIME_SECOND				0

#define LEAP_YEAR						1
#define FLAT_YEAR						0

typedef struct
{
	uint16_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}utc_t;

typedef struct
{
	uint16_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}time_tt;

utc_t unixToUtc(uint32_t u32UnixTimeStamp);
uint32_t utcToUnix(time_tt *time);