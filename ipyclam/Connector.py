In = "In"
Out = "Out"
Port = "Port"
Control = "Control"

class ConnectionExists(Exception):
	pass

class BadConnectorDirectionOrder(Exception):
	pass

class DifferentConnectorKind(Exception):
	pass

class SameConnectorDirection(Exception):
	pass

class DifferentConnectorType(Exception):
	pass

class Connector(object):
	def __init__(self, networkProxy, host, kind=Port, direction=In, name = "Inport1"):
		self._proxy = networkProxy
		self.__dict__["host"] = host
		self.__dict__["name"] = name
		self.__dict__["kind"] = kind
		self.__dict__["direction"] = direction

	def _hostname(self) :
		"Private helper to get the host name"
		return self.__dict__["host"]

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
		return self._proxy.connectorIndex(self._hostname(), self.kind, self.direction, self.name)

	@property
	def type(self):
		"""The type of the port"""
		return self._proxy.connectorType(self._hostname(), self.kind, self.direction, self.name)

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
		return PeerConnectors(self._proxy, self._hostname(), self.kind, self.direction, self.name)

	def connect(self, peer):
		if self.direction == peer.direction :
			raise SameConnectorDirection("Same direction: %s %s"%(self.name, peer.name))
		if self.kind != peer.kind :
			raise DifferentConnectorKind("Different kind: %s %s"%(self.name, peer.name))
		if self.type != peer.type :
			raise DifferentConnectorType("Different type: %s %s"%(self.name, peer.name))
		if self._proxy.connectionExists(self.kind, self._hostname(), self.name, peer._hostname(), peer.name):
			raise ConnectionExists("%s.%s and %s.%s already connected"%(self._hostname(), self.name, peer._hostname(), peer.name))
		if self.direction == Out :
			self._proxy.connect(self.kind, self._hostname(), self.name, peer._hostname(), peer.name)
		else :
			self._proxy.connect(self.kind, peer._hostname(), peer.name, self._hostname(), self.name)

	def __gt__(self, peer) :
		if self.direction == In and peer.direction == Out:
			raise BadConnectorDirectionOrder("Wrong connectors order: Output > Input")
		self.connect(peer)

	def __lt__(self, peer) :
		if self.direction == Out and peer.direction == In:
			raise BadConnectorDirectionOrder("Wrong connectors order: Input < Output")
		self.connect(peer)

	def disconnect(self, peer = None):
		if peer == None:
			self.disconnectFromAllPeers()
			return
		import Processing
		if type(peer) == Processing.Processing:
			self.disconnectProcessing(peer)
			return
		self._proxy.disconnect(self.kind, self._hostname(), self.name, peer._hostname(), peer.name)

	def disconnectProcessing(self, peer):
		names = (self._hostname(), self.name)
		if self.kind == Port:
			connections = [connection for connection in self._proxy.portConnections()]
			for connection in connections:
				if (connection[0:2] == names) & (peer.name == connection[2]):
					self._proxy.disconnect(self.kind, self._hostname(), self.name, peer.name, connection[3])
		else:
			connections = [connection for connection in self._proxy.controlConnections()]
			for connection in connections:
				if (connection[0:2] == names) & (peer.name == connection[2]):
					self._proxy.disconnect(self.kind, self._hostname(), self.name, peer.name, connection[3])

	def disconnectFromAllPeers(self):
		names = (self._hostname(), self.name)
		if self.kind == Port:
			connections = [connection for connection in self._proxy.portConnections()]
			for connection in connections:
				if (connection[0:2] == names):
					self._proxy.disconnect(self.kind, self._hostname(), self.name, connection[2], connection[3])
		if self.kind == Control:
			connections = [connection for connection in self._proxy.controlConnections()]
			for connection in connections:
				if (connection[0:2] == names):
					self._proxy.disconnect(self.kind, self._hostname(), self.name, connection[2], connection[3])

import unittest
import TestFixtures

