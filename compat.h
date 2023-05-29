/*===========================================================================

  compat.h

  Various CP/M / Linux compatibility constants and function
  declarations.

  Copyright (c)2022-3 Kevin Boone, GPL v3.0

===========================================================================*/
#ifndef __COMPAT_H
#define __COMPAT_H

/** 
  File creation flags. HI-TECH C only understands 0, 1, and 2 -- none
  of the more modern flags like O_CREAT exist. In fact, open() only
  works on a file that already exists. 
*/
#ifndef O_RDONLY
#define O_RDONLY 0
#endif
#ifndef O_WRONLY
#define O_WRONLY 1
#endif
#ifndef O_RDWR
#define O_RDWR 2
#endif

/* Standard CP/M block size in bytes */
#define CPMBLK 256

/* Neither HI-TECH nor Aztec C have 'const', but we should use it 
 * when building for Linux. */
#ifdef z80 
#define CONST 
#else
#define CONST const
#endif

#endif /* compat.h */

