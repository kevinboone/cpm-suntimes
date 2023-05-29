#!/bin/bash
# A script to invoke HI-TECH's OPTIM.COM optimizer, and set the exit code
#   to indicate success or failure

if [ "$1" == "" ] ; then
  ! echo Usage: optim.sh file.asm
else
  name=$(echo "$1" | cut -f 1 -d '.')
  cpm optim $1 | grep -v "^A>" > $name.as2
  ! grep -i ^$name $name.as2
fi
