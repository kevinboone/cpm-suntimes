/*=========================================================================

  suntimes 

  suntimes.c

  This file contains the actual computation of sunrise and sunset times.
  It is simplified version of one that I have used in a number of
  other projects, and is documented elsewhere. This version is rather
  unreadable, as I've shorted all the identifiers to make them more
  palatable to CP/M C compilers.

  Copyright (c)1999-2023 Kevin Boone, GPLv3.0

=========================================================================*/
#include <math.h>
#include "defs.h" 
#include "dtutil.h" 
#include "suntimes.h" 

#define TYPE_SUNRISE 0
#define TYPE_SUNSET 1

#define DEG_PER_HOUR 15

#define DEG_TO_RAD 0.0174532925199433
#define RAD_TO_DEG 57.2957795130823

/*=========================================================================

  asin_deg 

=========================================================================*/
static double asin_deg (double a)
  {
  return RAD_TO_DEG * asin (a);
  }

/*=========================================================================

  acos_deg 

=========================================================================*/
static double acos_deg (double a)
  {
  return RAD_TO_DEG * acos (a);
  }

/*=========================================================================

  sin_deg 

=========================================================================*/
static double sin_deg (double deg)
  {
  return sin (deg * DEG_TO_RAD);
  }

/*=========================================================================

  cos_deg 

=========================================================================*/
static double cos_deg (double deg)
  {
  return cos (deg * DEG_TO_RAD);
  }

/*=========================================================================

  tan_deg 

=========================================================================*/
static double tan_deg (double deg)
  {
  return tan (deg * DEG_TO_RAD);
  }

/*=========================================================================

  st_get_h_from_merid

=========================================================================*/
static double st_get_h_from_merid (double longitude)
  {
  return longitude / DEG_PER_HOUR;
  }

/*=========================================================================

  st_get_approx_t_days

=========================================================================*/
static double st_get_approx_t_days (int doy, double h_from_m, int type)
  {
  double ret;
  if (type == TYPE_SUNRISE)
    ret = doy + ((6.0 - h_from_m) / 24);
  else
    ret = doy + ((18.0 - h_from_m) / 24);
  return ret;
  }

/*=========================================================================

  st_get_mean_anomaly 

=========================================================================*/
static double st_get_mean_anomaly (int doy, double longitude, int type)
  {
  return (0.9865 * st_get_approx_t_days (doy, 
        st_get_h_from_merid (longitude), type)) - 3.289;
  }

/*=========================================================================

  sun_true_long 

=========================================================================*/
double st_get_sun_true_long (double sun_mean_anomaly)
  {
  double l = sun_mean_anomaly + (1.916 * sin_deg (sun_mean_anomaly))
                + (0.020 * sin_deg (2 * sun_mean_anomaly)) + 282.634;
    
  /* get longitude into 0-360 degree range */
  if (l >= 360.0) l = l - 360.0;
  if (l < 0) l = l + 360.0; 
  return l;

  }

/*=========================================================================

  st_get_ra_hrs 

=========================================================================*/
static double st_get_ra_hrs (double sun_true_long)
  {
  double a = 0.91764 * tan_deg (sun_true_long);
  double ra = RAD_TO_DEG * atan(a);
  
  double lQuadrant  = floor (sun_true_long / 90.0) * 90.0;
  double raQuadrant = floor (ra / 90.0) * 90.0;
  ra = ra + (lQuadrant - raQuadrant);
        
  return ra / DEG_PER_HOUR; /* convert degrees to hours */
  }

/*=========================================================================

  st_get_cos_lha 

=========================================================================*/
static double st_get_cos_lha (double sun_true_long, 
         double latitude, double zenith)
  {
  double sin_dec = 0.39782 * sin_deg (sun_true_long);
  double cos_dec  = cos_deg (asin_deg (sin_dec));
        
  double cosH = (cos_deg (zenith) 
     - (sin_dec * sin_deg(latitude))) / (cos_dec * cos_deg (latitude)); 
        
  /* TODO Check bounds (but I haven't checked them for the last 20 years,
     so perhaps there's no need :) */

  return cosH;

  }

/*=========================================================================

  get_local_mean_time

=========================================================================*/
static double st_get_local_mean_time (double local_hr, double  sun_ra_hours,
     double approx_time_days)
  {
  return local_hr + sun_ra_hours - (0.06571 * approx_time_days)  
    - 6.622;

  }

/*=========================================================================

  st_to_hms

=========================================================================*/
static void st_h_to_hm (double hr, int *hour, int *min)
  {
  double h, m;

  h = floor (hr);
  m = floor ((hr - h) * 60.0);
 
  *hour = (int)h;
  *min = (int)m;
  }

/*=========================================================================

  st_event

=========================================================================*/
static BOOL st_event (double latitude, double longitude, int year, int month, 
        int day, double zenith, double offset, double *event_hr,
        int type)
  {
  int doy = dt_get_doy (year, month, day);
  double sun_mean_anomaly, sun_true_long, sun_ra_hrs, cos_lha, local_hr,
     local_hr_angle, local_mean_time, temp;

  sun_mean_anomaly = st_get_mean_anomaly (doy, longitude, type);

  sun_true_long = st_get_sun_true_long (sun_mean_anomaly); 

  sun_ra_hrs = st_get_ra_hrs (sun_true_long);

  cos_lha = st_get_cos_lha (sun_true_long, latitude, zenith);

  if (cos_lha > 1 || cos_lha <= -1)
    {
    return FALSE;
    }

  if (type == TYPE_SUNRISE)
    local_hr_angle = 360.0 - acos_deg (cos_lha);
  else
    local_hr_angle = acos_deg (cos_lha);

  local_hr = local_hr_angle / DEG_PER_HOUR;

  local_mean_time = st_get_local_mean_time (local_hr, sun_ra_hrs,
     st_get_approx_t_days (doy, st_get_h_from_merid (longitude), type)); 

  temp = local_mean_time - st_get_h_from_merid (longitude);
  temp += offset;
  if (temp < 0) temp += 24;
  if (temp > 24) temp -= 24;

  *event_hr = temp;

  return TRUE;
  }

/*=========================================================================

  st_sunrise

=========================================================================*/
BOOL st_sunrise (double latitude, double longitude, int year, int month, 
        int day, double zenith, double offset, int *hour, int *min)
  {
  double hr;
  if (st_event (latitude, longitude, year, month, 
        day, zenith, offset, &hr, TYPE_SUNRISE))
    {
    st_h_to_hm (hr, hour, min);
    return TRUE;
    }
  else
    return FALSE;
  }

/*=========================================================================

  st_sunset

=========================================================================*/
BOOL st_sunset (double latitude, double longitude, int year, int month, 
        int day, double zenith, double offset, int *hour, int *min)
  {
  double hr;
  if (st_event (latitude, longitude, year, month, 
        day, zenith, offset, &hr, TYPE_SUNSET))
    {
    st_h_to_hm (hr, hour, min);
    return TRUE;
    }
  else
    return FALSE;
  }

/*=========================================================================

  st_high_noon

=========================================================================*/
BOOL st_high_noon (double latitude, double longitude, int year, int month, 
        int day, double offset, int *hour, int *min)
  {
  double hr1, hr2;
  if (st_event (latitude, longitude, year, month, 
        day, DEFAULT_ZENITH, offset, &hr1, TYPE_SUNRISE))
    {
    if (st_event (latitude, longitude, year, month, 
          day, DEFAULT_ZENITH, offset, &hr2, TYPE_SUNSET))
      {
      st_h_to_hm ((hr1 + hr2) / 2, hour, min);
      return TRUE;
      }
    }
  return FALSE;
  }


