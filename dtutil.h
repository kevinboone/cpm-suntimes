/*=========================================================================

  dtutil.h

  Various date/time functions

  Copyright (c)2023 Kevin Boone, GPLv3.0

=========================================================================*/
#ifndef __DTUTIL_H
#define __DTUTIL_H

#include "defs.h"

/** Get the day of year, where '1' is January first. This function needs
    to know the year, because leap years have different numbers of days
    in February. */
int dt_get_doy (int year, int month, int day);

/** Get the number of days in the specified month, where January=1. */
int dt_get_d_in_m (int year, int month);

/** Returns TRUE if the specified year is a leap year. */
BOOL dt_is_leap_year (int year);

/** Returns a constant pointer to an English version of the month name.
    If mon < 1 or mon > 12, returns a pointer to '?' */
char *dt_mon_name (int mon);

#endif

