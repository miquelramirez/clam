from Processing import Processing

import unittest
import TestFixtures
class ProcessingTests(unittest.TestCase):
	def proxy(self):
		return TestFixtures.proxy()
	def empty(self):
		return TestFixtures.empty()

	def test_type(self) :
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.type, "DummyTypeProcessing1")

	def test_typeIsReadOnly(self) :
		p = Processing("Processing1", self.proxy())
		try :
			p.type="AnotherType"
			self.fail("Exception expected")
		except AttributeError, e :
			self.assertEquals("Attribute 'type' is read only", e.args[0])

	def test_name(self) :
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.name, "Processing1")

	def test_ConfigGettingAsDictionary(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p["ConfigParam1"], "default1")

	def test_ConfigSettingAndGettingAsDictionary(self):
		p = Processing("Processing1", self.proxy())
		p["ConfigParam1"] = 'newvalue'
		self.assertEqual(p["ConfigParam1"], "newvalue")

	def test_configGettingAsAttribute(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.ConfigParam1, "default1")

	def test_configSettingAndGettingAsAttribute(self):
		p = Processing("Processing1", self.proxy())
		p.ConfigParam1 = 'newvalue'
		self.assertEqual(p.ConfigParam1, "newvalue")

	def test_ConfigGettersBy_configInterface(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p._config.ConfigParam1, "default1")
		self.assertEqual(p._config["ConfigParam2"], "default2")

	def test_ConfigSettersBy_configInterface(self):
		p = Processing("Processing1", self.proxy())
		p._config.ConfigParam1 = "newvalue1"
		p._config["ConfigParam2"] = "newvalue2"
		self.assertEqual(p._config.ConfigParam1, "newvalue1")
		self.assertEqual(p._config["ConfigParam2"], "newvalue2")

	def test_InportsGettingByNameAsDictionary(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p["InPort1"].name, "InPort1")
		self.assertEqual(p["InPort1"].kind, "Port")
		self.assertEqual(p["InPort1"].direction, "In")

	def test_InportsGettingByNameAsAttribute(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.InPort2.name, "InPort2")
		self.assertEqual(p.InPort2.kind, "Port")
		self.assertEqual(p.InPort2.direction, "In")

	def test_OutportsGettingByNameAsDictionary(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p["OutPort1"].name, "OutPort1")
		self.assertEqual(p["OutPort1"].kind, "Port")
		self.assertEqual(p["OutPort1"].direction, "Out")

	def test_OutportsGettingByNameAsAttribute(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.OutPort2.name, "OutPort2")
		self.assertEqual(p.OutPort2.kind, "Port")
		self.assertEqual(p.OutPort2.direction, "Out")

	def test_IncontrolsGettingByNameAsDictionary(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p["InControl1"].name, "InControl1")
		self.assertEqual(p["InControl1"].kind, "Control")
		self.assertEqual(p["InControl1"].direction, "In")

	def test_IncontrolsGettingByNameAsAttribute(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.InControl2.name, "InControl2")
		self.assertEqual(p.InControl2.kind, "Control")
		self.assertEqual(p.InControl2.direction, "In")

	def test_OutcontrolsGettingByNameAsDictionary(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p["OutControl1"].name, "OutControl1")
		self.assertEqual(p["OutControl1"].kind, "Control")
		self.assertEqual(p["OutControl1"].direction, "Out")

	def test_OutcontrolsGettingByNameAsAttribute(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.OutControl2.name, "OutControl2")
		self.assertEqual(p.OutControl2.kind, "Control")
		self.assertEqual(p.OutControl2.direction, "Out")

	def test_processingRenaming(self):
		p = Processing("Processing1", self.proxy())
		p.name = "ProcessingRenamed"
		self.assertEqual(p.name, "ProcessingRenamed")

	def test_dirFunction(self):
		p = Processing("Processing1", self.proxy())
		self.assertEquals(sorted(
			['ConfigParam1', 'ConfigParam2', 
			'InPort1', 'InPort2','InPort3','InPort4',
			'OutPort1', 'OutPort2',
			'InControl1', 'InControl2',
			'OutControl1', 'OutControl2',
			'name', 'type', '_config', '_inports', '_outports', '_incontrols', '_outcontrols']), dir(p))

	def test_connect_from_processing_to_processing(self):
		import Network
		net = Network.Network(self.empty())
		net.proc1 = "ProcessingWithPortsAndControls"
		net.proc2 = "ProcessingWithPortsAndControls"
		self.assertEquals(6, net.proc1 > net.proc2)
		self.assertMultiLineEqual(
			"network.proc1 = 'ProcessingWithPortsAndControls'\n"
			"network.proc2 = 'ProcessingWithPortsAndControls'\n"
			"\n"
			"network.proc1.OutPort1 > network.proc2.InPort1\n"
			"network.proc1.OutPort2 > network.proc2.InPort2\n"
			"network.proc1.OutPort3 > network.proc2.InPort3\n"
			"network.proc1.OutControl1 > network.proc2.InControl1\n"
			"network.proc1.OutControl2 > network.proc2.InControl2\n"
			"network.proc1.OutControl3 > network.proc2.InControl3"
			, net.code())

	def test_connect_from_processing_to_connector(self):
		import Network
		net = Network.Network(self.empty())
		net.proc1 = "ProcessingWithPortsAndControls"
		net.proc2 = "DummyControlSink"
		net.proc3 = "PortSink"
		self.assertEquals(3, net.proc1 > net.proc2.InControl1)
		self.assertEquals(4, net.proc1 > net.proc3.InPort1)

	def test_connect_from_processing_to_connectors(self):
		import Network
		net = Network.Network(self.empty())
		net.proc1 = "ProcessingWithPortsAndControls"
		net.proc2 = "ProcessingWithPortsAndControls"
		self.assertEquals(3, net.proc1 > net.proc2._incontrols)
		self.assertEquals(3, net.proc1 > net.proc2._inports)

class Clam_ProcessingTests(ProcessingTests):
	def proxy(self):
		import Clam_NetworkProxy
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		network = Network.Network(proxy)
		network.Processing1 = "DummyTypeProcessing1"
		network.Processing2 = "DummyTypeProcessing2"
		return proxy

	def empty(self):
		import Clam_NetworkProxy
		return Clam_NetworkProxy.Clam_NetworkProxy()

if __name__ == '__main__':
	unittest.main()
