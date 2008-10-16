#!/usr/bin/python

import sys
sys.path.append('../../../CLAM/scripts/')
from audiob2b import runBack2BackProgram

back2BackTests = [
	("mono2bformat",
		"OfflinePlayer example-data/mono2bformat.clamnetwork jaume.wav -o -c 4 bformat.wav"
		, [
			"bformat.wav",
		]),
	# TODO: take an existing bformat
	("bformat2binaural",
		"OfflinePlayer example-data/mono2bformat.clamnetwork jaume.wav -o -c 4 bformat.wav;"
		"OfflinePlayer example-data/bformat2binaural.clamnetwork bformat.wav -o -c 2 binaural.wav;"
		, [
			"bformat.wav",
			"binaural.wav",
		]),
	# TODO: take an existing bformat
	("bformat2surround",
		"OfflinePlayer example-data/mono2bformat.clamnetwork jaume.wav -o -c 4 bformat.wav;"
		"OfflinePlayer example-data/bformat2surround.clamnetwork bformat.wav -o -c 2 surround.wav;"
		, [
			"bformat.wav",
			"surround.wav",
		]),
]

runBack2BackProgram("b2b", sys.argv, back2BackTests)