class ConnectorTests(unittest.TestCase):
	def test_name(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.name, "InPort1")
	def test_name_isReadOnly(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "name", 'InPort2')
	def test_kind_whenPort(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.kind, "Port")
	def test_kind_whenControl(self):
		control = Connector(TestFixtures.proxy(), "Processing1", kind=Control, direction=In, name="InControl1")
		self.assertEqual(control.kind, "Control")
	def test_kind_isReadOnly(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "kind", Control)
	def test_direction_whenIn(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.direction, "In")
	def test_direction_whenOut(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=Out, name="InPort1")
		self.assertEqual(port.direction, "Out")
	def test_direction_isReadOnly(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "direction", Out)
	def test_index(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.index, 0)
	def test_index_isReadOnly(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "index", 2)
	def test_type(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.type, "type1")
	def test_type_isReadOnly(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "type", "tipus2")
	def test_host(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=In, name="OutPort1")
		self.assertEqual(port.host.name, "Processing1")
		self.assertEqual(port.host.type, "DummyTypeProcessing1")
	# TODO: is host read only?
	def test_peers_forInPorts(self):
		port = Connector(TestFixtures.proxy(), "Processing2", kind=Port, direction=In, name="Inport2")
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual(['OutPort1', 'OutPort2'], listPeers)
	def test_peers_forInPorts_whenEmpty(self):
		port = Connector(TestFixtures.proxy(), "Processing2", kind=Port, direction=In, name="Inport1")
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual([], listPeers)
	def test_peers_forOutPorts(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=Out, name="OutPort1")
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual(['Inport2'], listPeers)
	def test_connectTwoPorts(self) :
		proxy = TestFixtures.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		port.connect(port2)
		listPeersPort = [ connector.name for connector in port.peers ]
		self.assertEqual(['Inport2', 'Inport1'], listPeersPort)
		listPeersPort2 = [ connector.name for connector in port2.peers ]
		self.assertEqual(['OutPort1'], listPeersPort2)
	def test_connectTwoPortsPassingInportFirst(self) :
		proxy = TestFixtures.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		port2.connect(port)
		listPeersPort = [ connector.name for connector in port.peers ]
		self.assertEqual(['Inport2', 'Inport1'], listPeersPort)
		listPeersPort2 = [ connector.name for connector in port2.peers ]
		self.assertEqual(['OutPort1'], listPeersPort2)
	def test_connectTwoPortsWithSameDirectionAndFail(self) :
		proxy = TestFixtures.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=Out, name="Outport1")
		try:
			port.connect(port2)
			self.fail("Exception expected")
		except SameConnectorDirection, e:
			self.assertEquals("Same direction: OutPort1 Outport1", e.__str__())
	def	test_connectPortWithControlAndFail(self) :
		proxy = TestFixtures.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Control, direction=In, name="Incontrol1")
		try:
			port.connect(port2)
			self.fail("Exception expected")
		except DifferentConnectorKind, e:
			self.assertEquals("Different kind: OutPort1 Incontrol1", e.__str__())
	def	test_connectControlWithDifferentTypeControlAndFail(self) :
		proxy = TestFixtures.proxy()
		port = Connector(proxy, "Processing1", kind=Control, direction=Out, name="OutControl1")
		port2 = Connector(proxy, "Processing2", kind=Control, direction=In, name="Incontrol3")
		try:
			port.connect(port2)
			self.fail("Exception expected")
		except DifferentConnectorType, e:
			self.assertEquals("Different type: OutControl1 Incontrol3", e.__str__())

	def test_connectWith__gt__Operator(self) :
		proxy = TestFixtures.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		port > port2
		listPeersPort = [ connector.name for connector in port.peers ]
		self.assertEqual(['Inport2', 'Inport1'], listPeersPort)
		listPeersPort2 = [ connector.name for connector in port2.peers ]
		self.assertEqual(['OutPort1'], listPeersPort2)

	def test_connectTwoTimesSamePortsAndFail(self) :
		proxy = TestFixtures.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		port > port2
		try:
			port > port2
			self.fail("Exception expected")
		except ConnectionExists, e:
			self.assertEquals("Processing1.OutPort1 and Processing2.Inport1 already connected", e.__str__())

	def test_connectInputOutputWith__gt__OperatorAndFail(self) :
		proxy = TestFixtures.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		try:
			port2 > port
			self.fail("Exception expected")
		except BadConnectorDirectionOrder, e:
			self.assertEquals("Wrong connectors order: Output > Input", e.__str__())

	def test_connectWith__lt__Operator(self) :
		proxy = TestFixtures.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		port2 < port
		listPeersPort = [ connector.name for connector in port.peers ]
		self.assertEqual(['Inport2', 'Inport1'], listPeersPort)
		listPeersPort2 = [ connector.name for connector in port2.peers ]
		self.assertEqual(['OutPort1'], listPeersPort2)

	def test_connectOutputInputWith__lt__OperatorAndFail(self) :
		proxy = TestFixtures.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		try:
			port < port2
			self.fail("Exception expected")
		except BadConnectorDirectionOrder, e:
			self.assertEquals("Wrong connectors order: Input < Output", e.__str__())

	def test_disconnect_ports(self):
		port = Connector(TestFixtures.proxy(), "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(TestFixtures.proxy(), "Processing2", kind=Port, direction=In, name="Inport2")
		port.disconnect(port2)
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual([], listPeers)

	def test_disconnect_controls(self):
		control = Connector(TestFixtures.proxy(), "Processing1", kind=Control, direction=Out, name="OutControl1")
		control2 = Connector(TestFixtures.proxy(), "Processing2", kind=Control, direction=In, name="Incontrol2")
		control.disconnect(control2)
		listPeers = [ connector.name for connector in control.peers ]
		self.assertEqual([], listPeers)

	def test_disconnectports_from_processing(self):
		import Network
		net = Network.Network(TestFixtures.empty())
		net.source = "PortSource"
		net.sink = "SeveralInPortsProcessing"
		net.source.OutPort1 > net.sink.InPort1
		net.source.OutPort1 > net.sink.InPort2
		net.source.OutPort1 > net.sink.InPort3
		net.source.OutPort1 > net.sink.InPort4
		listPeers = [ connector.name for connector in net.source.OutPort1.peers ]
		self.assertEqual(['InPort1', 'InPort2', 'InPort3', 'InPort4'], listPeers)
		net.source.OutPort1.disconnect(net.sink)
		listPeers = [ connector.name for connector in net.source.OutPort1.peers ]
		self.assertEqual([], listPeers)

	def test_disconnectcontrols_from_processing(self):
		import Network
		net = Network.Network(TestFixtures.empty())
		net.source = "ControlSource"
		net.sink = "SeveralInControlsProcessing"
		net.source.OutControl1 > net.sink.InControl1
		net.source.OutControl1 > net.sink.InControl2
		net.source.OutControl1 > net.sink.InControl3
		net.source.OutControl1 > net.sink.InControl4
		listPeers = [ connector.name for connector in net.source.OutControl1.peers ]
		self.assertEqual(['InControl1', 'InControl2', 'InControl3', 'InControl4'], listPeers)
		net.source.OutControl1.disconnect(net.sink)
		listPeers = [ connector.name for connector in net.source.OutControl1.peers ]
		self.assertEqual([], listPeers)

	def test_disconnectallport_peers(self) :
		import Network
		net = Network.Network(TestFixtures.empty())
		net.Processing1 = "PortSource"
		net.Processing2 = "PortSink"
		net.Processing3 = "SeveralInPortsProcessing"
		net.Processing1.OutPort1 > net.Processing2.InPort1
		net.Processing1.OutPort1 > net.Processing3.InPort2
		net.Processing1.OutPort1 > net.Processing3.InPort3
		net.Processing1.OutPort1 > net.Processing3.InPort4
		listPeers = [ connector.name for connector in net.Processing1.OutPort1.peers ]
		self.assertEqual(['InPort1', 'InPort2', 'InPort3', 'InPort4'], listPeers)
		net.Processing1.OutPort1.disconnect()
		listPeers = [ connector.name for connector in net.Processing1.OutPort1.peers ]
		self.assertEqual([], listPeers)

	def test_disconnectallport_peers(self) :
		import Network
		net = Network.Network(TestFixtures.empty())
		net.Processing1 = "ControlSource"
		net.Processing2 = "ControlSink"
		net.Processing3 = "SeveralInControlsProcessing"
		net.Processing1.OutControl1 > net.Processing2.InControl1
		net.Processing1.OutControl1 > net.Processing3.InControl2
		net.Processing1.OutControl1 > net.Processing3.InControl3
		net.Processing1.OutControl1 > net.Processing3.InControl4
		listPeers = [ connector.name for connector in net.Processing1.OutControl1.peers ]
		self.assertEqual(['InControl1', 'InControl2', 'InControl3', 'InControl4'], listPeers)
		net.Processing1.OutControl1.disconnect()
		listPeers = [ connector.name for connector in net.Processing1.OutControl1.peers ]
		self.assertEqual([], listPeers)

if __name__ == '__main__':
	unittest.main()
