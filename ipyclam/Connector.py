import unittest
import definition

Port = "Port"
Control = "Control"
In = "In"
Out = "Out"

class Connector(object):
	Port = "Port"
	Control = "Control"
	In = "In"
	Out = "Out"
	def __init__(self, networkProxy, processingName, name = "Inport1", kind=Port, direction=In, index=1, type=None):
		self.__dict__["processingName"] = processingName
		self.__dict__["name"] = name
		self.__dict__["kind"] = kind
		self.__dict__["direction"] = direction
		self.__dict__["index"] = index
		self.__dict__["type"] = type
		self.__dict__["peers"] = []
		if kind == Port:
			for connection in networkProxy.portConnections(processingName):
				if connection[1] == name:
					self.__dict__["peers"].append(connection)
		if kind == Control:
			for connection in networkProxy.controlConnections(processingName):
				if connection[1] == name:
					self.__dict__["peers"].append(connection)
	@property
	def name(self):
		"""The name of the port"""
		return self.__dict__["name"]
	@property
	def kind(self):
		"""The kind of the port"""
		return self.__dict__["kind"]
	@property
	def direction(self):
		"""The direction of the port"""
		return self.__dict__["direction"]
	@property
	def index(self):
		"""The index of the port"""
		return self.__dict__["index"]
	@property
	def type(self):
		"""The type of the port"""
		return self.__dict__["type"]
	@property
	def peers(self):
		"""A list of all the Connectors connected to the Connector"""
		return self.__dict__["peers"]

class ConnectorTests(unittest.TestCase):
	def test_gettingName(self):
		port = Connector(definition.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=2, type="type1")
		self.assertEqual(port.name, "Inport1")
	def test_settingNameAndFailing(self):
		port = Connector(definition.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=2, type="type1")
		self.assertRaises(AttributeError, setattr, port, "name", 'Inport2')
	def test_gettingKindPort(self):
		port = Connector(definition.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=2, type="type1")
		self.assertEqual(port.kind, "Port")
	def test_gettingKindControl(self):
		port = Connector(definition.proxy(), "Processing1", name="Inport1", kind=Control, direction=In, index=2, type="type1")
		self.assertEqual(port.kind, "Control")
	def test_settingKindAndFailing(self):
		port = Connector(definition.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=2, type="type1")
		self.assertRaises(AttributeError, setattr, port, "kind", Connector.Control)
	def test_gettingDirectionIn(self):
		port = Connector(definition.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=2, type="type1")
		self.assertEqual(port.direction, "In")
	def test_gettingDirectionOut(self):
		port = Connector(definition.proxy(), "Processing1", name="Inport1", kind=Port, direction=Out, index=2, type="type1")
		self.assertEqual(port.direction, "Out")
	def test_settingDirectionAndFailing(self):
		port = Connector(definition.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=2, type="type1")
		self.assertRaises(AttributeError, setattr, port, "direction", Connector.Out)
	def test_gettingIndex(self):
		port = Connector(definition.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=1, type="type1")
		self.assertEqual(port.index, 1)
	def test_settingIndexAndFailing(self):
		port = Connector(definition.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=1, type="type1")
		self.assertRaises(AttributeError, setattr, port, "index", 2)
	def test_gettingType(self):
		port = Connector(definition.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=1, type="type1")
		self.assertEqual(port.type, "type1")
	def test_settingTypeAndFailing(self):
		port = Connector(definition.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=1, type="type1")
		self.assertRaises(AttributeError, setattr, port, "type", "tipus2")
	def test_peers(self):
		port = Connector(definition.proxy(), "Processing1", name="Outport1", kind=Port, direction=In, index=1, type="type1")
		self.assertEqual([
							('Processing1', 'Outport1', 'Processing2', 'Inport2'),
						], port.peers)

if __name__ == '__main__':
	unittest.main()
