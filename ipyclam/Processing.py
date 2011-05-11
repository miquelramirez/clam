import Connector
import Configuration
import Connectors

class NotFound(Exception) : pass

class Processing(object):
	def __init__(self, name, proxy):
		assert(proxy.hasProcessing(name)) # TODO unit test
		self.__dict__["name"] = name
		self.__dict__["proxy"] = proxy
		self.__dict__["type"] = proxy.processingType(name)
		self.__dict__["_config"] = Configuration.Configuration(proxy.processingConfig(self.name))
		self.__dict__["_inports"] = Connectors.Connectors(proxy, name, Connector.Port, Connector.In)
		self.__dict__["_outports"] = Connectors.Connectors(proxy, name, Connector.Port, Connector.Out)
		self.__dict__["_incontrols"] = Connectors.Connectors(proxy, name, Connector.Control, Connector.In)
		self.__dict__["_outcontrols"] = Connectors.Connectors(proxy, name, Connector.Control, Connector.Out)
	def __getitem__(self, name):
		if name in dir(self._config):
			return self._config[name]
		if name in dir(self._inports):
			return self._inports[name]
		if name in dir(self._outports):
			return self._outports[name]
		if name in dir(self._incontrols):
			return self._incontrols[name]
		if name in dir(self._outcontrols):
			return self._outcontrols[name]
		raise KeyError(name)
	def __setitem__(self, name, value):
		if name == 'name':
			self.__dict__["proxy"].processingRename(self.__dict__[name], value)
			self.__dict__["name"] = value
			return
		self._config[name] = value
	def __getattr__(self, name):
		try:
			return self.__getitem__(name)
		except KeyError:
			raise AttributeError(name)
	def __setattr__(self, name, value):
		if name is 'type': raise AttributeError("Attribute 'type' is read only")
		self.__setitem__(name, value)
	def __dir__(self):
		return (
			dir(self._config) + 
			dir(self._inports) + 
			dir(self._outports) + 
			dir(self._incontrols) + 
			dir(self._outcontrols) + 
			[
				"type" , 
				"_config", 
				"_inports", 
				"_outports", 
				"_incontrols", 
				"_outcontrols",
			]
		)

	def connect(self, peer):
		if type(peer) == Processing:
			return self.connectProcessings(peer)
		if type(peer) == Connector.Connector:
			return self.connectWithConnector(peer)

	def __gt__(self, peer) :
		return self.connect(peer)

	#Helper method to connect ports and controls from processing to processing
	def connectProcessings(self, peer):
		inports = 0
		incontrols = 0
		for connector in peer.__dict__["_inports"]:
			if inports >= self.__dict__["_outports"].__len__():
				break
			if self.__dict__["proxy"].areConnectable("Port", self.__dict__["name"], self.__dict__["_outports"][inports].name, connector.host.name, connector.name):
				self.__dict__["_outports"][inports] > connector
				inports += 1
		for connector in peer.__dict__["_incontrols"]:
			if incontrols >= self.__dict__["_outcontrols"].__len__():
				break
			if self.__dict__["proxy"].areConnectable("Port", self.__dict__["name"], self.__dict__["_outcontrols"][incontrols].name, connector.host.name, connector.name):
				self.__dict__["_outcontrols"][incontrols] > connector
				incontrols += 1
		return inports + incontrols

	#Helper method to connect compatible ports or controls to a single connector
	def connectWithConnector(self, peer):
		connections = 0
		if peer.kind == "Control":
			for connector in self.__dict__["_outcontrols"]:
				if self.__dict__["proxy"].areConnectable("Control", self.__dict__["name"], connector.name, peer.host.name, peer.name):
					connector > peer
					connections += 1
			return connections
		if peer.kind == "Port":
			for connector in self.__dict__["_outports"]:
				if self.__dict__["proxy"].areConnectable("Port", self.__dict__["name"], connector.name, peer.host.name, peer.name):
					connector > peer
					connections += 1
			return connections


