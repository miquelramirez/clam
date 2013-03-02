from Dummy_Engine import Dummy_Engine
import Connector

import unittest

class Dummy_EngineTest(unittest.TestCase) :

	def definition(self) :
		return [
		[
			dict(
				type = "DummyPortSource",
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
				type = "DummyPortSink",
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
		engine = Dummy_Engine(*self.definition())
		self.assertEquals("DummyPortSource", engine.processingType("Processing1"))

	def test_inports(self) :
		engine = Dummy_Engine(*self.definition())
		self.assertEquals([
				"InPort1",
				"InPort2",
			], engine.processingConnectors(
				"Processing1", Connector.Port, Connector.In))

	def test_outports(self) :
		engine = Dummy_Engine(*self.definition())
		self.assertEquals([
				"OutPort1",
				"OutPort2",
			], engine.processingConnectors(
				"Processing1", Connector.Port, Connector.Out))

	def test_incontrols(self) :
		engine = Dummy_Engine(*self.definition())
		self.assertEquals([
				"InControl1",
				"InControl2",
			], engine.processingConnectors(
				"Processing1", Connector.Control, Connector.In))

	def test_outcontrols(self) :
		engine = Dummy_Engine(*self.definition())
		self.assertEquals([
				"OutControl1",
				"OutControl2",
			], engine.processingConnectors(
				"Processing1", Connector.Control, Connector.Out))

	def test_procensignType_secondProcessing(self) :
		engine = Dummy_Engine(*self.definition())
		self.assertEquals("DummyPortSink", engine.processingType("Processing2"))

	def test_portConnectionsOut(self) :
		engine = Dummy_Engine(*self.definition())
		self.assertEquals([
				("Processing1", "OutPort1"),
				("Processing1", "OutPort2")]
				, engine.connectorPeers("Processing2", Connector.Port, Connector.In, "Inport2"))
	
	def test_portConnectionsIn(self) :
		engine = Dummy_Engine(*self.definition())
		self.assertEquals([
				("Processing2", "Inport2")]
				, engine.connectorPeers("Processing1", Connector.Port, Connector.Out, "OutPort1"))

	def test_connectorType(self) :
		engine = Dummy_Engine(*self.definition())
		type = engine.connectorType("Processing1", Connector.Port, Connector.In, "InPort1")
		self.assertEquals("type1", type)

	def test_connectorInfo(self) :
		engine = Dummy_Engine(*self.definition())
		index = engine.connectorIndex("Processing1", Connector.Port, Connector.In, "InPort1")
		self.assertEquals(0, index)

	def test_processingsName(self) :
		engine = Dummy_Engine(*self.definition())
		listProcessings = [ processName for processName in engine.processingNames() ]
		self.assertEquals(['Processing1', 'Processing2'], listProcessings)

	def test_portConnections(self) :
		engine = Dummy_Engine(*self.definition())
		self.assertEquals([
			("Processing1", "OutPort1", "Processing2", "Inport2"),
			("Processing1", "OutPort2", "Processing2", "Inport2"),
			("Processing2", "Outport2", "Processing1", "InPort2"),
		], engine.portConnections())

	def test_controlConnections(self) :
		engine = Dummy_Engine(*self.definition())
		self.assertEquals([
			("Processing1", "OutControl1", "Processing2", "Incontrol2"),
			("Processing1", "OutControl2", "Processing2", "Incontrol2"),
			("Processing2", "Outcontrol1", "Processing1", "InControl2"),
		], engine.controlConnections())

	def test_config(self) :
		engine = Dummy_Engine(*self.definition())
		self.assertEquals("default1", engine.processingConfig("Processing1")["ConfigParam1"] )
		self.assertEquals("default2", engine.processingConfig("Processing1")["ConfigParam2"] )

if __name__ == '__main__':
	unittest.main()


