#!/bin/bash

cat `dirname $0`/Info.plist | sed -e s/EXECUTABLE/$2/ | sed -e s/BUNDLENAME/$2/ > $1

