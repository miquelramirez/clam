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
	),
	OtherControlSink = dict(
		type = "OtherControlSink",
		config = dict(),
		inports = [],
		outports = [],
		incontrols = [['InControl1', 'OtherControlType']],
		outcontrols = []
	),
	SeveralInPortsProcessing = dict(
		type = "SeveralInPortsProcessing",
		config = dict(),
		inports = [['InPort1', 'DataType'], ['InPort2', 'DataType'], ['InPort3', 'DataType'], ['InPort4', 'DataType']],
		outports = [],
		incontrols = [],
		outcontrols = []
	),
	SeveralInControlsProcessing = dict(
		type = "SeveralInControlsProcessing",
		config = dict(),
		inports = [],
		outports = [],
		incontrols = [['InControl1', 'ControlType'], ['InControl2', 'ControlType'], ['InControl3', 'ControlType'], ['InControl4', 'ControlType']],
		outcontrols = []
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

	def __init__(self, processings=[], portConnections=[], controlConnections=[], description="", types=_dummyPrototypes) :
		self._processings = dict()
		for process in processings:
			self._processings[process['name']] = process
		self._portConnections = portConnections
		self._controlConnections = controlConnections
		self._types = types
		self._description = description

	def processingType(self, name) :
		return self._processings[name]["type"]

	def processingConfig(self, name) :
		return self._processings[name]["config"]

	def processingRename(self, oldName, newName):
		if newName in self._processings.keys():
			raise KeyError(newName + " is already taken")
		self._processings[newName] = self._processings[oldName]
		del self._processings[oldName]

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

	def processingNames(self) :
		return [ processingName for processingName in self._processings]

	def addProcessing(self, type, name) :
		if self.hasProcessing(name):
			raise BadProcessingName(name, "Name repeated")
		if type not in self._types.keys():
			raise BadProcessingType(type)
		self._processings[name] = self._types[type]

	def processingHasConnector(self, processingName, kind, direction, connectorName):
		return connectorName in self.processingConnectors(processingName, kind, direction)

	def areConnectable(self, kind, fromProcessing, fromConnector, toProcessing, toConnector) :
		return self.connectorType(fromProcessing, kind, Connector.Out, fromConnector) == self.connectorType(toProcessing, kind, Connector.In, toConnector)

	def connect(self, kind, fromProcessing, fromConnector, toProcessing, toConnector) :
		assert self.hasProcessing(fromProcessing), "%s does not exist" %(fromProcessing)
		assert self.hasProcessing(toProcessing), "%s does not exist" %(toProcessing)
		assert self.processingHasConnector(fromProcessing, kind, Connector.Out, fromConnector), "%s does not have connector %s"%(fromProcessing, fromConnector)
		assert self.processingHasConnector(toProcessing, kind, Connector.In, toConnector), "%s does not have connector %s"%(toProcessing, toConnector)
		assert self.areConnectable(kind, fromProcessing, fromConnector, toProcessing, toConnector), "%s and %s have incompatible types"%(fromConnector, toConnector)
		assert not self.connectionExists(kind, fromProcessing, fromConnector, toProcessing, toConnector), "%s.%s and %s.%s already connected"%(fromProcessing, fromConnector, toProcessing, toConnector)
		connections = self._controlConnections if kind == Connector.Control else self._portConnections
		connections.append((fromProcessing, fromConnector, toProcessing, toConnector))

	def portConnections(self) :
		return self._portConnections

	def controlConnections(self) :
		return self._controlConnections

	def availableTypes(self) :
		return self._types.keys()

	def connectionExists(self, kind, fromProcessing, fromConnector, toProcessing, toConnector) :
		connections = self._controlConnections if kind == Connector.Control else self._portConnections
		return (fromProcessing, fromConnector, toProcessing, toConnector) in connections

	def disconnect(self, kind, fromProcessing, fromConnector, toProcessing, toConnector) :
		if kind == Connector.Port:
			self._portConnections.remove((fromProcessing, fromConnector, toProcessing, toConnector))
		else:
			self._controlConnections.remove((fromProcessing, fromConnector, toProcessing, toConnector))

	def getDescription(self):
		return self._description

	def setDescription(self, description):
		self._description = description

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
			("Processing1", "OutControl1", "Processing2", "Incontrol2"),
			("Processing1", "OutControl2", "Processing2", "Incontrol2"),
			("Processing2", "Outcontrol1", "Processing1", "InControl2"),
		],
		""
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
		listProcessings = [ processName for processName in proxy.processingNames() ]
		self.assertEquals(['Processing1', 'Processing2'], listProcessings)

	def test_hasProcessing_whenItIsThere(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertTrue(proxy.hasProcessing('Processing1'))

	def test_hasProcessing_whenItIsNotThere(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertFalse(proxy.hasProcessing('NotExisting'))
	
	def test_processingsName_empty(self):
		proxy = Dummy_NetworkProxy()
		self.assertEquals([], proxy.processingNames())

	def test_addProcessing_withOneMinimalProcessing(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("MinimalProcessing", "AProcessing")
		self.assertEquals(['AProcessing'], proxy.processingNames())
		self.assertEquals('MinimalProcessing', proxy.processingType('AProcessing'))
		self.assertEquals({}, proxy.processingConfig('AProcessing'))
		self.assertEquals([], proxy.processingConnectors('AProcessing', Connector.Port, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AProcessing', Connector.Port, Connector.Out))
		self.assertEquals([], proxy.processingConnectors('AProcessing', Connector.Control, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AProcessing', Connector.Control, Connector.Out))

	def test_addProcessing_withInPort(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("PortSink", "APortSink")
		self.assertEquals(['APortSink'], proxy.processingNames())
		self.assertEquals('PortSink', proxy.processingType('APortSink'))
		self.assertEquals({}, proxy.processingConfig('APortSink'))
		self.assertEquals(["InPort1"], proxy.processingConnectors('APortSink', Connector.Port, Connector.In))
		self.assertEquals([], proxy.processingConnectors('APortSink', Connector.Port, Connector.Out))
		self.assertEquals([], proxy.processingConnectors('APortSink', Connector.Control, Connector.In))
		self.assertEquals([], proxy.processingConnectors('APortSink', Connector.Control, Connector.Out))

	def test_addProcessing_withInControl(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSink", "AControlSink")
		self.assertEquals(['AControlSink'], proxy.processingNames())
		self.assertEquals('ControlSink', proxy.processingType('AControlSink'))
		self.assertEquals({}, proxy.processingConfig('AControlSink'))
		self.assertEquals([], proxy.processingConnectors('AControlSink', Connector.Port, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AControlSink', Connector.Port, Connector.Out))
		self.assertEquals(["InControl1"], proxy.processingConnectors('AControlSink', Connector.Control, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AControlSink', Connector.Control, Connector.Out))

	def test_addProcessing_withOutPort(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("PortSource", "APortSource")
		self.assertEquals(['APortSource'], proxy.processingNames())
		self.assertEquals('PortSource', proxy.processingType('APortSource'))
		self.assertEquals({}, proxy.processingConfig('APortSource'))
		self.assertEquals([], proxy.processingConnectors('APortSource', Connector.Port, Connector.In))
		self.assertEquals(["OutPort1"], proxy.processingConnectors('APortSource', Connector.Port, Connector.Out))
		self.assertEquals([], proxy.processingConnectors('APortSource', Connector.Control, Connector.In))
		self.assertEquals([], proxy.processingConnectors('APortSource', Connector.Control, Connector.Out))

	def test_addProcessing_withInControl(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "AControlSource")
		self.assertEquals(['AControlSource'], proxy.processingNames())
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

	def test_addProcessing_withBadType(self) :
		proxy = Dummy_NetworkProxy()
		try:
			proxy.addProcessing("ABadType", "Processing1")
			self.fail("Exception expected")
		except BadProcessingType, e:
			self.assertEquals("'BadProcessingType(ABadType)'", e.__str__())

	def test_connect_ports(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("PortSource", "Processing1")
		proxy.addProcessing("PortSink", "Processing2")
		proxy.connect(Connector.Port, "Processing1", "OutPort1", "Processing2", "InPort1")
		self.assertEquals([
				("Processing2", "InPort1")]
				, proxy.connectorPeers("Processing1", Connector.Port, Connector.Out, "OutPort1"))

	def test_connect_controls(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		proxy.addProcessing("ControlSink", "Processing2")
		proxy.connect(Connector.Control, "Processing1", "OutControl1", "Processing2", "InControl1")
		self.assertEquals([
				("Processing2", "InControl1")]
				, proxy.connectorPeers("Processing1", Connector.Control, Connector.Out, "OutControl1"))

	def test_connect_missingControl(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		try:
			proxy.connect(Connector.Control, "Processing1", "OutControl1", "NonExistingProcessing", "InControl1")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(("NonExistingProcessing does not exist", ), e.args)

	def test_connect_missingPort(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		proxy.addProcessing("ControlSink", "ProcessingWithNoIncontrol2")
		try:
			proxy.connect(Connector.Control, "Processing1", "OutControl1", "ProcessingWithNoIncontrol2", "InControl2")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(("ProcessingWithNoIncontrol2 does not have connector InControl2", ), e.args)

	def test_connect_connectorsWithDifferentTypes(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		proxy.addProcessing("OtherControlSink", "ProcessingWithOtherControlType")
		try:
			proxy.connect(Connector.Control, "Processing1", "OutControl1", "ProcessingWithOtherControlType", "InControl1")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(("OutControl1 and InControl1 have incompatible types", ), e.args)

	def test_portConnections(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
			("Processing1", "OutPort1", "Processing2", "Inport2"),
			("Processing1", "OutPort2", "Processing2", "Inport2"),
			("Processing2", "Outport2", "Processing1", "InPort2"),
		], proxy.portConnections())

	def test_controlConnections(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
			("Processing1", "OutControl1", "Processing2", "Incontrol2"),
			("Processing1", "OutControl2", "Processing2", "Incontrol2"),
			("Processing2", "Outcontrol1", "Processing1", "InControl2"),			
		], proxy.controlConnections())

	def test_availableTypes(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
			'PortSource',
			'PortSink',
			'ControlSource', 
			'OtherControlSink',
			'MinimalProcessing',
			'SeveralInPortsProcessing',
			'ControlSink',
			].sort(), proxy.availableTypes().sort())

	def test_areConnectable(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		proxy.addProcessing("ControlSink", "Processing2")
		self.assertTrue(proxy.areConnectable(Connector.Control, "Processing1", "OutControl1", "Processing2", "InControl1"))
		proxy.addProcessing("OtherControlSink", "ProcessingWithOtherControlType")
		self.assertFalse(proxy.areConnectable(Connector.Control, "Processing1", "OutControl1", "ProcessingWithOtherControlType", "InControl1"))

	def test_processingHasConnector(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		self.assertTrue(proxy.processingHasConnector("Processing1", Connector.Control, Connector.Out, "OutControl1"))
		self.assertFalse(proxy.processingHasConnector("Processing1", Connector.Control, Connector.In, "InControl1"))

	def test_connectionExists_withControls_whenDoesNotExists(self) :
		proxy = Dummy_NetworkProxy()
		self.assertFalse(proxy.connectionExists(Connector.Control, "Processing1", "OutControl1", "Processing2", "InControl2"))

	def test_connectionExists_withControls_whenExists(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		proxy.addProcessing("ControlSink", "Processing2")
		self.assertTrue(proxy.connectionExists(Connector.Control, "Processing1", "OutControl1", "Processing2", "InControl1"))

	def test_connectionExists_withPorts_whenDoesNotExists(self) :
		proxy = Dummy_NetworkProxy()
		self.assertFalse(proxy.connectionExists(Connector.Port, "Processing3", "OutPort1", "Processing4", "InPort1"))

	def test_connectionExists_withPorts_whenExists(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("PortSource", "Processing3")
		proxy.addProcessing("PortSink", "Processing4")
		proxy.connect(Connector.Port, "Processing3", "OutPort1", "Processing4", "InPort1")
		self.assertTrue(proxy.connectionExists(Connector.Port, "Processing3", "OutPort1", "Processing4", "InPort1"))
		self.assertFalse(proxy.connectionExists(Connector.Port, "Processing3", "OutPort1", "Processing4", "InPort2"))

	def test_connectWhenAlreadyConnected(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("PortSource", "Processing5")
		proxy.addProcessing("PortSink", "Processing6")
		proxy.connect(Connector.Port, "Processing5", "OutPort1", "Processing6", "InPort1")
		try:
			proxy.connect(Connector.Port, "Processing5", "OutPort1", "Processing6", "InPort1")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(("Processing5.OutPort1 and Processing6.InPort1 already connected", ), e.args)


	def test_disconnect_ports(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		proxy.disconnect(Connector.Port, "Processing1", "OutPort1", "Processing2", "Inport2")
		self.assertEquals([]
				, proxy.connectorPeers("Processing1", Connector.Port, Connector.Out, "OutPort1"))

	def test_disconnect_controls(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		proxy.disconnect(Connector.Control, "Processing1", "OutControl1", "Processing2", "Incontrol2")
		self.assertEquals([]
				, proxy.connectorPeers("Processing1", Connector.Control, Connector.Out, "OutControl1"))

	def test_processingRenaming(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		proxy.processingRename("Processing1", "ProcessingRenamed")
		self.assertFalse(proxy.hasProcessing("Processing1"))
		self.assertTrue(proxy.hasProcessing("ProcessingRenamed"))

	def test_processingRenamingWithExistingNameFails(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("PortSource", "ExistingName")
		proxy.addProcessing("PortSink", "ProcessingToRename")
		# TOFIX: KeyError should be used just when a non existing key, and args should be the kay
		try:
			proxy.processingRename("ProcessingToRename", "ExistingName")
		except KeyError, e:
			self.assertEquals(("ExistingName is already taken", ), e.args)

	def test_getNetworkDescriptionEmpty(self):
		proxy = Dummy_NetworkProxy()
		self.assertEquals("", proxy.getDescription())

	def test_setNetworkDescription(self):
		proxy = Dummy_NetworkProxy()
		proxy.setDescription("A description")
		self.assertEquals("A description", proxy.getDescription())

if __name__ == '__main__':
	unittest.main()


