#!/bin/bash
# A script to check that HI-TECH C and the CP/M emulator are installed
echo Checking prerequisites... 
if  which cpm > /dev/null; then
  if [ -f "p1.com" ] ; then
    echo CP/M emulator and Hi-Tech C appear to be installed
    true
  else
    echo ------------------------------------------------------------------
    echo "-- Hi-Tech C is not installed. Do 'make prepare' first if you have"
    echo -- curl and lbrate. Or obtain the binaries from
    echo -- https://github.com/agn453/HI-TECH-Z80-C/tree/master/dist
    echo ------------------------------------------------------------------
    false
  fi
else
    echo ------------------------------------------------------------------
    echo -- CP/M emulator 'cpm' is not installed. Get it from:
    echo -- https://codeload.github.com/jhallen/cpm/zip/refs/heads/master 
    echo ------------------------------------------------------------------
    false
fi
