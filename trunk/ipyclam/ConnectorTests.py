from Connector import *

import unittest
import TestFixtures

class ConnectorTests(unittest.TestCase):
	def empty(self):
		import Dummy_NetworkProxy
		return Dummy_NetworkProxy.Dummy_NetworkProxy()

	def fixture1(self):
		engine = self.empty()
		engine.addProcessing("DummyTypeProcessing1", "proc1")
		engine.addProcessing("DummyTypeProcessing2", "proc2")
		engine.connect("Port","proc1","OutPort1","proc2","Inport2")
		engine.connect("Port","proc2","Outport2","proc1","InPort2")
		engine.connect("Control","proc1","OutControl1","proc2","Incontrol2")
		engine.connect("Control","proc1","OutControl2","proc2","Incontrol2")
		engine.connect("Control","proc2","Outcontrol1","proc1","InControl2")
		return engine

	def test_name(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.name, "InPort1")
	def test_name_isReadOnly(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "name", 'InPort2')
	def test_kind_whenPort(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.kind, "Port")
	def test_kind_whenControl(self):
		control = Connector(self.fixture1(), "proc1", kind=Control, direction=In, name="InControl1")
		self.assertEqual(control.kind, "Control")
	def test_kind_isReadOnly(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "kind", Control)
	def test_direction_whenIn(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.direction, "In")
	def test_direction_whenOut(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=Out, name="InPort1")
		self.assertEqual(port.direction, "Out")
	def test_direction_isReadOnly(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "direction", Out)
	def test_index(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.index, 0)
	def test_index_isReadOnly(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "index", 2)
	def test_type(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=In, name="InPort1")
		self.assertEqual(port.type, "f")
	def test_type_isReadOnly(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=In, name="InPort1")
		self.assertRaises(AttributeError, setattr, port, "type", "tipus2")
	def test_host(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=In, name="OutPort1")
		self.assertEqual(port.host.name, "proc1")
		self.assertEqual(port.host.type, "DummyTypeProcessing1")
	# TODO: is host read only?
	def test_peers_forInPorts(self):
		port = Connector(self.fixture1(), "proc2", kind=Port, direction=In, name="Inport2")
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual(['OutPort1'], listPeers)
	def test_peers_forInPorts_whenEmpty(self):
		port = Connector(self.fixture1(), "proc2", kind=Port, direction=In, name="Inport1")
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual([], listPeers)
	def test_peers_forIncontrols_whenEmpty(self):
		port = Connector(self.fixture1(), "proc2", kind=Control, direction=In, name="Incontrol1")
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual([], listPeers)
	def test_peers_forOutPorts(self):
		port = Connector(self.fixture1(), "proc1", kind=Port, direction=Out, name="OutPort1")
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual(['Inport2'], listPeers)

	def peerList(self, connector) :
		return "".join((
			"{0}.{1}\n".format(peer.host.name, peer.name)
			for peer in connector.peers))

	def test_connectTwoPorts(self) :
		engine = self.fixture1()
		port = Connector(engine, "proc1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(engine, "proc2", kind=Port, direction=In, name="Inport1")
		port.connect(port2)
		self.assertMultiLineEqual(
			self.peerList(port),
			"proc2.Inport2\n"
			"proc2.Inport1\n"
			)
		self.assertMultiLineEqual(
			self.peerList(port2),
			"proc1.OutPort1\n"
			)

	def test_connectTwoPortsPassingInportFirst(self) :
		engine = self.fixture1()
		port = Connector(engine, "proc1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(engine, "proc2", kind=Port, direction=In, name="Inport1")
		port.connect(port2)
		self.assertMultiLineEqual(
			self.peerList(port),
			"proc2.Inport2\n"
			"proc2.Inport1\n"
			)
		self.assertMultiLineEqual(
			self.peerList(port2),
			"proc1.OutPort1\n"
			)

	def test_connectTwoPortsWithSameDirectionAndFail(self) :
		engine = self.fixture1()
		port = Connector(engine, "proc1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(engine, "proc2", kind=Port, direction=Out, name="Outport1")
		try:
			port.connect(port2)
		except SameConnectorDirection, e:
			self.assertEquals("Same direction: OutPort1 Outport1", e.__str__())
		else:
			self.fail("Exception expected")

	def	test_connectPortWithControlAndFail(self) :
		engine = self.fixture1()
		port = Connector(engine, "proc1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(engine, "proc2", kind=Control, direction=In, name="Incontrol1")
		try:
			port.connect(port2)
		except DifferentConnectorKind, e:
			self.assertEqual(e.message,
				"Different kind: OutPort1 Incontrol1")
		else :
			self.fail("Exception expected")

	def	test_connectControlWithDifferentTypeControlAndFail(self) :
		engine = self.fixture1()
		port = Connector(engine, "proc1", kind=Control, direction=Out, name="OutControl1")
		port2 = Connector(engine, "proc2", kind=Control, direction=In, name="Incontrol3")
		try:
			port.connect(port2)
			self.fail("Exception expected")
		except DifferentConnectorType, e:
			self.assertEqual(e.message,
				"Different type: OutControl1 Incontrol3")

	def test_connectWith__gt__Operator(self) :
		engine = self.fixture1()
		port = Connector(engine, "proc1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(engine, "proc2", kind=Port, direction=In, name="Inport1")
		port > port2
		self.assertMultiLineEqual(
			self.peerList(port),
			"proc2.Inport2\n"
			"proc2.Inport1\n"
			)
		self.assertMultiLineEqual(
			self.peerList(port2),
			"proc1.OutPort1\n"
			)

	def test_connectTwoTimesSamePortsAndFail(self) :
		engine = self.fixture1()
		port = Connector(engine, "proc1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(engine, "proc2", kind=Port, direction=In, name="Inport1")
		port > port2
		try:
			port > port2
			self.fail("Exception expected")
		except ConnectionExists, e:
			self.assertEquals("proc1.OutPort1 and proc2.Inport1 already connected", e.__str__())

	def test_connectInputOutputWith__gt__OperatorAndFail(self) :
		engine = self.fixture1()
		port = Connector(engine, "proc1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(engine, "proc2", kind=Port, direction=In, name="Inport1")
		try:
			port2 > port
			self.fail("Exception expected")
		except BadConnectorDirectionOrder, e:
			self.assertEquals("Wrong connectors order: Output > Input", e.__str__())

	def test_connectWith__lt__Operator(self) :
		engine = self.fixture1()
		port = Connector(engine, "proc1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(engine, "proc2", kind=Port, direction=In, name="Inport1")
		port2 < port
		listPeersPort = [ connector.name for connector in port.peers ]
		self.assertEqual(['Inport2', 'Inport1'], listPeersPort)
		listPeersPort2 = [ connector.name for connector in port2.peers ]
		self.assertEqual(['OutPort1'], listPeersPort2)

	def test_connectOutputInputWith__lt__OperatorAndFail(self) :
		engine = self.fixture1()
		port = Connector(engine, "proc1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(engine, "proc2", kind=Port, direction=In, name="Inport1")
		try:
			port < port2
			self.fail("Exception expected")
		except BadConnectorDirectionOrder, e:
			self.assertEquals("Wrong connectors order: Input < Output", e.__str__())

	def test_disconnect_ports(self):
		engine = self.fixture1()
		port = Connector(engine, "proc1", kind=Port, direction=Out, name="OutPort1")
		port2 = Connector(engine, "proc2", kind=Port, direction=In, name="Inport2")
		port.disconnect(port2)
		listPeers = [ connector.name for connector in port.peers ]
		self.assertEqual([], listPeers)

	def test_disconnect_controls(self):
		engine = self.fixture1()
		control = Connector(engine, "proc1", kind=Control, direction=Out, name="OutControl1")
		control2 = Connector(engine, "proc2", kind=Control, direction=In, name="Incontrol2")
		control.disconnect(control2)
		listPeers = [ connector.name for connector in control.peers ]
		self.assertEqual([], listPeers)

	def test_disconnectports_from_processing(self):
		engine = self.empty()
		engine.addProcessing("DummyPortSource", "source")
		engine.addProcessing("DummyPortSink", "sink")
		engine.addProcessing("DummyPortSink", "sink2")
		source = Connector(engine, "source", "Port", "Out", "OutPort1")
		sink = Connector(engine, "sink", "Port", "In", "InPort1")
		sink2 = Connector(engine, "sink2", "Port", "In", "InPort1")
		source > sink
		source > sink2
		self.assertMultiLineEqual(
			self.peerList(source),
			"sink.InPort1\n"
			"sink2.InPort1\n"
			)
		source.disconnect(sink.host)
		self.assertMultiLineEqual(
			self.peerList(source),
			"sink2.InPort1\n"
			)
		source.disconnect(sink2.host)
		self.assertMultiLineEqual(
			self.peerList(source),"")

	def test_disconnectcontrols_from_processing(self):
		engine = self.empty()
		engine.addProcessing("DummyControlSource", "source")
		engine.addProcessing("DummyControlSink", "sink")
		engine.addProcessing("DummyControlSink", "sink2")
		source = Connector(engine, "source", "Control", "Out", "OutControl1")
		sink = Connector(engine, "sink", "Control", "In", "InControl1")
		sink2 = Connector(engine, "sink2", "Control", "In", "InControl1")
		source > sink
		source > sink2
		self.assertMultiLineEqual(
			self.peerList(source),
			"sink.InControl1\n"
			"sink2.InControl1\n"
			)
		source.disconnect(sink.host)
		self.assertMultiLineEqual(
			self.peerList(source),
			"sink2.InControl1\n"
			)
		source.disconnect(sink2.host)
		self.assertMultiLineEqual(
			self.peerList(source),"")

	def test_disconnectallport_peers(self) :
		engine = self.empty()
		engine.addProcessing("DummyPortSource", "source")
		engine.addProcessing("DummyPortSink", "sink")
		engine.addProcessing("DummyPortSink", "sink2")
		source = Connector(engine, "source", "Port", "Out", "OutPort1")
		sink = Connector(engine, "sink", "Port", "In", "InPort1")
		sink2 = Connector(engine, "sink2", "Port", "In", "InPort1")
		source > sink
		source > sink2
		self.assertMultiLineEqual(
			self.peerList(source),
			"sink.InPort1\n"
			"sink2.InPort1\n"
			)
		source.disconnect()
		self.assertMultiLineEqual(self.peerList(source), "")

	def test_disconnectallcontrol_peers(self) :
		engine = self.empty()
		engine.addProcessing("DummyControlSource", "source")
		engine.addProcessing("DummyControlSink", "sink")
		engine.addProcessing("DummyControlSink", "sink2")
		source = Connector(engine, "source", "Control", "Out", "OutControl1")
		sink = Connector(engine, "sink", "Control", "In", "InControl1")
		sink2 = Connector(engine, "sink2", "Control", "In", "InControl1")
		source > sink
		source > sink2
		self.assertMultiLineEqual(
			self.peerList(source),
			"sink.InControl1\n"
			"sink2.InControl1\n"
			)
		source.disconnect()
		self.assertMultiLineEqual(self.peerList(source), "")

class Clam_ConnectorTests(ConnectorTests):
	def empty(self):
		import Clam_NetworkProxy
		return Clam_NetworkProxy.Clam_NetworkProxy()

if __name__ == '__main__':
	unittest.main()
