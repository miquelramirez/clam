#!/usr/bin/python

from Aggregator import *
from Schema import *
import sys

if sys.argv[1] == "-" :
	script = sys.stdin
else :
	script = file(sys.argv[1])
sources = sys.argv[2:]

target = Schema()
aggregator = Aggregator(script)
aggregator.run(target, [ Schema(file(source)) for source in sources ] )

target.Dump(sys.stdout)



