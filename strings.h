/*=========================================================================

  strings.h

  String manipulation.

  Copyright (c)2022-3 Kevin Boone, GPLv3.0

=========================================================================*/
#ifndef __STRINGS_H
#define __STRINGS_H

/** Remove CR and LF characters from the end of a string. */
void trimright (char *s);

/** Convert an ASCII string to lower case. */
void strlwr (char *s);

#endif




