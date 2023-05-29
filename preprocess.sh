#!/bin/bash
# A script to invoke HI-TECH's CPP.COM preprocessor, and set the exit code
#   to indicate success or failure

if [ "$1" == "" ] ; then
  ! echo Usage: preprocess.sh file.c
else
  name=$(echo "$1" | cut -f 1 -d '.')
  cpm cpp -Ia: $1 | grep -v "^A>" > $name.pre 
  ! grep -i ^$name $name.pre
fi
