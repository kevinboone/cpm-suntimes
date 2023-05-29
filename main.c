/*=========================================================================

  suntimes 

  main.c

  Copyright (c)2022-3 Kevin Boone, GPLv3.0

=========================================================================*/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "getopt.h"
#include "defs.h"
#include "compat.h"
#include "strings.h"
#include "romwbw.h"
#include "suntimes.h"


char ini_file[20] = "suntimes.ini";
double lat, longt, offset;
int year, month, day;
BOOL has_lat = FALSE;
BOOL has_longt = FALSE;
BOOL has_date = FALSE;
BOOL has_offset = FALSE;

/*=========================================================================

  show_help 

=========================================================================*/
void show_help (void)
  {
  printf ("Usage: suntimes\n");
  printf ("   /h            Show this help message\n");
  printf ("   /v            Show version information\n");
  }

/*=========================================================================

  show_version

=========================================================================*/
void show_version (void)
  {
  printf ("suntimes version 0.1a\n");
  printf ("Copyright (c)2023 Kevin Boone\n");
  printf ("Distributed under the terms of the GNU Public Licence, v3.0\n");
  }

/*=========================================================================

  parse_date 

=========================================================================*/
void parse_date (char *s)
  {
  int n;
  n = sscanf (s, "%d-%d-%d", &year, &month, &day);
  if (n == 3) return;
  printf ("Invalid date: %s\n", s);
  exit (-1);
  }

/*=========================================================================

  do_config_var 

=========================================================================*/
void do_config_var (char *s)
  {
  char *p;
  strlwr (s);
  p = strchr (s, '=');
  if (p)
    {
    char *key, *value;
    key = s;
    *p = 0;
    value = p + 1;
    /*printf ("key=%s value=%s\n", key, value);*/
    if (strcmp (key, "lat") == 0)
      {
      lat = atof (value);
      has_lat = TRUE;
      }
    else if (strcmp (key, "long") == 0)
      {
      longt = atof (value);
      has_longt = TRUE;
      }
    else if (strcmp (key, "offset") == 0)
      {
      offset = atof (value);
      has_offset = TRUE;
      }
    else if (strcmp (key, "date") == 0)
      {
      parse_date (value); /* If error, does not return */
      has_date = TRUE;
      }
    }
  else
    {
    /* We will ignore anything without '=' in it */
    }
  }

/*=========================================================================

  read_ini 

=========================================================================*/
void read_ini (char *file)
  {
  static char line[80];
  FILE *f;
  f = fopen (file, "r"); 
  if (f)
    {
    while (!feof (f))
      {
      fgets (line, sizeof (line), f);
      trimright (line);
      do_config_var (line);
      }
    }
  else
    {
    printf ("Warning: can't open config file '%s'\n", ini_file);
    }

  }

/*=========================================================================

  print_sunrise 

=========================================================================*/
static void print_sunrise (double lat, double longt, 
        int year, int month, int day, double zenith, double offset)
  {
  int hour, min;
  if (st_sunrise (lat, longt, year, month, day, zenith, offset, 
       &hour, &min))
    printf ("%02d:%02d", hour, min);
  else
    printf ("n/a", hour, min);
  }

/*=========================================================================

  print_sunset

=========================================================================*/
static void print_sunset (double lat, double longt, 
        int year, int month, int day, double zenith, double offset)
  {
  int hour, min;
  if (st_sunset (lat, longt, year, month, day, zenith, offset, 
       &hour, &min))
    printf ("%02d:%02d", hour, min);
  else
    printf ("n/a", hour, min);
  }

/*=========================================================================

  print_high_noon

=========================================================================*/
static void print_high_noon (double lat, double longt, 
        int year, int month, int day, double offset)
  {
  int hour, min;
  if (st_high_noon (lat, longt, year, month, day, offset, 
       &hour, &min))
    printf ("%02d:%02d", hour, min);
  else
    printf ("n/a", hour, min);
  }

/*=========================================================================

  print_dms

=========================================================================*/
void print_dms (double angle)
  {
  double h = floor (angle);
  double m = floor ((angle - h) * 60.0);
  double s = floor ((angle - h - m / 60.0) * 3600.0);
  printf ("%d %d' %d''", (int)h, (int)m, (int)s);
  }

/*=========================================================================

  print_lat 

=========================================================================*/
void print_lat (double lat)
  {
  if (lat < 0)
    {
    print_dms (-lat);
    printf (" S");
    }
  else  
    {
    print_dms (lat);
    printf (" N");
    }
  }

/*=========================================================================

  print_longt

=========================================================================*/
void print_longt (double longt)
  {
  if (longt < 0)
    {
    print_dms (-longt);
    printf (" W");
    }
  else  
    {
    print_dms (longt);
    printf (" E");
    }
  }

/*=========================================================================

  main

=========================================================================*/
int main (int argc, char **argv)
  {
  int i, opt;

  char rtcbuff[6];
  if (rwbw_getrtc (rtcbuff) == 0)
    {
    year = (int)rtcbuff[0] + 2000;
    month = (int)rtcbuff[1];
    day = (int)rtcbuff[2];
    has_date = TRUE;
    }

  /* argv[0] is not set by the HI-TECH C compiler */
  argv[0] = "suntimes";

  while ((opt = getopt (argc, argv, "?vVhHc:C:")) != -1)
    {
    switch (opt)
      {
      case '?':
      case ':':
      case 'h':
      case 'H':
        show_help ();
        return 0; 
      case 'v': case 'V':
        show_version ();
        return 0;
      case 'c': case 'C':
	strncpy (ini_file, optarg, sizeof (ini_file) - 1);
        break;
      default:
        return 1;
      }
    }

  read_ini(ini_file);
  for (i = optind; i < argc; i++)
    {
    do_config_var (argv[i]); 
    }

  if (has_lat && has_longt && has_date && has_offset)
    {
    printf ("Location: ");
    print_longt (longt);
    printf (", ");
    print_lat (lat);
    printf ("\nOffset from UTC: %.1f hour\n", offset);
    printf ("%s %d, %d\n", dt_mon_name (month), day, year);
    printf ("Nautical twilight starts: ");
    print_sunrise (lat, longt, year, month, day, NAUTICAL_TWILIGHT, offset);
    printf ("\nCivil twilight starts: ");
    print_sunrise (lat, longt, year, month, day, CIVIL_TWILIGHT, offset);
    printf ("\nSunrise: ");
    print_sunrise (lat, longt, year, month, day, DEFAULT_ZENITH, offset);
    printf ("\nHigh noon: ");
    print_high_noon (lat, longt, year, month, day, offset);
    printf ("\nSunset: ");
    print_sunset (lat, longt, year, month, day, DEFAULT_ZENITH, offset);
    printf ("\nCivil twilight ends: ");
    print_sunset (lat, longt, year, month, day, CIVIL_TWILIGHT, offset);
    printf ("\nNautical twilight ends: ");
    print_sunset (lat, longt, year, month, day, NAUTICAL_TWILIGHT, offset);
    printf ("\n");
    }
  else
    {
    fputs ("Some parameters were not provided.\n", stderr); 
    }

  return 0;
  }

