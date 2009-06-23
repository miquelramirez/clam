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
	os.system('ln -s ../../../../acustica/bformat2binaural/equivalentIRs/mitKemarFullL/E%s.wav .'%i)

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
	("mono2hoa3",
		"OfflinePlayer example-data/mono2hoa3.clamnetwork b2b/spacialization/inputs/jaume.wav -o -c 16 hoa3.wav"
		, [
			"hoa3.wav",
		]),
	("hoa2sixteen",
		"OfflinePlayer example-data/mono2hoa3.clamnetwork b2b/spacialization/inputs/jaume.wav -o -c 16 hoa3.wav;"
		"OfflinePlayer example-data/hoa32fifteen.clamnetwork hoa3.wav -o -c 15 sixteen.wav;"
		, [
			"hoa3.wav",
			"sixteen.wav",
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
		"OfflinePlayer example-data/bformat2surround.clamnetwork bformat.wav -o -c 5 surround.wav;"
		, [
			"bformat.wav",
			"surround.wav",
		]),
	("bformat2sixteen",
		"OfflinePlayer example-data/mono2bformat.clamnetwork b2b/spacialization/inputs/jaume.wav -o -c 4 bformat.wav;"
		"OfflinePlayer example-data/bformat2fifteen.clamnetwork bformat.wav -o -c 15 sixteen.wav;"
		, [
			"bformat.wav",
			"sixteen.wav",
		]),
	("vbap_to_15",
		"OfflinePlayer example-data/15_direct_sound_choreography.clamnetwork b2b/spacialization/inputs/jaume.wav -o -c 15 vbap15.wav;"
		, [
			"vbap15.wav"
		 ]),
    ("ladspab2b-test",
        "OfflinePlayer example-data/mono2bformat_via_ladspa.clamnetwork b2b/spacialization/inputs/jaume.wav -o -c 4 bformat.wav;"
        , [
            "bformat.wav",
        ]),

]

runBack2BackProgram(data_path, sys.argv, back2BackTests)





