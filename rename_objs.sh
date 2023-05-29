#!/bin/sh
i=0
for x in $@ ; do
  i=$((i+1))
  cp $x _$i.obj 
done
