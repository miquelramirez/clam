#!/bin/bash
# Very simple processing generation script (standard GPL and current year)
# Usage: ./generate.sh PROCESSING_NAME TEMPLATE_FILE

PROCESSING_NAME=$1
TEMPLATE_FILE=$2

./TemplateGenerator.py $TEMPLATE_FILE
TEMPLATE_NAME=`head -n1 $TEMPLATE_FILE | cut -f2 -d:`
./TemplatedPluginsGenerator.py $PROCESSING_NAME $TEMPLATE_NAME  "" GPL 2008
