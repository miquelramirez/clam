import Dummy_NetworkProxy

def empty() :
	return Dummy_NetworkProxy.Dummy_NetworkProxy([], [], [])

def proxy() :
	return Dummy_NetworkProxy.Dummy_NetworkProxy(
		[
			dict(
				type = "DummyTypeProcessing1",
				name = "Processing1",
				config = dict(
					ConfigParam1 = "default1",
					ConfigParam2 = "default2",
				),
				inports = [
					["InPort1", "f"],
					["InPort2", "f"],
					['InPort3', 'f'],
					['InPort4', 'f'],
				],
				outports = [
					["OutPort1", "f"],
					["OutPort2", "f"]
				],
				incontrols = [
					["InControl1", "f"],
					["InControl2", "f"]
				],
				outcontrols = [
					["OutControl1", "f"],
					["OutControl2", "f"]
				],
			),
			dict(
				type = "DummyTypeProcessing2",
				name = "Processing2",
				config = dict(
					ConfigParam1 = "default1",
					ConfigParam2 = "default2",
				),
				inports = [
					["Inport1", "f"],
					["Inport2", "f"]
				],
				outports = [
					["Outport1", "f"],
					["Outport2", "f"]
				],
				incontrols = [
					["Incontrol1", "f"],
					["Incontrol2", "f"],
					["Incontrol3", "i"]
				],
				outcontrols = [
					["Outcontrol1", "f"],
					["Outcontrol2", "f"]
				],
			),
		],
		[
			("Processing1", "OutPort1", "Processing2", "Inport2"),
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

def dummyConfigWithNestedConfigs() :
	return dict(
		ConfigParam1 = "default1",
		ConfigParam2 = "default2",
		ConfigParam3 = dict(
			NestedParam1 = "defaultnested1",
			NestedParam2 = "defaultnested2"
		)
	)
