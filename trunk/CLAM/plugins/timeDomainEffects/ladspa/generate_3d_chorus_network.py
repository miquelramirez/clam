#!/usr/bin/python
import os
import sys
# shared paths
base_path = os.path.expanduser("~")
base_path = os.path.abspath(os.path.join(os.path.dirname(__file__),"../../.."))
clam_path = os.path.join(base_path, "clam")
sys.path.append(os.path.join(clam_path,"CLAM/scripts"))
import clamrefactor

def generate_3d_chorus_network(layoutfile):
	channels = len([line for line in open(layoutfile) if line.strip()[0] != '#'])
	print "num channels: ", channels
	template_filename = "3d_chorus_template.clamnetwork"
	filename = "3d_chorus_%i.clamnetwork" % channels
	network = clamrefactor.ClamNetwork(file(template_filename))
	network.setConfig("AudioBufferSink", "NSinks", str(channels))
	# generate needed ports, processings and connections
	y_offset = 150
	f=open(layoutfile)
	for i in range(channels) :
		if i == 0: 
			line = f.readline()		
			thisline=line.split(" ")		
			network.setConfig("PhaseOffset_%i" %i, "Default", str(thisline[1]))
			continue
		network.duplicateProcessing("MonoFractionalDelayModulation_0", "MonoFractionalDelayModulation_%i" % i, 10, y_offset*i)
		network.duplicateProcessing("PhaseOffset_0", "PhaseOffset_%i" % i, 10, y_offset*i)
		network.duplicateProcessing("param_Variability_0", "param_Variability_%i" % i, 10, y_offset*i)

		
		network.addConnection('control_connection', "width", "output", "MonoFractionalDelayModulation_%i"%i, "width in msecs")
		network.addConnection('control_connection', "mod_Frequency", "output", "MonoFractionalDelayModulation_%i"%i, "freq in Hz")
		network.addConnection('control_connection', "center_Tap", "output", "MonoFractionalDelayModulation_%i"%i, "centerTap msecs")
		network.addConnection('control_connection', "PhaseOffset_%i"%i, "out", "MonoFractionalDelayModulation_%i"%i, "offset in degrees")
		network.addConnection('control_connection', "param_Variability_%i"%i, "output", 
				      "MonoFractionalDelayModulation_%i"%i, "variability of parameters")

		network.addConnection('port_connection', "AudioBufferSource", "1", "MonoFractionalDelayModulation_%i"%i, "InputBuffer")
		network.addConnection('port_connection', "MonoFractionalDelayModulation_%i"%i, "OutputBuffer", "AudioBufferSink", "%i"%(i+1))
				
		line = f.readline()		
		thisline=line.split(" ")		
		network.setConfig("PhaseOffset_%i" %i, "Default", str(thisline[1]))
	f.close()		
	network.dump(file(filename,"w"))


layoutfiles = ["/usr/local/share/clam/layouts/14.layout", "/usr/local/share/clam/layouts/15.layout", "/usr/local/share/clam/layouts/22.layout"]
for layoutfile in layoutfiles:
	generate_3d_chorus_network( layoutfile )
