/*===========================================================================

  getopt.h

  Copyright (c)2021-3 Kevin Boone, GPL v3.0

===========================================================================*/
#ifndef __GETOPT_H
#define __GETOPT_H

extern char *optarg;
extern int optind, opterr, optopt, optpos, optreset;

/*
 * HI-TECH C allows modern-style formal paramters declarations in 
 * prototypes, but Aztec C does not.
*/
/*
int getopt (int argc, char **argv, char *optstring)
*/
int getopt();

#endif /* getopt.h */

