import Connector

_connectorKindNames = [
	(Connector.Port, Connector.In, 'inports'),
	(Connector.Port, Connector.Out, 'outports'),
	(Connector.Control, Connector.In, 'incontrols'),
	(Connector.Control, Connector.Out, 'outcontrols'),
	]
_kind2Name = dict([((k,d),n) for k,d,n in _connectorKindNames])

_dummyPrototypes = dict(
	MinimalProcessing = dict(
		type = "MinimalProcessing",
		config = dict(),
		inports = [],
		outports = [],
		incontrols = [],
		outcontrols = []
	),
	PortSink = dict(
		type = "PortSink",
		config = dict(),
		inports = [['InPort1', 'DataType']],
		outports = [],
		incontrols = [],
		outcontrols = []
	),
	PortSource = dict(
		type = "PortSource",
		config = dict(),
		inports = [],
		outports = [['OutPort1', 'DataType']],
		incontrols = [],
		outcontrols = []
	),
	ControlSink = dict(
		type = "ControlSink",
		config = dict(),
		inports = [],
		outports = [],
		incontrols = [['InControl1', 'ControlType']],
		outcontrols = []
	),
	ControlSource = dict(
		type = "ControlSource",
		config = dict(),
		inports = [],
		outports = [],
		incontrols = [],
		outcontrols = [['OutControl1', 'ControlType']]
	)
)

class BadProcessingName(Exception):
	def __init__(self, name, reason):
		self.name = name
		self.reason = reason
	def __str__(self):
		return repr(self.name + ': ' + self.reason)

class BadProcessingType(Exception):
	def __init__(self, type):
		self.type = type
	def __str__(self):
		return repr("BadProcessingType(" + self.type + ")")


