In = "In"
Out = "Out"
Port = "Port"
Control = "Control"

class Connector(object):
	In = In
	Out = Out
	Port = Port
	Control = Control
	def __init__(self, networkProxy, host, name = "Inport1", kind=Port, direction=In, index=1, type=None):
		self._proxy = networkProxy
		self.__dict__["host"] = host
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
	@property
	def host(self):
		import Processing
		return Processing.Processing(
			self.__dict__["host"],
			self._proxy,
			)
	@property
	def peers(self):
		from PeerConnectors import PeerConnectors
		if self.__dict__["direction"] == Connector.In:
			direction = Connector.Out
		else:
			direction = Connector.In
		return PeerConnectors(self.__dict__["host"], self.__dict__["name"], self.__dict__["kind"], direction, self._proxy)

import unittest
import TestFixtures

class ConnectorTests(unittest.TestCase):
	def test_gettingName(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=2, type="type1")
		self.assertEqual(port.name, "Inport1")
	def test_settingNameAndFailing(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=2, type="type1")
		self.assertRaises(AttributeError, setattr, port, "name", 'Inport2')
	def test_gettingKindPort(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=2, type="type1")
		self.assertEqual(port.kind, "Port")
	def test_gettingKindControl(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Inport1", kind=Control, direction=In, index=2, type="type1")
		self.assertEqual(port.kind, "Control")
	def test_settingKindAndFailing(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=2, type="type1")
		self.assertRaises(AttributeError, setattr, port, "kind", Connector.Control)
	def test_gettingDirectionIn(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=2, type="type1")
		self.assertEqual(port.direction, "In")
	def test_gettingDirectionOut(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Inport1", kind=Port, direction=Out, index=2, type="type1")
		self.assertEqual(port.direction, "Out")
	def test_settingDirectionAndFailing(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=2, type="type1")
		self.assertRaises(AttributeError, setattr, port, "direction", Connector.Out)
	def test_gettingIndex(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=1, type="type1")
		self.assertEqual(port.index, 1)
	def test_settingIndexAndFailing(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=1, type="type1")
		self.assertRaises(AttributeError, setattr, port, "index", 2)
	def test_gettingType(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=1, type="type1")
		self.assertEqual(port.type, "type1")
	def test_settingTypeAndFailing(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Inport1", kind=Port, direction=In, index=1, type="type1")
		self.assertRaises(AttributeError, setattr, port, "type", "tipus2")
	def test_hostNameAndType(self):
		port = Connector(TestFixtures.proxy(), "Processing1", name="Outport1", kind=Port, direction=In, index=1, type="type1")
		self.assertEqual(port.host.name, "Processing1")
		self.assertEqual(port.host.type, "AudioSource")
	def test_peers(self):
		port = Connector(TestFixtures.proxy(), "Processing2", name="Inport2", kind=Port, direction=In, index=1, type="type1")
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual(['Outport1', 'Outport2'], listPeers)

if __name__ == '__main__':
	unittest.main()
