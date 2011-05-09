import Processing
import ProcessingTypes

class Network(object):
	def __init__(self, networkProxy):
		self.__dict__['_proxy'] = networkProxy

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
		return self._proxy.processingNames()

	@property
	def types(self) :
		return ProcessingTypes.ProcessingTypes(self._proxy)

	def code(self, networkVar = "network"):
		code = "\n".join([
			"%s.%s = '%s'"%(networkVar, name, self._proxy.processingType(name))
			for name in self._proxy.processingNames()])
		code += "\n"
		code += "\n".join([
				"%s.%s.%s > network.%s.%s"%(networkVar, fromProcessing, fromConnector, toProcessing, toConnector)
				for fromProcessing, fromConnector, toProcessing, toConnector in self._proxy.portConnections()])
		code += "\n"
		code += "\n".join([
				"%s.%s.%s > network.%s.%s"%(networkVar, fromProcessing, fromConnector, toProcessing, toConnector)
				for fromProcessing, fromConnector, toProcessing, toConnector in self._proxy.controlConnections()])
		return code

	def __setattr__(self, name, type) :
		if name == "description":
			self._proxy.setDescription(type)
			return
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

import operator
import unittest
import TestFixtures

class NetworkTests(unittest.TestCase):
	def test_ProcessingGettingAsDictionary(self) :
		net = Network(TestFixtures.proxy())
		self.assertEqual("Processing1", net["Processing1"].name)
		self.assertEqual("Processing2", net["Processing2"].name)

	def test_ProcessingGettingAsAttribute(self) :
		net = Network(TestFixtures.proxy())
		self.assertEqual("Processing1", net.Processing1.name)
		self.assertEqual("Processing2", net.Processing2.name)

	def test_dirFunction(self) :
		net = Network(TestFixtures.proxy())
		self.assertEquals(["Processing1", "Processing2"], dir(net))

	def test_ProcessingAsAttributesGettingAndFailing(self):
		net = Network(TestFixtures.proxy())
		self.assertRaises(AttributeError, getattr, net, "NonExistingProcessing")

	def test_ValuesAsDictionaryGettingAndFailing(self):
		net = Network(TestFixtures.proxy())
		self.assertRaises(KeyError, operator.getitem, net, "NonExistingProcessing")

	def _test_codeEmptyNetwork(self) :
		net = Network(TestFixtures.empty())
		self.assertEquals("\n\n", net.code())

	def _test_addProcessing(self) :
		net = Network(TestFixtures.empty())
		net.processing1 = "MinimalProcessing"
		self.assertEquals(
			"network.processing1 = 'MinimalProcessing'\n"
			"\n"
			, net.code())

	def _test_addProcessingAsItem(self) :
		net = Network(TestFixtures.empty())
		net["processing1"] = "MinimalProcessing"
		self.assertEquals(
			"network.processing1 = 'MinimalProcessing'\n"
			"\n"
			, net.code())

	def _test_addTwoProcessingsSameType(self) :
		net = Network(TestFixtures.empty())
		net.processing1 = "MinimalProcessing"
		net.processing2 = "MinimalProcessing"
		self.assertEquals(
			"network.processing1 = 'MinimalProcessing'\n"
			"network.processing2 = 'MinimalProcessing'\n"
			"\n"
			, net.code())

	def _test_addTwoProcessingsDifferentType(self) :
		net = Network(TestFixtures.empty())
		net.processing1 = "MinimalProcessing"
		net.processing2 = "PortSink"
		self.assertEquals(
			"network.processing1 = 'MinimalProcessing'\n"
			"network.processing2 = 'PortSink'\n"
			"\n"
			, net.code())

	def test_addPortSourceAndPortSinkAndConnectPorts(self) :
		net = Network(TestFixtures.empty())
		net.processing1 = "PortSource"
		net.processing2 = "PortSink"
		net.processing1.OutPort1 > net.processing2.InPort1
		self.assertEquals(
			"network.processing1 = 'PortSource'\n"
			"network.processing2 = 'PortSink'\n"
			"network.processing1.OutPort1 > network.processing2.InPort1\n"
			, net.code())

	def test_addControlSourceAndControlSinkAndConnectControls(self) :
		net = Network(TestFixtures.empty())
		net.processing1 = "ControlSource"
		net.processing2 = "ControlSink"
		net.processing1.OutControl1 > net.processing2.InControl1
		self.assertEquals(
			"network.processing1 = 'ControlSource'\n"
			"network.processing2 = 'ControlSink'\n"
			"\n"
			"network.processing1.OutControl1 > network.processing2.InControl1"
			, net.code())

	def test_addPortControlSourceAndPortControlSinkAndConnectPortsAndControls(self) :
		net = Network(TestFixtures.empty())
		net.processing1 = "PortSource"
		net.processing2 = "PortSink"
		net.processing3 = "ControlSource"
		net.processing4 = "ControlSink"
		net.processing1.OutPort1 > net.processing2.InPort1
		net.processing3.OutControl1 > net.processing4.InControl1
		self.assertEquals(
			"network.processing1 = 'PortSource'\n"
			"network.processing2 = 'PortSink'\n"
			"network.processing3 = 'ControlSource'\n"
			"network.processing4 = 'ControlSink'\n"
			"network.processing1.OutPort1 > network.processing2.InPort1\n"
			"network.processing3.OutControl1 > network.processing4.InControl1"
			, net.code())

	def test_types(self) :
		net = Network(TestFixtures.empty())
		self.assertEqual("MinimalProcessing", net.types.MinimalProcessing)

	def test_setDescription(self):
		net = Network(TestFixtures.empty())
		net.description = "A description"
		self.assertEquals("A description", net.description)

	def test_changeNetworkVarForCode(self):
		net = Network(TestFixtures.empty())
		net.processing1 = "ControlSource"
		net.processing2 = "ControlSink"
		net.processing1.OutControl1 > net.processing2.InControl1
		self.assertEquals(
			"net.processing1 = 'ControlSource'\n"
			"net.processing2 = 'ControlSink'\n"
			"\n"
			"net.processing1.OutControl1 > network.processing2.InControl1"
			, net.code("net"))		

	def test_deleteProcessingAsAttribute(self):
		net = Network(TestFixtures.empty())
		net.processing1 = "PortSource"
		net.processing2 = "PortSink"
		del net.processing1
		self.assertEquals(
			"network.processing2 = 'PortSink'\n"
			"\n"
			, net.code())

	def test_deleteProcessingAsItem(self):
		net = Network(TestFixtures.empty())
		net["processing1"] = "PortSource"
		net["processing2"] = "PortSink"
		del net["processing1"]
		self.assertEquals(
			"network.processing2 = 'PortSink'\n"
			"\n"
			, net.code())

if __name__ == '__main__':
	unittest.main()
