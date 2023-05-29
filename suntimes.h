/*=========================================================================

  suntimes 

  suntimes.h

  Copyright (c)2022-3 Kevin Boone, GPLv3.0

=========================================================================*/
#ifndef __SUNTIMES_H
#define __SUNTIMES_H

/* Constants for the various accepted zeniths. The 'default' zenith is
   the one that most people take to represent conventional sunrise/set */
#define DEFAULT_ZENITH (90.0 + 50.0/60.0)
#define CIVIL_TWILIGHT (90.0 + 50.0/60.0 + 6.0)
#define NAUTICAL_TWILIGHT (90.0 + 50.0/60.0 + 12.0)
#define ASTRO_TWILIGHT (90.0 + 50.0/60.0 + 18.0)

/** Get sunrise at the specific location on the specified date. Returns 
    TRUE if the sun actually rises in those conditions. */
BOOL st_sunrise (double latitude, double longitude, int year, int month, 
        int day, double zenith, double offset, int *hour, int *min);

/** Get sunset at the specific location on the specified date. Returns 
    TRUE if the sun actually sets in those conditions. */
BOOL st_sunset (double latitude, double longitude, int year, int month, 
        int day, double zenith, double offset, int *hour, int *min);

/** Get high noon at the specific location on the specified date. Returns 
    TRUE if the sun actually rises in those conditions. */
BOOL st_high_noon (double latitude, double longitude, int year, int month, 
        int day, double offset, int *hour, int *min);

#endif

