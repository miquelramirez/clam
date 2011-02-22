import Connector

_connectorKindNames = [
	(Connector.Port, Connector.In, 'inports'),
	(Connector.Port, Connector.Out, 'outports'),
	(Connector.Control, Connector.In, 'incontrols'),
	(Connector.Control, Connector.Out, 'outcontrols'),
	]
_kind2Name = dict([((k,d),n) for k,d,n in _connectorKindNames])

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
		connectorKindName = _kind2Name[(kind,direction)]
		return self._processings[name][connectorKindName]

	def connectorPeers(self, processingName, portName, kind, direction):
		connections = self._portConnections if kind == Connector.Port else self._controlConnections
		if direction == Connector.Out :
			return [x[0:2] for x in connections if (processingName, portName)==x[2:4] ]
		else :
			return [x[2:4] for x in connections if (processingName, portName)==x[0:2] ]

	def connectorInfo(self, processingName, connectorName, kind, direction) :
		connectorKindName = _kind2Name[(kind,direction)]
		for i, (name, type) in enumerate(self._processings[processingName][connectorKindName]):
			if name == connectorName:
				return (i, type)

	def connectorIndex(self, processingName, connectorName, kind, direction) :
		connectorKindName = _kind2Name[(kind,direction)]
		for i, (name, type) in enumerate(self._processings[processingName][connectorKindName]):
			if name == connectorName: return i

	def connectorType(self, processingName, connectorName, kind, direction) :
		connectorKindName = _kind2Name[(kind,direction)]
		for name, type in self._processings[processingName][connectorKindName]:
			if name == connectorName: return type

	def hasProcessing(self, processingName) :
		return processingName in self._processings

	def processingsName(self) :
		for processingName in self._processings:
			yield processingName

import unittest

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
					["InPort1", "type1"],
					["InPort2", "type1"]
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
					["Incontrol2", "type2"]
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
			("Processing1", "InControl1", "Processing2", "Incontrol2"),
			("Processing1", "InControl2", "Processing2", "Incontrol2"),
			("Processing2", "Incontrol1", "Processing1", "InControl2"),			
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
				["InPort1", "type1"],
				["InPort2", "type1"]
			], proxy.processingConnectors(
				"Processing1", Connector.Port, Connector.In))

	def test_outports(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				["OutPort1", "type1"],
				["OutPort2", "type1"]
			], proxy.processingConnectors(
				"Processing1", Connector.Port, Connector.Out))

	def test_incontrols(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				["InControl1", "type2"],
				["InControl2", "type2"]
			], proxy.processingConnectors(
				"Processing1", Connector.Control, Connector.In))

	def test_outcontrols(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				["OutControl1", "type2"],
				["OutControl2", "type2"]
			], proxy.processingConnectors(
				"Processing1", Connector.Control, Connector.Out))

	def test_withBadProcessingName(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertRaises(KeyError, proxy.processingType, "Processing3")

	def test_typeSecondProcessing(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals("AudioSink", proxy.processingType("Processing2"))

	def test_portConnectionsOut(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				("Processing1", "OutPort1"),
				("Processing1", "OutPort2")]
				, proxy.connectorPeers("Processing2", "Inport2", Connector.Port, Connector.Out))
	
	def test_portConnectionsIn(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				("Processing2", "Inport2")]
				, proxy.connectorPeers("Processing1", "OutPort1", Connector.Port, Connector.In))

	def test_connectorInfo(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		index, type = proxy.connectorInfo("Processing1", "InPort1", Connector.Port, Connector.In)
		self.assertEquals(0, index)
		self.assertEquals("type1", type)

	def test_connectorType(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		type = proxy.connectorType("Processing1", "InPort1", Connector.Port, Connector.In)
		self.assertEquals("type1", type)

	def test_connectorInfo(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		index = proxy.connectorIndex("Processing1", "InPort1", Connector.Port, Connector.In)
		self.assertEquals(0, index)

	def test_processingsName(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		listProcessings = [ processName for processName in proxy.processingsName() ]
		self.assertEquals(['Processing1', 'Processing2'], listProcessings)

	def test_hasProcessing_whenItIsThere(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertTrue(proxy.hasProcessing('Processing1'))

	def test_hasProcessing_whenItIsNotThere(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertFalse(proxy.hasProcessing('NotExisting'))

if __name__ == '__main__':
	unittest.main()

