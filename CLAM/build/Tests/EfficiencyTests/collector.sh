#!/bin/bash
make depend
make
date=`date "+%Y%m%d-%H%M%S"`
callgrind --simulate-cache=yes --base=callgrind-$date.out ./EfficiencyTests
logfile=callgrind-$date.out.*
python ../../../test/EfficiencyTests/DataCollectorTrigger.py $logfile

