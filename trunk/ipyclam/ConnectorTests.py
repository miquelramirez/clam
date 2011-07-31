from Connector import *

import unittest
import TestFixtures

class ConnectorTests(unittest.TestCase):
	def proxy(self):
		return TestFixtures.proxy()
	def empty(self):
		return TestFixtures.empty()

	def test_name(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.name, "InPort1")
	def test_name_isReadOnly(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "name", 'InPort2')
	def test_kind_whenPort(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.kind, "Port")
	def test_kind_whenControl(self):
		control = Connector(self.proxy(), "Processing1", kind=Control, direction=In, name="InControl1")
		self.assertEqual(control.kind, "Control")
	def test_kind_isReadOnly(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "kind", Control)
	def test_direction_whenIn(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.direction, "In")
	def test_direction_whenOut(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=Out, name="InPort1")
		self.assertEqual(port.direction, "Out")
	def test_direction_isReadOnly(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "direction", Out)
	def test_index(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.index, 0)
	def test_index_isReadOnly(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "index", 2)
	def test_type(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.type, "f")
	def test_type_isReadOnly(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "type", "tipus2")
	def test_host(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=In, name="OutPort1")
		self.assertEqual(port.host.name, "Processing1")
		self.assertEqual(port.host.type, "DummyTypeProcessing1")
	# TODO: is host read only?
	def test_peers_forInPorts(self):
		port = Connector(self.proxy(), "Processing2", kind=Port, direction=In, name="Inport2")
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual(['OutPort1'], listPeers)
	def test_peers_forInPorts_whenEmpty(self):
		port = Connector(self.proxy(), "Processing2", kind=Port, direction=In, name="Inport1")
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual([], listPeers)
	def test_peers_forIncontrols_whenEmpty(self):
		port = Connector(self.proxy(), "Processing2", kind=Control, direction=In, name="Incontrol1")
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual([], listPeers)
	def test_peers_forOutPorts(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=Out, name="OutPort1")
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual(['Inport2'], listPeers)
	def test_connectTwoPorts(self) :
		proxy = self.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		port.connect(port2)
		listPeersPort = [ connector.name for connector in port.peers ]
		self.assertEqual(['Inport2', 'Inport1'], listPeersPort)
		listPeersPort2 = [ connector.name for connector in port2.peers ]
		self.assertEqual(['OutPort1'], listPeersPort2)
	def test_connectTwoPortsPassingInportFirst(self) :
		proxy = self.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		port.connect(port2)
		listPeersPort = [ connector.name for connector in port.peers ]
		self.assertEqual(['Inport2', 'Inport1'], listPeersPort)
		listPeersPort2 = [ connector.name for connector in port2.peers ]
		self.assertEqual(['OutPort1'], listPeersPort2)
	def test_connectTwoPortsWithSameDirectionAndFail(self) :
		proxy = self.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=Out, name="Outport1")
		try:
			port.connect(port2)
			self.fail("Exception expected")
		except SameConnectorDirection, e:
			self.assertEquals("Same direction: OutPort1 Outport1", e.__str__())
	def	test_connectPortWithControlAndFail(self) :
		proxy = self.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Control, direction=In, name="Incontrol1")
		try:
			port.connect(port2)
			self.fail("Exception expected")
		except DifferentConnectorKind, e:
			self.assertEquals("Different kind: OutPort1 Incontrol1", e.__str__())
	def	test_connectControlWithDifferentTypeControlAndFail(self) :
		proxy = self.proxy()
		port = Connector(proxy, "Processing1", kind=Control, direction=Out, name="OutControl1")
		port2 = Connector(proxy, "Processing2", kind=Control, direction=In, name="Incontrol3")
		try:
			port.connect(port2)
			self.fail("Exception expected")
		except DifferentConnectorType, e:
			self.assertEquals("Different type: OutControl1 Incontrol3", e.__str__())

	def test_connectWith__gt__Operator(self) :
		proxy = self.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		port > port2
		listPeersPort = [ connector.name for connector in port.peers ]
		self.assertEqual(['Inport2', 'Inport1'], listPeersPort)
		listPeersPort2 = [ connector.name for connector in port2.peers ]
		self.assertEqual(['OutPort1'], listPeersPort2)

	def test_connectTwoTimesSamePortsAndFail(self) :
		proxy = self.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		port > port2
		try:
			port > port2
			self.fail("Exception expected")
		except ConnectionExists, e:
			self.assertEquals("Processing1.OutPort1 and Processing2.Inport1 already connected", e.__str__())

	def test_connectInputOutputWith__gt__OperatorAndFail(self) :
		proxy = self.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		try:
			port2 > port
			self.fail("Exception expected")
		except BadConnectorDirectionOrder, e:
			self.assertEquals("Wrong connectors order: Output > Input", e.__str__())

	def test_connectWith__lt__Operator(self) :
		proxy = self.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		port2 < port
		listPeersPort = [ connector.name for connector in port.peers ]
		self.assertEqual(['Inport2', 'Inport1'], listPeersPort)
		listPeersPort2 = [ connector.name for connector in port2.peers ]
		self.assertEqual(['OutPort1'], listPeersPort2)

	def test_connectOutputInputWith__lt__OperatorAndFail(self) :
		proxy = self.proxy()
		port = Connector(proxy, "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(proxy, "Processing2", kind=Port, direction=In, name="Inport1")
		try:
			port < port2
			self.fail("Exception expected")
		except BadConnectorDirectionOrder, e:
			self.assertEquals("Wrong connectors order: Input < Output", e.__str__())

	def test_disconnect_ports(self):
		port = Connector(self.proxy(), "Processing1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(self.proxy(), "Processing2", kind=Port, direction=In, name="Inport2")
		port.disconnect(port2)
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual([], listPeers)

	def test_disconnect_controls(self):
		control = Connector(self.proxy(), "Processing1", kind=Control, direction=Out, name="OutControl1")
		control2 = Connector(self.proxy(), "Processing2", kind=Control, direction=In, name="Incontrol2")
		control.disconnect(control2)
		listPeers = [ connector.name for connector in control.peers ]
		self.assertEqual([], listPeers)

	def test_disconnectports_from_processing(self):
		import Network
		net = Network.Network(self.empty())
		net.source = "PortSource"
		net.sink = "PortSink"
		net.sink2 = "PortSink"
		net.source.OutPort1 > net.sink.InPort1
		net.source.OutPort1 > net.sink2.InPort1
		listPeers = [ connector.name for connector in net.source.OutPort1.peers ]
		self.assertEqual(['InPort1', 'InPort1'], listPeers)
		net.source.OutPort1.disconnect(net.sink)
		listPeers = [ connector.name for connector in net.source.OutPort1.peers ]
		self.assertEqual(['InPort1'], listPeers)
		net.source.OutPort1.disconnect(net.sink2)
		listPeers = [ connector.name for connector in net.source.OutPort1.peers ]
		self.assertEqual([], listPeers)

	def test_disconnectcontrols_from_processing(self):
		import Network
		net = Network.Network(self.empty())
		net.source = "DummyControlSource"
		net.sink = "DummyControlSink"
		net.sink2 = "DummyControlSink"
		net.source.OutControl1 > net.sink.InControl1
		net.source.OutControl1 > net.sink2.InControl1
		listPeers = [ connector.name for connector in net.source.OutControl1.peers ]
		self.assertEqual(['InControl1', 'InControl1'], listPeers)
		net.source.OutControl1.disconnect(net.sink)
		listPeers = [ connector.name for connector in net.source.OutControl1.peers ]
		self.assertEqual(['InControl1'], listPeers)
		net.source.OutControl1.disconnect(net.sink2)
		listPeers = [ connector.name for connector in net.source.OutControl1.peers ]
		self.assertEqual([], listPeers)

	def test_disconnectallport_peers(self) :
		import Network
		net = Network.Network(self.empty())
		net.Processing1 = "PortSource"
		net.Processing2 = "PortSink"
		net.Processing3 = "PortSink"
		net.Processing1.OutPort1 > net.Processing2.InPort1
		net.Processing1.OutPort1 > net.Processing3.InPort1
		listPeers = [ (connector.host.name, connector.name) for connector in net.Processing1.OutPort1.peers ]
		self.assertEqual([('Processing2', 'InPort1'), ('Processing3', 'InPort1')], listPeers)
		net.Processing1.OutPort1.disconnect()
		listPeers = [ connector.name for connector in net.Processing1.OutPort1.peers ]
		self.assertEqual([], listPeers)

	def test_disconnectallcontrol_peers(self) :
		import Network
		net = Network.Network(self.empty())
		net.Processing1 = "DummyControlSource"
		net.Processing2 = "DummyControlSink"
		net.Processing3 = "DummyControlSink"
		net.Processing1.OutControl1 > net.Processing2.InControl1
		net.Processing1.OutControl1 > net.Processing3.InControl1
		listPeers = [ (connector.host.name, connector.name) for connector in net.Processing1.OutControl1.peers ]
		self.assertEqual([('Processing2', 'InControl1'), ('Processing3', 'InControl1')], listPeers)
		net.Processing1.OutControl1.disconnect()
		listPeers = [ connector.name for connector in net.Processing1.OutControl1.peers ]
		self.assertEqual([], listPeers)

class Clam_ConnectorTests(ConnectorTests):
	def proxy(self):
		import Clam_NetworkProxy
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		network = Network.Network(proxy)
		network.Processing1 = "DummyTypeProcessing1"
		network.Processing2 = "DummyTypeProcessing2"
		network.Processing1.OutPort1 > network.Processing2.Inport2
		network.Processing2.Outport2 > network.Processing1.InPort2
		network.Processing1.OutControl1 > network.Processing2.Incontrol2
		network.Processing1.OutControl2 > network.Processing2.Incontrol2
		network.Processing2.Outcontrol1 > network.Processing1.InControl2
		return proxy

	def empty(self):
		import Clam_NetworkProxy
		return Clam_NetworkProxy.Clam_NetworkProxy()

if __name__ == '__main__':
	unittest.main()
