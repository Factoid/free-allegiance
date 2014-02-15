#!/usr/bin/env python3
import json

with open( "test.json", "r" ) as f:
  json_str = json.load(f) 
  with open( "test2.json", "w" ) as outF:
    json.dump(json_str, outF, sort_keys=True, indent=4)
