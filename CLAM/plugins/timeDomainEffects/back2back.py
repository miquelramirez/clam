#!/usr/bin/python

#"""
#ln -s ../../../data_imm_bm/b2b/
#"""

import sys, os
sys.path.append('../../../CLAM/scripts/')
from audiob2b import runBack2BackProgram
from audiob2b import run

#data_path="/tmp"
params = dict(
	input_path="b2b/distance/inputs",
	output_path="b2b/distance/outputs",
)

back2BackTests = [
        ("doppler", # test name
		"OfflinePlayer networks/DopplerFractionalDelay.clamnetwork -s %(input_path)s/doppler.txt %(input_path)s/sweep_mono.wav -o -c 1 output.wav" % params
		, [
			"output.wav",
		]),

        ("dopplerMiddle", # test name
		"OfflinePlayer networks/DopplerFractionalDelay.clamnetwork -s %(input_path)s/dopplerMiddle.txt %(input_path)s/sweep_mono.wav -o -c 1 output.wav" % params
		, [
			"output.wav",
		]),

]
runBack2BackProgram(params['output_path'], sys.argv, back2BackTests)
