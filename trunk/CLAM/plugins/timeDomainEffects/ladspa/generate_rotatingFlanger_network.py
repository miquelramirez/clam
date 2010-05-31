#!/usr/bin/python
import os
import sys
# shared paths
base_path = os.path.expanduser("~")
base_path = os.path.abspath(os.path.join(os.path.dirname(__file__),"../../../../.."))
clam_path = os.path.join(base_path, "clam")
sys.path.append(os.path.join(clam_path,"CLAM/scripts"))
import clamrefactor

def generate_flanger_network(layoutfile):
	channels = len([line for line in open(layoutfile) if line.strip()[0] != '#'])
	print "num channels: ", channels
	template_filename = "rotatingFlanger_template.clamnetwork"
	filename = "rotatingFlanger_%i.clamnetwork" % channels
	network = clamrefactor.ClamNetwork(file(template_filename))
	network.setConfig("AudioBufferSink", "NSinks", str(channels))
	network.setConfig("ProximityBasedAmplitudePanning", "SpeakerLayout", layoutfile)
	# generate needed ports, processings and connections
	y_offset = 50
	for i in range(channels) :
		if i == 0: continue
		network.duplicateProcessing("AudioBufferAmplifier_0", "AudioBufferAmplifier_%i" % i, 10, y_offset*i)
		network.addConnection('control_connection', "ProximityBasedAmplitudePanning", "%02i"%i, "AudioBufferAmplifier_%i" % i, "Gain")
		network.addConnection('port_connection', "RotatingFlanger", "OutputBuffer", "AudioBufferAmplifier_%i"%i, "Input Audio")
		network.addConnection('port_connection', "AudioBufferAmplifier_%i"%i, "Audio Output", "AudioBufferSink", "%i"%(i+1))
	network.dump(file(filename,"w"))


layoutfiles = ["/usr/local/share/clam/layouts/15.layout", "/usr/local/share/clam/layouts/22.layout"]
for layoutfile in layoutfiles:
	generate_flanger_network( layoutfile )