class Dummy_NetworkProxy :

	def __init__(self, processings=[], portConnections=[], controlConnections=[]) :
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
		return [name for name,type in self._processings[name][connectorKindName]]

	def connectorPeers(self, processingName, kind, direction, portName):
		connections = self._portConnections if kind == Connector.Port else self._controlConnections
		if direction == Connector.In :
			return [x[0:2] for x in connections if (processingName, portName)==x[2:4] ]
		else :
			return [x[2:4] for x in connections if (processingName, portName)==x[0:2] ]

	def connectorIndex(self, processingName, kind, direction, connectorName) :
		connectorKindName = _kind2Name[(kind,direction)]
		for i, (name, type) in enumerate(self._processings[processingName][connectorKindName]):
			if name == connectorName: return i

	def connectorType(self, processingName, kind, direction, connectorName) :
		connectorKindName = _kind2Name[(kind,direction)]
		for name, type in self._processings[processingName][connectorKindName]:
			if name == connectorName: return type

	def hasProcessing(self, processingName) :
		return processingName in self._processings

	def processingsName(self) :
		return [ processingName for processingName in self._processings]

	def addProcessing(self, type, name) :
		if self.hasProcessing(name):
			raise BadProcessingName(name, "Name repeated")
		if type not in _dummyPrototypes.keys():
			raise BadProcessingType(type)
		self._processings[name] = _dummyPrototypes[type]

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
				"InPort1",
				"InPort2",
			], proxy.processingConnectors(
				"Processing1", Connector.Port, Connector.In))

	def test_outports(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				"OutPort1",
				"OutPort2",
			], proxy.processingConnectors(
				"Processing1", Connector.Port, Connector.Out))

	def test_incontrols(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				"InControl1",
				"InControl2",
			], proxy.processingConnectors(
				"Processing1", Connector.Control, Connector.In))

	def test_outcontrols(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				"OutControl1",
				"OutControl2",
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
				, proxy.connectorPeers("Processing2", Connector.Port, Connector.In, "Inport2"))
	
	def test_portConnectionsIn(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				("Processing2", "Inport2")]
				, proxy.connectorPeers("Processing1", Connector.Port, Connector.Out, "OutPort1"))

	def test_connectorType(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		type = proxy.connectorType("Processing1", Connector.Port, Connector.In, "InPort1")
		self.assertEquals("type1", type)

	def test_connectorInfo(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		index = proxy.connectorIndex("Processing1", Connector.Port, Connector.In, "InPort1")
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
	
	def test_processingsName_empty(self):
		proxy = Dummy_NetworkProxy()
		self.assertEquals([], proxy.processingsName())

	def test_addProcessing_withOneMinimalProcessing(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("MinimalProcessing", "AProcessing")
		self.assertEquals(['AProcessing'], proxy.processingsName())
		self.assertEquals('MinimalProcessing', proxy.processingType('AProcessing'))
		self.assertEquals({}, proxy.processingConfig('AProcessing'))
		self.assertEquals([], proxy.processingConnectors('AProcessing', Connector.Port, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AProcessing', Connector.Port, Connector.Out))
		self.assertEquals([], proxy.processingConnectors('AProcessing', Connector.Control, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AProcessing', Connector.Control, Connector.Out))

	def test_addProcessing_withInPort(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("PortSink", "APortSink")
		self.assertEquals(['APortSink'], proxy.processingsName())
		self.assertEquals('PortSink', proxy.processingType('APortSink'))
		self.assertEquals({}, proxy.processingConfig('APortSink'))
		self.assertEquals(["InPort1"], proxy.processingConnectors('APortSink', Connector.Port, Connector.In))
		self.assertEquals([], proxy.processingConnectors('APortSink', Connector.Port, Connector.Out))
		self.assertEquals([], proxy.processingConnectors('APortSink', Connector.Control, Connector.In))
		self.assertEquals([], proxy.processingConnectors('APortSink', Connector.Control, Connector.Out))

	def test_addProcessing_withInControl(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSink", "AControlSink")
		self.assertEquals(['AControlSink'], proxy.processingsName())
		self.assertEquals('ControlSink', proxy.processingType('AControlSink'))
		self.assertEquals({}, proxy.processingConfig('AControlSink'))
		self.assertEquals([], proxy.processingConnectors('AControlSink', Connector.Port, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AControlSink', Connector.Port, Connector.Out))
		self.assertEquals(["InControl1"], proxy.processingConnectors('AControlSink', Connector.Control, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AControlSink', Connector.Control, Connector.Out))

	def test_addProcessing_withOutPort(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("PortSource", "APortSource")
		self.assertEquals(['APortSource'], proxy.processingsName())
		self.assertEquals('PortSource', proxy.processingType('APortSource'))
		self.assertEquals({}, proxy.processingConfig('APortSource'))
		self.assertEquals([], proxy.processingConnectors('APortSource', Connector.Port, Connector.In))
		self.assertEquals(["OutPort1"], proxy.processingConnectors('APortSource', Connector.Port, Connector.Out))
		self.assertEquals([], proxy.processingConnectors('APortSource', Connector.Control, Connector.In))
		self.assertEquals([], proxy.processingConnectors('APortSource', Connector.Control, Connector.Out))

	def test_addProcessing_withInControl(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "AControlSource")
		self.assertEquals(['AControlSource'], proxy.processingsName())
		self.assertEquals('ControlSource', proxy.processingType('AControlSource'))
		self.assertEquals({}, proxy.processingConfig('AControlSource'))
		self.assertEquals([], proxy.processingConnectors('AControlSource', Connector.Port, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AControlSource', Connector.Port, Connector.Out))
		self.assertEquals([], proxy.processingConnectors('AControlSource', Connector.Control, Connector.In))
		self.assertEquals(["OutControl1"], proxy.processingConnectors('AControlSource', Connector.Control, Connector.Out))

	def test_addProcessing_withNameAlreadyAdded(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "ARepeatedName")
		try:
			proxy.addProcessing("ControlSource", "ARepeatedName")
			self.fail("Exception expected")
		except BadProcessingName, e:
			self.assertEquals("'ARepeatedName: Name repeated'", e.__str__())


	def test_addProcessing_withNameAlreadyAdded(self) :
		proxy = Dummy_NetworkProxy()
		try:
			proxy.addProcessing("ABadType", "Processing1")
			self.fail("Exception expected")
		except BadProcessingType, e:
			self.assertEquals("'BadProcessingType(ABadType)'", e.__str__())


if __name__ == '__main__':
	unittest.main()

