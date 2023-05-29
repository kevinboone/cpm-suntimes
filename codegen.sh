#!/bin/bash
# A script to invoke the HI-TECH cgen utility, and set the exit code for
#  make according to whether it worked or not

if [ "$1" == "" ] ; then
  ! echo Usage: codegen.sh file.c
else
  name=$(echo "$1" | cut -f 1 -d '.')
  cpm cgen $1 | grep -v "^A>" > $name.asm
  ! grep -i ^$name $name.asm
fi
