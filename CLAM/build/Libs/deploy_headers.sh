#!/bin/bash

HEADERS=`find ../../src -name *.h*`

for header in $HEADERS; 
  do 
  cp $header includes/CLAM; 
done;