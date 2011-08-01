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
	DummyControlSink = dict(
		type = "DummyControlSink",
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
	DummyControlSource = dict(
		type = "DummyControlSource",
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
	),
	ProcessingWithNameSpacedPorts = dict(
		type = "ProcessingWithNameSpacedPorts",
		config = dict(),
		inports = [['An inport', 'DataType']],
		outports = [['An outport', 'DataType']],
		incontrols = [],
		outcontrols = []
	),
	ProcessingWithNameSpacedControls = dict(
		type = "ProcessingWithNameSpacedControls",
		config = dict(),
		inports = [],
		outports = [],
		incontrols = [['An incontrol', 'ControlType']],
		outcontrols = [['An outcontrol', 'ControlType']]
	),
	ProcessingWithPortsAndControls = dict(
		type = "ProcessingWithPortsAndControls",
		config = dict(),
		inports = [['InPort1', 'DataType'], ['InPort2', 'DataType'], ['InPort3', 'DataType']],
		outports = [['OutPort1', 'DataType'], ['OutPort2', 'DataType'], ['OutPort3', 'DataType'], ['OutPort4', 'DataType']],
		incontrols = [['InControl1', 'ControlType'], ['InControl2', 'ControlType'], ['InControl3', 'ControlType'], ['InControl4', 'ControlType']],
		outcontrols = [['OutControl1', 'ControlType'], ['OutControl2', 'ControlType'], ['OutControl3', 'ControlType']]
	),
	ProcessingWithNumericPorts = dict(
		type = "ProcessingWithNumericPorts",
		config = dict(),
		inports = [['1', 'DataType']],
		outports = [['1', 'DataType'], ['2', 'DataType']],
		incontrols = [],
		outcontrols = []
	),
	ProcessingWithNumericControls = dict(
		type = "ProcessingWithNumericControls",
		config = dict(),
		inports = [],
		outports = [],
		incontrols = [['1', 'ControlType'], ['2', 'ControlType']],
		outcontrols = [['1', 'ControlType']]
	),
	ProcessingWithConfig = dict(
		type = "ProcessingWithConfig",
		config = dict(
			ConfigParam1 = "default1",
			ConfigParam2 = "default2",
		),
		inports = [],
		outports = [],
		incontrols = [['1', 'ControlType'], ['2', 'ControlType']],
		outcontrols = [['1', 'ControlType']]
	),
	DummyProcessingWithStringConfiguration = dict(
		type = "DummyProcessingWithStringConfiguration",
		config = dict(
			AString = "DefaultValue",
			OtherString = "Another default value",
		),
		inports = [],
		outports = [],
		incontrols = [],
		outcontrols = []
	),
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
		import Dummy_ConfigurationProxy
		return Dummy_ConfigurationProxy.Dummy_ConfigurationProxy(self._processings[name]["config"])

	def processingRename(self, oldName, newName):
		if newName in self._processings.keys():
			raise KeyError("A processing named '%s' already exists"%newName)
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

	def deleteProcessing(self, processingName):
		del self._processings[processingName]



