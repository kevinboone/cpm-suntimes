/*=========================================================================

  dtutil.c

  Date/time functions. See dtutil.h for more information.

  Copyright (c)2023 Kevin Boone, GPLv3.0

=========================================================================*/

#include "dtutil.h"

static int month_days[12] =
      {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static char *mname[12] = 
      {"January","February","March","April","May","June","July",
       "August","September","October","November","December"};

/*=========================================================================

  dt_is_leap_year

=========================================================================*/
BOOL dt_is_leap_year (int year)
  {
  if ((year % 100 != 0 || year % 400 == 0) && (year % 4 == 0)) return TRUE;
  return FALSE;
  }

/*=========================================================================

  dt_get_d_in_m

=========================================================================*/
int dt_get_d_in_m (int year, int month)
  {
  if (month == 2) return (dt_is_leap_year (year) ? 29 : 28);
  return month_days [month - 1];
  }

/*=========================================================================

  dt_get_doy

=========================================================================*/
int dt_get_doy (int year, int month, int day)
  {
  int i, days = 0;
  for (i = 1; i < month; i++)
     days += dt_get_d_in_m (year, i);
  days += day;
  return days;
  }

/*=========================================================================

  dt_mon_name

=========================================================================*/
char *dt_mon_name (int mon)
  {
  if (mon < 1) return "?";
  if (mon > 12) return "?";
  return mname[mon - 1];
  }

