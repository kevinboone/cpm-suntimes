# cpm-suntimes

Version 0.1a

## What is this?

`suntimes` is a simple utility for CP/M (Z80 platform) which displays
the times of sunrise and sunset, and related information, at a specific
location on a specified date. It is a simplified version of a similar
utility that I maintain for Linux. 

`suntimes` can get the current date from a real-time clock on a CP/M
machine with ROMWBW firmware.

All parameters can be specified on the command line or, where it makes
sense, in a configuration file.

## Sample usage

    c> suntimes lat=51.51 long=0.118 offset=1 date=2023-5-29

    Location: 0 7' 4'' W, 51 30' 35'' N
    Offset from UTC: 1.0 hour
    May 29, 2023
    Nautical twilight starts: 02:50
    Civil twilight starts: 03:59
    Sunrise: 04:52
    High noon: 12:58
    Sunset: 21:05
    Civil twilight ends: 21:57
    Nautical twilight ends: 23:06

All parameters can be given in a configuration file. It makes sense to
put the latitude and longitude in this file, but probably not the date
(although this is allowed).

## Configuration file

`suntimes` looks for a file called `suntimes.ini` on the current logged-in
drive. This behaviour can be overridden on the command line:

    c> suntimes /c f:london.ini ...

See the included `suntimes.ini` for a sample. 

## Parameters

*date*

The date parameter has the form `date=YYYY-MM-DD`. At present, no other
format is recognized. The date must be a four-digit number. If the current
date is read from a real-time clock, and the date is only two digits,
then 2000 is added to it. However, this isn't done for the command-line --
it's possible to check sunrise times before 2000, in the unlikely event that
the need arises.

*offset*

The time offset in hours from UTC (GMT) at the location of the calculation.
Bear in mind that in many locations this will vary according to 
daylight-savings and for other reasons.

This offset is how far _ahead_ of GMT the location is, on the date of the
calculation. So for UK `offset=1` in summer DST and `offset=0` in the
winter. The corresponding offsets for, for example, Paris, France
are `2` and `1`.

`suntimes` understands fractional UTC offsets as apply, for example, in India. 

*lat*

A decimal latitude. Positive latitudes are north of the equator.

*long*

A decimal longitudes. Positive longitudes are north of the equator.

## Limitations 

Except on a ROMWBW system with a real-time clock, you'll always have
to specify the date of the calculation (probably on the command line),
even if it's today. CP/M does not mandate a real-time clock, and does
not provide a system call to read it even if there is one.

CP/M does not have a timezone database, which is why the user must 
specify the UTC offset. It would be nice if we could work out the UTC
offset from the location but, in general, we can't. Not in a way that
would be practicable on CP/M, anyway. Timezone is really a political
matter, not a geographical one; daylight savings time even more so.

The Z80 math
library only provides 32-bit precision, so results are not as accurate
as those from my Linux utilities that perform similar calculations.
However, I've checked the results against those published by the
BBC, for a range of dates and locations, and they agree to within
a couple of minutes. 

In some extreme northerly or southerly locations, sunrise is an annual
event. If there is no sunrise or sunset at a particular location and
date, `suntimes` should be well-behaved, and display a message to that
effect. However, it won't behave well if a specified UTC offset is
meaningless at a location.

It's a subtle point, but the date specified to `suntimes` is a date
relative to UTC, not at the location. The date at the location could be
a day ahead. This will introduce an error of a few minutes at such
locations.

## Where to get latitude and longitude for a location

The Google Maps web interface will display latitude and longitude in
the format this program understands. 

## Building

`suntimes` will build on CP/M using the Hi-Tech Z80 C compiler and Z80
assembler. In practice, I build it on Linux using an emulator to run these
tools. Please see the `Makefile` for how this is done.  The process is pretty
ugly, beause the Hi-Tech tools were never (of course) designed to be integrated
with `make`. 

In principle, with the proper set-up, building on Linux just amounts to running

    make

which creates `suntimes.com`. However, the amount of set-up needed to
get to this stage is rather considerable.

## Technical notes ##

At nearly 30kB, `suntimes` is a bloated beast of a program by CP/M
standards. Most of that space is consumed by the floating-point math
library. I could shave off a couple of kB by, for example, reducing the
error checking and making the text messages shorter, but it hardly
seems worth it when the math library uses over 20kB.

I chose Hi-Tech C for the implementation because its C language is
the most 'modern' (although still not very modern), so it was relatively
easy to convert the algorithmic part of the application from my Linux
version. I had to shorten all the identifier names, however. 

I only use Z80 assembler for reading the real-time clock in a ROMWBW
system. The program should not require the presence of ROMWBW but, of course,
it won't be able to infer the current date.

## Author and legal

`suntimes` for CP/M is maintained by Kevin Boone and is copyright (c)1998-2023
Kevin Boone. It is distributed under the terms of the GNU Public Licence, v3.0.
There is, of course, no warranty of any kind. The astrononical calculations on
which this program is based are derived from published materials that I believe
to be in the public domain. 

## Revisions

0.1a May 2023

First working version

