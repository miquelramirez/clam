#!/usr/bin/python

from sys import path
path.append("../../osc/oscpython")
import OSC
import time
print "playing samples"
OSC.Message("/clam/target",[int(0),"play",int(1)]).sendlocal(7000)
time.sleep(0.1)
OSC.Message("/clam/target",[int(1),"play",int(1)]).sendlocal(7000)
time.sleep(5)
print "stoping samples"
OSC.Message("/clam/target",[int(0),"stop",int(1)]).sendlocal(7000)
time.sleep(0.1)
OSC.Message("/clam/target",[int(1),"stop",int(1)]).sendlocal(7000)