import unittest
import TestFixtures
class ProcessingTests(unittest.TestCase):

	def test_type(self) :
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.type, "AudioSource")

	def test_typeIsReadOnly(self) :
		p = Processing("Processing1", TestFixtures.proxy())
		try :
			p.type="AnotherType"
			self.fail("Exception expected")
		except AttributeError, e :
			self.assertEquals("Attribute 'type' is read only", e.args[0])

	def test_name(self) :
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.name, "Processing1")

	def test_ConfigGettingAsDictionary(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p["ConfigParam1"], "default1")

	def test_ConfigSettingAndGettingAsDictionary(self):
		p = Processing("Processing1", TestFixtures.proxy())
		p["ConfigParam1"] = 'newvalue'
		self.assertEqual(p["ConfigParam1"], "newvalue")

	def test_configGettingAsAttribute(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.ConfigParam1, "default1")

	def test_configSettingAndGettingAsAttribute(self):
		p = Processing("Processing1", TestFixtures.proxy())
		p.ConfigParam1 = 'newvalue'
		self.assertEqual(p.ConfigParam1, "newvalue")

	def test_ConfigGettersBy_configInterface(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p._config.ConfigParam1, "default1")
		self.assertEqual(p._config["ConfigParam2"], "default2")

	def test_ConfigSettersBy_configInterface(self):
		p = Processing("Processing1", TestFixtures.proxy())
		p._config.ConfigParam1 = "newvalue1"
		p._config["ConfigParam2"] = "newvalue2"
		self.assertEqual(p._config.ConfigParam1, "newvalue1")
		self.assertEqual(p._config["ConfigParam2"], "newvalue2")

	def test_InportsGettingByNameAsDictionary(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p["InPort1"].name, "InPort1")
		self.assertEqual(p["InPort1"].kind, "Port")
		self.assertEqual(p["InPort1"].direction, "In")

	def test_InportsGettingByNameAsAttribute(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.InPort2.name, "InPort2")
		self.assertEqual(p.InPort2.kind, "Port")
		self.assertEqual(p.InPort2.direction, "In")

	def test_OutportsGettingByNameAsDictionary(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p["OutPort1"].name, "OutPort1")
		self.assertEqual(p["OutPort1"].kind, "Port")
		self.assertEqual(p["OutPort1"].direction, "Out")

	def test_OutportsGettingByNameAsAttribute(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.OutPort2.name, "OutPort2")
		self.assertEqual(p.OutPort2.kind, "Port")
		self.assertEqual(p.OutPort2.direction, "Out")

	def test_IncontrolsGettingByNameAsDictionary(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p["InControl1"].name, "InControl1")
		self.assertEqual(p["InControl1"].kind, "Control")
		self.assertEqual(p["InControl1"].direction, "In")

	def test_IncontrolsGettingByNameAsAttribute(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.InControl2.name, "InControl2")
		self.assertEqual(p.InControl2.kind, "Control")
		self.assertEqual(p.InControl2.direction, "In")

	def test_OutcontrolsGettingByNameAsDictionary(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p["OutControl1"].name, "OutControl1")
		self.assertEqual(p["OutControl1"].kind, "Control")
		self.assertEqual(p["OutControl1"].direction, "Out")

	def test_OutcontrolsGettingByNameAsAttribute(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.OutControl2.name, "OutControl2")
		self.assertEqual(p.OutControl2.kind, "Control")
		self.assertEqual(p.OutControl2.direction, "Out")

	def test_processingRenaming(self):
		p = Processing("Processing1", TestFixtures.proxy())
		p.name = "ProcessingRenamed"
		self.assertEqual(p.name, "ProcessingRenamed")

	def test_dirFunction(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEquals(sorted(
			['ConfigParam1', 'ConfigParam2', 
			'InPort1', 'InPort2','InPort3','InPort4',
			'OutPort1', 'OutPort2',
			'InControl1', 'InControl2',
			'OutControl1', 'OutControl2',
			'type', '_config', '_inports', '_outports', '_incontrols', '_outcontrols']), dir(p))

	def test_connect_from_processing_to_processing(self):
		import Network
		net = Network.Network(TestFixtures.empty())
		net.proc1 = "ProcessingWithPortsAndControls"
		net.proc2 = "ProcessingWithPortsAndControls"
		self.assertEquals(6, net.proc1 > net.proc2)
		self.assertEquals(
			"network.proc1 = 'ProcessingWithPortsAndControls'\n"
			"network.proc2 = 'ProcessingWithPortsAndControls'\n"
			"network.proc1.OutPort1 > network.proc2.InPort1\n"
			"network.proc1.OutPort2 > network.proc2.InPort2\n"
			"network.proc1.OutPort3 > network.proc2.InPort3\n"
			"network.proc1.OutControl1 > network.proc2.InControl1\n"
			"network.proc1.OutControl2 > network.proc2.InControl2\n"
			"network.proc1.OutControl3 > network.proc2.InControl3"
			, net.code())

	def test_connect_from_processing_to_connector(self):
		import Network
		net = Network.Network(TestFixtures.empty())
		net.proc1 = "ProcessingWithPortsAndControls"
		net.proc2 = "ControlSink"
		net.proc3 = "PortSink"
		self.assertEquals(3, net.proc1 > net.proc2.InControl1)
		self.assertEquals(4, net.proc1 > net.proc3.InPort1)

if __name__ == '__main__':
	unittest.main()
