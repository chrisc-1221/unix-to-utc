#include "main.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

utc_t UtcTime;
uint32_t u32UnixTimeStamp;
time_tt stCurrentTime;

uint16_t LeapYearNumber;
uint16_t FlatYearNumber;

int main(void)
{
    stCurrentTime.year = 2025;
    stCurrentTime.month = 4;
    stCurrentTime.date = 3;
    stCurrentTime.hour = 10;
    stCurrentTime.minute = 29;
    stCurrentTime.second = 30;

    u32UnixTimeStamp = utcToUnix(&stCurrentTime);
    printf("unix timestamp: %d \n", u32UnixTimeStamp);

    u32UnixTimeStamp += 3600; //test: set 3600 for 1 hour
	unixToUtc(u32UnixTimeStamp);
    printf("UTC: %d-%d-%d %d:%d:%d \n", UtcTime.year, UtcTime.month, UtcTime.date, UtcTime.hour, UtcTime.minute, UtcTime.second);
    
    //printf("Unix TimeStamp: \n");
    //scanf("%d", &u32UnixTimeStamp);
    //unixToUtc(u32UnixTimeStamp);
    //printf("UTC: %d-%d-%d %d:%d:%d \n", UtcTime.year, UtcTime.month, UtcTime.date, UtcTime.hour, UtcTime.minute, UtcTime.second);
}

/*
=============================================================================
 *  @brief :       UNIX to UTC
 *  @note :
 *  @param :None
 *  @retva l: None
=============================================================================
*/
utc_t unixToUtc(uint32_t u32UnixTimeStamp)
{
	uint8_t flat_year_month_day[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	uint8_t leap_year_month_day[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	int32_t year_temp = 0;
	int32_t day_temp = 0;
	int32_t cnt_temp = 0;
	uint8_t is_leap_or_flat_year;
	uint8_t i;

	cnt_temp = u32UnixTimeStamp;

	while(cnt_temp >= 126230400)
	{
		year_temp ++;
		cnt_temp = cnt_temp - 126230400;
	}

	UtcTime.year = UNIX_TIME_YEAR + (4 * year_temp);

	if(cnt_temp >= 31536000)
	{
		UtcTime.year ++;
		cnt_temp -= 31536000;

		if(cnt_temp >= 31536000)
		{
			UtcTime.year ++;
			cnt_temp -= 31536000;

			if(cnt_temp >= 31622400)
			{
				UtcTime.year ++;
				cnt_temp -= 31622400;

				if(cnt_temp >= 31536000)
				{
					UtcTime.year ++;
					cnt_temp -= 31536000;
				}
			}
		}
	}

	if((((UtcTime.year % 4) == 0) && ((UtcTime.year % 100) != 0)) || ((UtcTime.year % 400) == 0))
	{
		is_leap_or_flat_year = LEAP_YEAR;
	}
	else
	{
		is_leap_or_flat_year = FLAT_YEAR;
	}

	day_temp = cnt_temp / 86400;

	UtcTime.hour = (cnt_temp - day_temp * 86400) / 3600;
	UtcTime.minute = (cnt_temp - day_temp * 86400 - UtcTime.hour * 3600) / 60;
	UtcTime.second = cnt_temp % 60;

	UtcTime.month = 1;
	for(i = 0; i < 12; i ++)
	{
		if(is_leap_or_flat_year == FLAT_YEAR)
		{
			if(day_temp >= flat_year_month_day[i + 1])
			{
				UtcTime.month ++; day_temp -= flat_year_month_day[i + 1];
			}
		}
		else if(is_leap_or_flat_year == LEAP_YEAR)
		{
			if(day_temp >= leap_year_month_day[i + 1])
			{
				UtcTime.month ++;
				day_temp -= leap_year_month_day[i + 1];
			}
		}
	}

	UtcTime.date = day_temp + 1;

	return UtcTime;
}

/*
=============================================================================
 *  @brief :       UTC to UNIX
 *  @note :
 *  @param :None
 *  @retva l: None
=============================================================================
*/
uint32_t utcToUnix(time_tt *time)
{
	uint8_t FlatYearMonthDay[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	uint8_t LeapYearMonthDay[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	uint16_t i;
	uint16_t ThisYear;

	uint16_t LeapYearNumber = 0;
	uint16_t FlatYearNumber = 0;
	uint16_t LastMonDays =  0;

	uint32_t TimeStamp = 0;

	for (i = 1970; i < stCurrentTime.year; i ++)
	{
		if((((i % 4) == 0) && ((i % 100) != 0)) || ((i % 400) == 0))
		{
			LeapYearNumber ++;
		}
		else
		{
			FlatYearNumber ++;
		}
	}
	TimeStamp = LeapYearNumber * 31622400 + FlatYearNumber * 31536000;

	if((((stCurrentTime.year % 4) == 0) && ((stCurrentTime.year % 100) != 0)) || ((stCurrentTime.year % 400) == 0))
	{
		ThisYear = LEAP_YEAR;
	}
	else
	{
		ThisYear = FLAT_YEAR;
	}

	for ( i = 1; i < stCurrentTime.month; i ++)
	{
		if(ThisYear == LEAP_YEAR)
		{
			LastMonDays += LeapYearMonthDay[i];
		}
		else if(ThisYear == FLAT_YEAR)
		{
			LastMonDays += FlatYearMonthDay[i];
		}
	}
	LastMonDays = LastMonDays + stCurrentTime.date - 1;
	TimeStamp += LastMonDays * 86400;

	TimeStamp += stCurrentTime.hour * 3600;
	TimeStamp += stCurrentTime.minute * 60;
	TimeStamp += stCurrentTime.second;

	return TimeStamp;
}