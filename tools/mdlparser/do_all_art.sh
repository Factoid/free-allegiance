#!/bin/bash
for f in ../Artwork/*.mdl
do
  echo "Processing "$f
  ./mdlparser.py `basename ${f%.*}` > /dev/null 2> /dev/null
  rc=$?
  if [[ $rc != 0 ]]; then
    echo "Failed on "$f
    echo `basename ${f%.*}` >> failures.txt
  fi
done

