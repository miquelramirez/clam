#!/usr/bin/python

import sys
sys.path.append('../../../CLAM/scripts/')
from audiob2b import runBack2BackProgram

# Pau, quan tinguem un directori pels fitxers b2b_spatialization -> b2b/spatialization

back2BackTests = [
	("mono2bformat", "OfflinePlayer example-data/mono2bformat.clamnetwork jaume.wav -o -c 4 output.wav", [
			("b2b_spacialization_mono2bformat_output", "output.wav"),
		]),
]

runBack2BackProgram(sys.argv, back2BackTests)





