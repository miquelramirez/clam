import unittest
import Connector

class Dummy_NetworkProxy :
	def __init__(self, processings, portConnections, controlConnections) :
		self._processings = dict()
		for process in processings:
			self._processings[process['name']] = process
		self._portConnections = portConnections
		self._controlConnections = controlConnections
	def processingType(self, name) :
		return self._processings[name]["type"]
	def processingConfig(self, name) :
		return self._processings[name]["config"]
	def processingConnectors(self, name, kind, direction) :
		nameKinds = [
			(Connector.Port, Connector.In, 'inports'),
			(Connector.Port, Connector.Out, 'outports'),
			(Connector.Control, Connector.In, 'incontrols'),
			(Connector.Control, Connector.Out, 'outcontrols'),
		]
		mapping = dict([((k,d),n) for k,d,n in nameKinds])
		return self._processings[name][mapping[kind,direction]]
	def portConnections(self, name):
		connections = []
		for connection in self._portConnections:
			if connection[0] == name:
				connections.append(connection)
		return connections
	def controlConnections(self, name):
		connections = []
		for connection in self._controlConnections:
			if connection[0] == name:
				connections.append(connection)
		return connections


class Dummy_NetworkProxyTest(unittest.TestCase) :
	def definition(self) :
		return [
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
			("Processing1", "Incontrol1", "Processing2", "Incontrol2"),
			("Processing1", "Incontrol2", "Processing2", "Incontrol2"),
			("Processing2", "Incontrol1", "Processing1", "Incontrol2"),			
		]
	]

	def test_type(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals("AudioSource", proxy.processingType("Processing1"))

	def test_config(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals(dict(
				ConfigParam1 = "default1",
				ConfigParam2 = "default2",
			), proxy.processingConfig("Processing1"))

	def test_inports(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				["Inport1", "type1"],
				["Inport2", "type1"]
			], proxy.processingConnectors(
				"Processing1", Connector.Port, Connector.In))

	def test_outports(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				["Outport1", "type1"],
				["Outport2", "type1"]
			], proxy.processingConnectors(
				"Processing1", Connector.Port, Connector.Out))

	def test_incontrols(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				["Incontrol1", "type2"],
				["Incontrol2", "type2"]
			], proxy.processingConnectors(
				"Processing1", Connector.Control, Connector.In))

	def test_outcontrols(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				["Outcontrol1", "type2"],
				["Outcontrol2", "type2"]
			], proxy.processingConnectors(
				"Processing1", Connector.Control, Connector.Out))

	def test_withBadProcessingName(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertRaises(KeyError, proxy.processingType, "Processing3")

	def test_typeSecondProcessing(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals("AudioSink", proxy.processingType("Processing2"))
	
	def test_portConnections(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
			("Processing1", "Outport1", "Processing2", "Inport2"),
			("Processing1", "Outport2", "Processing2", "Inport2"),
		], proxy.portConnections("Processing1"))
		self.assertEquals([
			("Processing2", "Outport1", "Processing1", "Inport2"),
		], proxy.portConnections("Processing2"))
	
	def test_controlConnections(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
			("Processing1", "Incontrol1", "Processing2", "Incontrol2"),
			("Processing1", "Incontrol2", "Processing2", "Incontrol2"),
		], proxy.controlConnections("Processing1"))
		self.assertEquals([
			("Processing2", "Incontrol1", "Processing1", "Incontrol2"),
		], proxy.controlConnections("Processing2"))


if __name__ == '__main__':
	unittest.main()

