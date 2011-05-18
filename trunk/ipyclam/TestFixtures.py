import Dummy_NetworkProxy

def empty() :
	return Dummy_NetworkProxy.Dummy_NetworkProxy([], [], [])

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
					["InPort1", "type1"],
					["InPort2", "type1"],
					['InPort3', 'type1'],
					['InPort4', 'type1'],
				],
				outports = [
					["OutPort1", "type1"],
					["OutPort2", "type1"]
				],
				incontrols = [
					["InControl1", "type2"],
					["InControl2", "type2"]
				],
				outcontrols = [
					["OutControl1", "type2"],
					["OutControl2", "type2"]
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
					["Incontrol2", "type2"],
					["Incontrol3", "DifferentType"]
				],
				outcontrols = [
					["Outcontrol1", "type2"],
					["Outcontrol2", "type2"]
				],
			),
		],
		[
			("Processing1", "OutPort1", "Processing2", "Inport2"),
			("Processing1", "OutPort2", "Processing2", "Inport2"),
			("Processing2", "Outport2", "Processing1", "InPort2"),
		],
		[
			("Processing1", "OutControl1", "Processing2", "Incontrol2"),
			("Processing1", "OutControl2", "Processing2", "Incontrol2"),
			("Processing2", "Outcontrol1", "Processing1", "InControl2"),			
		],
		""
)

def dummyConfigWithStrings() :
	return dict(
		ConfigParam1 = "Param1",
		ConfigParam2 = "Param2",
		ConfigParam3 = "Param3"
	)


