import unittest

Port = "Port"
Control = "Control"
In = "In"
Out = "Out"

class Connector(object):
	Port = "Port"
	Control = "Control"
	In = "In"
	Out = "Out"
	def __init__(self, type, name = "inport1", kind=Port, direction=In, index=1):
		self.__dict__["name"] = name
		self.__dict__["kind"] = kind
		self.__dict__["direction"] = direction
		self.__dict__["index"] = index
		self.__dict__["type"] = type
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

class ConnectorTests(unittest.TestCase):
	def setUp(self):
		pass
	def test_gettingName(self):
		port = Connector(
			name="inport1", 
			kind=Connector.Port,
			direction=Connector.In,
			index=2,
			type="tipus1"
			)
		self.assertEqual(port.name, "inport1")
	def test_settingNameAndFailing(self):
		port = Connector(
			name="inport1", 
			kind=Connector.Port,
			direction=Connector.In,
			index=2,
			type="tipus1"
			)
		self.assertRaises(AttributeError, setattr, port, "name", 'inport2')
	def test_gettingKindPort(self):
		port = Connector(name="inport1", kind=Connector.Port, direction=Connector.In, index=2, type="tipus1")
		self.assertEqual(port.kind, "Port")
	def test_gettingKindControl(self):
		port = Connector(name="inport1", kind=Connector.Control, direction=Connector.In, index=2, type="tipus1")
		self.assertEqual(port.kind, "Control")
	def test_settingKindAndFailing(self):
		port = Connector(name="inport1", kind=Connector.Port, direction=Connector.In, index=2, type="tipus1")
		self.assertRaises(AttributeError, setattr, port, "kind", Connector.Control)
	def test_gettingDirectionIn(self):
		port = Connector(name="inport1", kind=Connector.Port, direction=Connector.In, index=2, type="tipus1")
		self.assertEqual(port.direction, "In")
	def test_gettingDirectionOut(self):
		port = Connector(name="inport1", kind=Connector.Port, direction=Connector.Out, index=2, type="tipus1")
		self.assertEqual(port.direction, "Out")
	def test_settingDirectionAndFailing(self):
		port = Connector(name="inport1", kind=Connector.Port, direction=Connector.In, index=2, type="tipus1")
		self.assertRaises(AttributeError, setattr, port, "direction", Connector.Out)
	def test_gettingIndex(self):
		port = Connector(name="inport1", kind=Connector.Port, direction=Connector.In, index=1, type="tipus1")
		self.assertEqual(port.index, 1)
	def test_settingIndexAndFailing(self):
		port = Connector(name="inport1", kind=Connector.Port, direction=Connector.In, index=1, type="tipus1")
		self.assertRaises(AttributeError, setattr, port, "index", 2)
	def test_gettingType(self):
		port = Connector(name="inport1", kind=Connector.Port, direction=Connector.In, index=1, type="tipus1")
		self.assertEqual(port.type, "tipus1")
	def test_settingTypeAndFailing(self):
		port = Connector(name="inport1", kind=Connector.Port, direction=Connector.In, index=1, type="tipus1")
		self.assertRaises(AttributeError, setattr, port, "type", "tipus2")


if __name__ == '__main__':
	unittest.main()
