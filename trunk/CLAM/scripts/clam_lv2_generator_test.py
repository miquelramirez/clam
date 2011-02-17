#!/usr/bin/python
# -*- coding: utf-8 -*-

from audiob2b import runBack2BackProgram
import sys, os

data_path="../../../clam-test-data/b2b/lv2_plugin/"

othercable_network = os.path.join(data_path,"inputs","othercable.clamnetwork")
clam_lv2_uri = "http://clam-project.org/examples/lv2"

back2BackTests = [
	("simple_ttl",
		"./clam_lv2_generator.py --ttl -y mylib -u %(uri)s %(network)s > output.ttl" 
		% dict(
			network=othercable_network,
			uri=clam_lv2_uri,
		) , [
			"output.ttl",
		]),

	("gui_ttl",
		"./clam_lv2_generator.py --ttl -y mylib -u %(uri)s -g %(guibin)s %(network)s > output.ttl" 
		% dict(
			network=othercable_network,
			uri=clam_lv2_uri,
			guibin="myuilib",
		) , [
			"output.ttl",
		]),
	# TODO: --ttl without --binary gives error
	# TODO: --ttl without --uribase gives error
	# TODO: --ttl with --doap
	# TODO: --manifest networks
	# TODO: --main networks
]
runBack2BackProgram(data_path, sys.argv, back2BackTests)


