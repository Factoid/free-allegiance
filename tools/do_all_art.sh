#!/bin/bash
for f in ../Artwork/*.mdl
do
  echo "Processing "$f
  ./mdlparser.py `basename ${f%.*}` 2> /dev/null
  rc=$?
  if [[ $rc != 0 ]]; then
    echo "Failed on file "$f
    exit $rc
  fi
done

