#!/usr/bin/python

"""
Data for this B2B test can be found at the clam-test-data repository.
Create a link to clam-test-data/b2b to this directory.
Create links to acustica/bformat2binaural/{Ew,Ex,Ey,Ez} to this directory.

Needed setup:
 cd ~/acustica/HRTFs/ && ./prepareAll.sh 
 cd ~/acustica/bformat2binaural/ && ./generateAllEquivalent.sh 
"""

import sys, os
sys.path.append('../../../CLAM/scripts/')
from audiob2b import runBack2BackProgram


os.system('rm E?.wav')
for i in 'xyzw' :
	os.system('ln -s ~/acustica/bformat2binaural/equivalentIRs/MIT/full-L/E%s.wav .'%i)

data_path="b2b/spacialization"

back2BackTests = [
	("mono2binaural",
		"OfflinePlayer example-data/mono2binaural.clamnetwork b2b/spacialization/inputs/jaume.wav -o -c 2 binaural.wav"
		, [
			"binaural.wav",
		]),
	("mono2bformat",
		"OfflinePlayer example-data/mono2bformat.clamnetwork b2b/spacialization/inputs/jaume.wav -o -c 4 bformat.wav"
		, [
			"bformat.wav",
		]),
	# TODO: take an existing bformat
	("bformat2binaural",
		"OfflinePlayer example-data/mono2bformat.clamnetwork b2b/spacialization/inputs/jaume.wav -o -c 4 bformat.wav;"
		"OfflinePlayer example-data/bformat2binaural.clamnetwork bformat.wav -o -c 2 binaural.wav;"
		, [
			"bformat.wav",
			"binaural.wav",
		]),
	# TODO: take an existing bformat
	("bformat2surround",
		"OfflinePlayer example-data/mono2bformat.clamnetwork b2b/spacialization/inputs/jaume.wav -o -c 4 bformat.wav;"
		"OfflinePlayer example-data/bformat2surround.clamnetwork bformat.wav -o -c 2 surround.wav;"
		, [
			"bformat.wav",
			"surround.wav",
		]),
]

runBack2BackProgram(data_path, sys.argv, back2BackTests)





