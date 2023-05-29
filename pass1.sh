#!/bin/bash
# A script to invoke HI-TECH P1.COM. This is the tool that is most likely to
#   fail if there are errors in the C code. So we need to report those errors
#   as cleanly as we can, bearing in mind that they will be mixed up in
#   the program's p-code output

if [ "$1" == "" ] ; then
  ! echo Usage: pass1.sh file.pre
else
  name=$(echo "$1" | cut -f 1 -d '.')
  cpm p1 $1 | grep -v "^A>" > $name.p1
  if grep -i ^$name $name.p1 ; then
    grep "^\\s\\s" $name.p1
    false
  else
    true 
  fi
fi
