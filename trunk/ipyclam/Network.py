import Processing
import ProcessingTypes

class Network(object):
	def __init__(self, networkProxy):
		self.__dict__['_proxy'] = networkProxy
		self.__dict__['methods'] = ["types", "code", "xml"]

	def __getitem__(self, name):
		if not self._proxy.hasProcessing(name) :
			raise KeyError(name)
		return Processing.Processing(proxy=self._proxy, name=name)

	def __getattr__(self, name):
		if name == "description":
			return self._proxy.getDescription()
		if not self._proxy.hasProcessing(name) :
			raise AttributeError(name)
		return Processing.Processing(proxy=self._proxy, name=name)

	def __dir__(self):
		return self._proxy.processingNames() + ["description"]

	@property
	def types(self) :
		return ProcessingTypes.ProcessingTypes(self._proxy)

	def code(self, networkVar = "network", fullConfig = False):

		def appendAttribute(name):
			if name[0].isdigit():
				return "[\"%s\"]"%name
			if name.isalnum():
				return "."+name
			else:
				return "[\"%s\"]"%name
		code = ""
		if self._proxy.getDescription() != "":
			code += "%s.description = %s\n"%(networkVar, repr(self._proxy.getDescription()) )
		code += "\n".join([
			"%s%s = '%s'"%(networkVar, appendAttribute(name), self._proxy.processingType(name))
			for name in self._proxy.processingNames()])
		code += "\n"
		code += self.configCode(networkVar, fullConfig)
		code += "\n"
		code += "\n".join([
				"%s%s%s > %s%s%s"%(networkVar, appendAttribute(fromProcessing), appendAttribute(fromConnector), networkVar, appendAttribute(toProcessing), appendAttribute(toConnector))
				for fromProcessing, fromConnector, toProcessing, toConnector in self._proxy.portConnections()])
		code += "\n"
		code += "\n".join([
				"%s%s%s > %s%s%s"%(networkVar, appendAttribute(fromProcessing), appendAttribute(fromConnector), networkVar, appendAttribute(toProcessing), appendAttribute(toConnector))
				for fromProcessing, fromConnector, toProcessing, toConnector in self._proxy.controlConnections()])
		return code

	def configCode(self, networkVar, fullConfig):
		configCode = ""
		for name in self._proxy.processingNames():
			configCode += self.__getitem__(name)._config.code(name, networkVar, fullConfig)
		return configCode

	def __setattr__(self, name, type) :
		if name == "description":
			self._proxy.setDescription(type)
			return
		if name in self.__dict__['methods']:
			raise AssertionError("Wrong processing name: %s is a method"%(name))
		# TODO: fail on existing attributes (not processings)
		self._proxy.addProcessing(type, name)

	def __setitem__(self, name, type) :
		self.__setattr__(name, type)

	def __delattr__(self, name):
		if not self._proxy.hasProcessing(name) :
			raise AttributeError(name)
		self._proxy.deleteProcessing(name)

	def __delitem__(self, name):
		if not self._proxy.hasProcessing(name) :
			raise KeyError(name)
		self.__delattr__(name)
	# TODO: Workaround to be able to use the function from the C++ proxy
	def xml(self):
		return self._proxy.xml()

	def __repr__(self) :
		return self.code()

import operator
import unittest
import TestFixtures

