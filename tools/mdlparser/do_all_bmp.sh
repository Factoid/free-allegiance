#!/bin/bash
for f in ./Artwork/*bmp.mdl
do
  #echo "Processing "$f
  ./mdlparser.py --noImage --stats $f
  rc=$?
#  if [[ $rc != 0 ]]; then
#    echo "Failed on file "$f
#  fi
done

