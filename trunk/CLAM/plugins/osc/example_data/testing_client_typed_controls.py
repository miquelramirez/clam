#!/usr/bin/python

from sys import path
path.append("../oscpython")
import OSC
OSC.Message("/clam/target",("testing the","typed","controls",3.14)).sendlocal(7000)