class NetworkTests(unittest.TestCase):
	def proxy(self):
		return TestFixtures.proxy()
	def empty(self):
		return TestFixtures.empty()

	def test_ProcessingGettingAsDictionary(self) :
		net = Network(self.proxy())
		self.assertEqual("Processing1", net["Processing1"].name)
		self.assertEqual("Processing2", net["Processing2"].name)

	def test_ProcessingGettingAsAttribute(self) :
		net = Network(self.proxy())
		self.assertEqual("Processing1", net.Processing1.name)
		self.assertEqual("Processing2", net.Processing2.name)

	def test_dirFunction(self) :
		net = Network(self.proxy())
		self.assertEquals(["Processing1", "Processing2", "description"], dir(net))

	def test_ProcessingAsAttributesGettingAndFailing(self):
		net = Network(self.proxy())
		self.assertRaises(AttributeError, getattr, net, "NonExistingProcessing")

	def test_ValuesAsDictionaryGettingAndFailing(self):
		net = Network(self.proxy())
		self.assertRaises(KeyError, operator.getitem, net, "NonExistingProcessing")

	def test_codeEmptyNetwork(self) :
		net = Network(self.empty())
		self.assertEquals("\n\n\n", net.code())

	def test_addProcessing(self) :
		net = Network(self.empty())
		net.processing1 = "PortSource"
		self.assertEquals(
			"network.processing1 = 'PortSource'\n"
			"\n"
			"\n"
			, net.code())

	def test_addProcessingAsItem(self) :
		net = Network(self.empty())
		net["processing1"] = "PortSource"
		self.assertEquals(
			"network.processing1 = 'PortSource'\n"
			"\n"
			"\n"
			, net.code())

	def test_addTwoProcessingsSameType(self) :
		net = Network(self.empty())
		net.processing1 = "PortSource"
		net.processing2 = "PortSource"
		self.assertEquals(
			"network.processing1 = 'PortSource'\n"
			"network.processing2 = 'PortSource'\n"
			"\n"
			"\n"
			, net.code())

	def test_addTwoProcessingsDifferentType(self) :
		net = Network(self.empty())
		net.processing1 = "PortSource"
		net.processing2 = "PortSink"
		self.assertEquals(
			"network.processing1 = 'PortSource'\n"
			"network.processing2 = 'PortSink'\n"
			"\n"
			"\n"
			, net.code())

	def test_addPortSourceAndPortSinkAndConnectPorts(self) :
		net = Network(self.empty())
		net.processing1 = "PortSource"
		net.processing2 = "PortSink"
		net.processing1.OutPort1 > net.processing2.InPort1
		self.assertEquals(
			"network.processing1 = 'PortSource'\n"
			"network.processing2 = 'PortSink'\n"
			"\n"
			"network.processing1.OutPort1 > network.processing2.InPort1\n"
			, net.code())

	def test_addControlSourceAndControlSinkAndConnectControls(self) :
		net = Network(self.empty())
		net.processing1 = "DummyControlSource"
		net.processing2 = "DummyControlSink"
		net.processing1.OutControl1 > net.processing2.InControl1
		self.assertEquals(
			"network.processing1 = 'DummyControlSource'\n"
			"network.processing2 = 'DummyControlSink'\n"
			"\n"
			"\n"
			"network.processing1.OutControl1 > network.processing2.InControl1"
			, net.code())

	def test_addPortControlSourceAndPortControlSinkAndConnectPortsAndControls(self) :
		net = Network(self.empty())
		net.processing1 = "PortSource"
		net.processing2 = "PortSink"
		net.processing3 = "DummyControlSource"
		net.processing4 = "DummyControlSink"
		net.processing1.OutPort1 > net.processing2.InPort1
		net.processing3.OutControl1 > net.processing4.InControl1
		self.assertEquals(
			"network.processing1 = 'PortSource'\n"
			"network.processing2 = 'PortSink'\n"
			"network.processing3 = 'DummyControlSource'\n"
			"network.processing4 = 'DummyControlSink'\n"
			"\n"
			"network.processing1.OutPort1 > network.processing2.InPort1\n"
			"network.processing3.OutControl1 > network.processing4.InControl1"
			, net.code())

	def test_types(self) :
		net = Network(self.empty())
		self.assertEqual("DummyControlSink", net.types.DummyControlSink)

	def test_setDescription(self):
		net = Network(self.empty())
		net.description = "A description"
		self.assertEquals("A description", net.description)

	def test_changeNetworkVarForCode(self):
		net = Network(self.empty())
		net.processing1 = "DummyControlSource"
		net.processing2 = "DummyControlSink"
		net.processing1.OutControl1 > net.processing2.InControl1
		self.assertEquals(
			"net.processing1 = 'DummyControlSource'\n"
			"net.processing2 = 'DummyControlSink'\n"
			"\n"
			"\n"
			"net.processing1.OutControl1 > net.processing2.InControl1"
			, net.code("net"))		

	def test_deleteProcessingAsAttribute(self):
		net = Network(self.empty())
		net.processing1 = "PortSource"
		net.processing2 = "PortSink"
		del net.processing1
		self.assertEquals(
			"network.processing2 = 'PortSink'\n"
			"\n"
			"\n"
			, net.code())

	def test_deleteProcessingAsItem(self):
		net = Network(self.empty())
		net["processing1"] = "PortSource"
		net["processing2"] = "PortSink"
		del net["processing1"]
		self.assertEquals(
			"network.processing2 = 'PortSink'\n"
			"\n"
			"\n"
			, net.code())

	def test_codeWhenNotAlphaNumericProcessingAndConnectorNames(self) :
		net = Network(self.empty())
		net.processing1 = "ProcessingWithNameSpacedPorts"
		net.processing2 = "ProcessingWithNameSpacedPorts"
		net.processing3 = "ProcessingWithNameSpacedControls"
		net.processing4 = "ProcessingWithNameSpacedControls"
		net.processing1["An outport"] > net.processing2["An inport"]
		net.processing3["An outcontrol"] > net.processing4["An incontrol"]
		self.assertEquals(
			"network.processing1 = 'ProcessingWithNameSpacedPorts'\n"
			"network.processing2 = 'ProcessingWithNameSpacedPorts'\n"
			"network.processing3 = 'ProcessingWithNameSpacedControls'\n"
			"network.processing4 = 'ProcessingWithNameSpacedControls'\n"
			"\n"
			"network.processing1[\"An outport\"] > network.processing2[\"An inport\"]\n"
			"network.processing3[\"An outcontrol\"] > network.processing4[\"An incontrol\"]"
			, net.code())

	def test_addProcessingWithName_code_AndFail(self):
		net = Network(self.empty())
		try:
			net.code = "PortSource"
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals("Wrong processing name: code is a method", e.__str__())

	def test_addProcessingWithName_types_AndFail(self):
		net = Network(self.empty())
		try:
			net.types = "PortSource"
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals("Wrong processing name: types is a method", e.__str__())

	def test_codeShowsDescription(self):
		net = Network(self.empty())
		net.description = "A description"
		net.proc1 = "PortSink"
		self.assertEquals("A description", net.description)
		self.assertEquals(
			"network.description = 'A description'\n"
			"network.proc1 = 'PortSink'\n"
			"\n"
			"\n"
			, net.code())

	def test_codeForNumericConnectors(self):
		net = Network(self.empty())
		net.proc1 = "ProcessingWithNumericPorts"
		net.proc2 = "ProcessingWithNumericPorts"
		net.proc1["2"] > net.proc2["1"]
		self.assertEquals(
			"network.proc1 = 'ProcessingWithNumericPorts'\n"
			"network.proc2 = 'ProcessingWithNumericPorts'\n"
			"\n"
			"network.proc1[\"2\"] > network.proc2[\"1\"]\n"
			, net.code())

	def test_change_config_attributes(self):
		net = Network(self.proxy())
		net.Processing1["ConfigParam1"] = 'newvalue'
		self.assertEqual(net.Processing1["ConfigParam1"], "newvalue")

	def test_code_for_changing_config_attributes(self):
		net = Network(self.empty())
		net.Processing1 = "DummyProcessingWithStringConfiguration"
		net.Processing1.UnString = 'newvalue'
		self.assertEquals(
			"network.Processing1 = 'DummyProcessingWithStringConfiguration'\n"
			"network.Processing1['UnString'] = 'newvalue'\n"
			"network.Processing1['UnAltreString'] = 'Un altre valor per defecte'\n"
			"\n\n"
			, net.code(fullConfig=True))

class Clam_NetworkTests(NetworkTests):
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
