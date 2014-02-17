#!/usr/bin/env python3
# Simple script to take the json output from Cereal and put it in sorted key order
# This is just so it's easier to compare the output from the python mdlparser.py script
import json

with open( "test.json", "r" ) as f:
  json_str = json.load(f) 
  with open( "test2.json", "w" ) as outF:
    json.dump(json_str, outF, sort_keys=True, indent=4)
