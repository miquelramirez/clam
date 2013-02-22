from Processing import Processing
from Connector import BadConnectorDirectionOrder

import unittest
class ProcessingTests(object):
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


	def test_type(self) :
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p.type, "DummyTypeProcessing1")

	def test_typeIsReadOnly(self) :
		p = Processing("proc1", self.fixture1())
		try :
			p.type="AnotherType"
			self.fail("Exception expected")
		except AttributeError, e :
			self.assertEqual("Attribute 'type' is read only", e.args[0])

	def test_name(self) :
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p.name, "proc1")

	def test_ConfigGettingAsDictionary(self):
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p["ConfigParam1"], "default1")

	def test_ConfigSettingAndGettingAsDictionary(self):
		p = Processing("proc1", self.fixture1())
		p["ConfigParam1"] = 'newvalue'
		self.assertEqual(p["ConfigParam1"], "newvalue")

	def test_configGettingAsAttribute(self):
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p.ConfigParam1, "default1")

	def test_configSettingAndGettingAsAttribute(self):
		p = Processing("proc1", self.fixture1())
		p.ConfigParam1 = 'newvalue'
		self.assertEqual(p.ConfigParam1, "newvalue")

	def test_ConfigGettersBy_configInterface(self):
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p._config.ConfigParam1, "default1")
		self.assertEqual(p._config["ConfigParam2"], "default2")

	def test_ConfigSettersBy_configInterface(self):
		p = Processing("proc1", self.fixture1())
		p._config.ConfigParam1 = "newvalue1"
		p._config["ConfigParam2"] = "newvalue2"
		self.assertEqual(p._config.ConfigParam1, "newvalue1")
		self.assertEqual(p._config["ConfigParam2"], "newvalue2")

	def test_InportsGettingByNameAsDictionary(self):
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p["InPort1"].name, "InPort1")
		self.assertEqual(p["InPort1"].kind, "Port")
		self.assertEqual(p["InPort1"].direction, "In")

	def test_InportsGettingByNameAsAttribute(self):
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p.InPort2.name, "InPort2")
		self.assertEqual(p.InPort2.kind, "Port")
		self.assertEqual(p.InPort2.direction, "In")

	def test_OutportsGettingByNameAsDictionary(self):
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p["OutPort1"].name, "OutPort1")
		self.assertEqual(p["OutPort1"].kind, "Port")
		self.assertEqual(p["OutPort1"].direction, "Out")

	def test_OutportsGettingByNameAsAttribute(self):
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p.OutPort2.name, "OutPort2")
		self.assertEqual(p.OutPort2.kind, "Port")
		self.assertEqual(p.OutPort2.direction, "Out")

	def test_IncontrolsGettingByNameAsDictionary(self):
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p["InControl1"].name, "InControl1")
		self.assertEqual(p["InControl1"].kind, "Control")
		self.assertEqual(p["InControl1"].direction, "In")

	def test_IncontrolsGettingByNameAsAttribute(self):
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p.InControl2.name, "InControl2")
		self.assertEqual(p.InControl2.kind, "Control")
		self.assertEqual(p.InControl2.direction, "In")

	def test_OutcontrolsGettingByNameAsDictionary(self):
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p["OutControl1"].name, "OutControl1")
		self.assertEqual(p["OutControl1"].kind, "Control")
		self.assertEqual(p["OutControl1"].direction, "Out")

	def test_OutcontrolsGettingByNameAsAttribute(self):
		p = Processing("proc1", self.fixture1())
		self.assertEqual(p.OutControl2.name, "OutControl2")
		self.assertEqual(p.OutControl2.kind, "Control")
		self.assertEqual(p.OutControl2.direction, "Out")

	def test_processingRenaming(self):
		p = Processing("proc1", self.fixture1())
		p.name = "ProcessingRenamed"
		self.assertEqual(p.name, "ProcessingRenamed")

	def test_dirFunction(self):
		p = Processing("proc1", self.fixture1())
		self.assertEqual(sorted([
			'name', 'type', 'connect',
			'_config', '_inports', '_outports', '_incontrols', '_outcontrols',
			'ConfigParam1', 'ConfigParam2', 
			'InPort1', 'InPort2','InPort3','InPort4',
			'OutPort1', 'OutPort2',
			'InControl1', 'InControl2',
			'OutControl1', 'OutControl2',
			]), dir(p))

	def connectivityFixture(self) :
		engine = self.empty()
		engine.addProcessing("ProcessingWithPortsAndControls", "multi1")
		engine.addProcessing("ProcessingWithPortsAndControls", "multi2")
		engine.addProcessing("DummyControlSink", "csink")
		engine.addProcessing("DummyControlSource", "csource")
		engine.addProcessing("DummyPortSink", "psink")
		engine.addProcessing("DummyPortSource", "psource")
		return engine

	def test_connect_to_processing(self):
		engine = self.connectivityFixture()
		p1 = Processing("multi1", engine)
		p2 = Processing("multi2", engine)
		p1 > p2
		self.assertEqual([
			("multi1", "OutControl1", "multi2", "InControl1"),
			("multi1", "OutControl2", "multi2", "InControl2"),
			("multi1", "OutControl3", "multi2", "InControl3"),
			], engine.controlConnections())
		self.assertEqual([
			("multi1", "OutPort1", "multi2", "InPort1"),
			("multi1", "OutPort2", "multi2", "InPort2"),
			("multi1", "OutPort3", "multi2", "InPort3"),
			], engine.portConnections())

	def test_connect_from_processing(self):
		engine = self.connectivityFixture()
		p1 = Processing("multi1", engine)
		p2 = Processing("multi2", engine)
		p2 < p1
		self.assertEqual([
			("multi1", "OutControl1", "multi2", "InControl1"),
			("multi1", "OutControl2", "multi2", "InControl2"),
			("multi1", "OutControl3", "multi2", "InControl3"),
			], engine.controlConnections())
		self.assertEqual([
			("multi1", "OutPort1", "multi2", "InPort1"),
			("multi1", "OutPort2", "multi2", "InPort2"),
			("multi1", "OutPort3", "multi2", "InPort3"),
			], engine.portConnections())

	def test_connect_to_connectors(self):
		engine = self.connectivityFixture()
		multi1 = Processing("multi1", engine)
		multi2 = Processing("multi2", engine)
		self.assertEqual(3, multi1 > multi2._incontrols)
		self.assertEqual(3, multi1 > multi2._inports)
		self.assertEqual([
			("multi1", "OutControl1", "multi2", "InControl1"),
			("multi1", "OutControl2", "multi2", "InControl2"),
			("multi1", "OutControl3", "multi2", "InControl3"),
			], engine.controlConnections())
		self.assertEqual([
			("multi1", "OutPort1", "multi2", "InPort1"),
			("multi1", "OutPort2", "multi2", "InPort2"),
			("multi1", "OutPort3", "multi2", "InPort3"),
			], engine.portConnections())

	def test_connect_to_connector(self):
		engine = self.connectivityFixture()
		multi1 = Processing("multi1", engine)
		csink = Processing("csink", engine)

		self.assertEqual(3, multi1 > csink.InControl1)
		self.assertEqual([
			("multi1", "OutControl1", "csink", "InControl1"),
			("multi1", "OutControl2", "csink", "InControl1"),
			("multi1", "OutControl3", "csink", "InControl1"),
			], engine.controlConnections())
		self.assertEqual([
			], engine.portConnections())

	def test_connect_to_connectors_wrongDirecction(self):
		engine = self.connectivityFixture()
		multi1 = Processing("multi1", engine)
		multi2 = Processing("multi2", engine)
		try :
			multi1 > multi2._outports
		except BadConnectorDirectionOrder, e:
			self.assertEqual(e.message,
				"Wrong connectors order: Output > Input")
		else :
			self.fail("Exception expected")

	def test_connect_to_connector_wrongDirection(self):
		engine = self.connectivityFixture()
		multi1 = Processing("multi1", engine)
		csource = Processing("csource", engine)

		try :
			multi1 > csource.OutControl1
		except BadConnectorDirectionOrder, e:
			self.assertEqual(e.message,
				"Wrong connectors order: Output > Input")
		else:
			self.fail("Exception expected")

	def test_connect_to_alien(self):
		engine = self.connectivityFixture()
		proc1 = Processing("multi1", engine)

		try :
			proc1 > 34
		except AssertionError, e :
			self.assertEqual(e.message,
				"Unexpected connection peer: 34")
		else :
			self.fail("Failed assertion expected")

	def test_connect_from_connectors(self):
		engine = self.connectivityFixture()
		multi1 = Processing("multi1", engine)
		multi2 = Processing("multi2", engine)
		self.assertEqual(3, multi2 < multi1._outcontrols)
		self.assertEqual(3, multi2 < multi1._outports)
		self.assertEqual([
			("multi1", "OutControl1", "multi2", "InControl1"),
			("multi1", "OutControl2", "multi2", "InControl2"),
			("multi1", "OutControl3", "multi2", "InControl3"),
			], engine.controlConnections())
		self.assertEqual([
			("multi1", "OutPort1", "multi2", "InPort1"),
			("multi1", "OutPort2", "multi2", "InPort2"),
			("multi1", "OutPort3", "multi2", "InPort3"),
			], engine.portConnections())

	def test_connect_from_connector(self):
		engine = self.connectivityFixture()
		multi1 = Processing("multi1", engine)
		multi2 = Processing("multi2", engine)
		self.assertEqual(4, multi2 < multi1.OutControl2)
		self.assertEqual(3, multi2 < multi1.OutPort3)
		self.assertEqual([
			("multi1", "OutControl2", "multi2", "InControl1"),
			("multi1", "OutControl2", "multi2", "InControl2"),
			("multi1", "OutControl2", "multi2", "InControl3"),
			("multi1", "OutControl2", "multi2", "InControl4"),
			], engine.controlConnections())
		self.assertEqual([
			("multi1", "OutPort3", "multi2", "InPort1"),
			("multi1", "OutPort3", "multi2", "InPort2"),
			("multi1", "OutPort3", "multi2", "InPort3"),
			], engine.portConnections())

	def test_connect_from_connectors_wrongDirecction(self):
		engine = self.connectivityFixture()
		multi1 = Processing("multi1", engine)
		multi2 = Processing("multi2", engine)
		try :
			multi1 < multi2._inports
		except BadConnectorDirectionOrder, e:
			self.assertEqual(e.message,
				"Wrong connectors order: Input < Output")
		else :
			self.fail("Exception expected")

	def test_connect_from_connector_wrongDirection(self):
		engine = self.connectivityFixture()
		multi1 = Processing("multi1", engine)
		multi2 = Processing("multi2", engine)

		try :
			multi1 < multi2.InControl1
		except BadConnectorDirectionOrder, e:
			self.assertEqual(e.message,
				"Wrong connectors order: Input < Output")
		else:
			self.fail("Exception expected")

	def test_connect_from_alien(self):
		engine = self.connectivityFixture()
		proc1 = Processing("multi1", engine)

		try :
			proc1 < 34
		except AttributeError, e :
			self.assertEqual(e.message,
				"'int' object has no attribute 'connect'")
		else :
			self.fail("Failed assertion expected")

	def test_connect_noConnectorToConnect(self):
		engine = self.connectivityFixture()
		psource = Processing("psource", engine)
		csink = Processing("csink", engine)
		self.assertEqual(0, psource > csink)

class ProcessingTests_Dummy(ProcessingTests, unittest.TestCase):
	def empty(self):
		import Dummy_NetworkProxy
		return Dummy_NetworkProxy.Dummy_NetworkProxy()

class ProcessingTests_Clam(ProcessingTests, unittest.TestCase):
	def empty(self):
		import Clam_NetworkProxy
		return Clam_NetworkProxy.Clam_NetworkProxy()

	"Override because of CLAM inport connection limitation"
	def test_connect_from_processing_to_port(self):
		engine = self.empty()
		engine.addProcessing("ProcessingWithPortsAndControls", "proc1")
		engine.addProcessing("DummyPortSink", "proc2")
		proc1 = Processing("proc1", engine)
		proc2 = Processing("proc2", engine)

		self.assertEqual(1, proc1 > proc2.InPort1)
		self.assertEqual([
			], engine.controlConnections())
		self.assertEqual([
			("proc1", "OutPort1", "proc2", "InPort1"),
			], engine.portConnections())

if __name__ == '__main__':
	unittest.main()

