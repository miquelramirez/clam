import Dummy_NetworkProxy

def proxy() :
	return Dummy_NetworkProxy.Dummy_NetworkProxy(
		[
			dict(
				type = "AudioSource",
				name = "Processing1",
				config = dict(
					ConfigParam1 = "default1",
					ConfigParam2 = "default2",
				),
				inports = [
					["Inport1", "type1"],
					["Inport2", "type1"],
					['Inport3', 'type1'],
					['Inport4', 'type1'],
				],
				outports = [
					["Outport1", "type1"],
					["Outport2", "type1"]
				],
				incontrols = [
					["Incontrol1", "type2"],
					["Incontrol2", "type2"]
				],
				outcontrols = [
					["Outcontrol1", "type2"],
					["Outcontrol2", "type2"]
				],
			),
			dict(
				type = "AudioSink",
				name = "Processing2",
				config = dict(
					ConfigParam1 = "default1",
					ConfigParam2 = "default2",
				),
				inports = [
					["Inport1", "type1"],
					["Inport2", "type1"]
				],
				outports = [
					["Outport1", "type1"],
					["Outport2", "type1"]
				],
				incontrols = [
					["Incontrol1", "type2"],
					["Incontrol2", "type2"]
				],
				outcontrols = [
					["Outcontrol1", "type2"],
					["Outcontrol2", "type2"]
				],
			),
		],
		[
			("Processing1", "Outport1", "Processing2", "Inport2"),
			("Processing1", "Outport2", "Processing2", "Inport2"),
			("Processing2", "Outport1", "Processing1", "Inport2"),
		],
		[
			("Processing1", "Outcontrol1", "Processing2", "Incontrol2"),
			("Processing1", "Outcontrol2", "Processing2", "Incontrol2"),
			("Processing2", "Outcontrol1", "Processing1", "Incontrol2"),			
		]
)