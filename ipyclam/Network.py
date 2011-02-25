import Processing

class Network(object):
	def __init__(self, networkProxy):
		self.__dict__['_proxy'] = networkProxy

	def __getitem__(self, name):
		if not self._proxy.hasProcessing(name) :
			raise KeyError(name)
		return Processing.Processing(proxy=self._proxy, name=name)

	def __getattr__(self, name):
		if not self._proxy.hasProcessing(name) :
			raise AttributeError(name)
		return Processing.Processing(proxy=self._proxy, name=name)

	def __dir__(self):
		return self._proxy.processingNames()

	def code(self):
		return "\n".join([
			"network.%s = '%s'"%(name, self._proxy.processingType(name))
			for name in self._proxy.processingNames()])

	def __setattr__(self, name, type) :
		# TODO: fail on existing attributes (not processings)
		self._proxy.addProcessing(type, name)
		

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

	def test_codeEmptyNetwork(self) :
		net = Network(TestFixtures.empty())
		self.assertEquals("", net.code())

	def test_addProcessing(self) :
		net = Network(TestFixtures.empty())
		net.processing1 = "MinimalProcessing"
		self.assertEquals(
			"network.processing1 = 'MinimalProcessing'"
			, net.code())

	def test_addTwoProcessingsSameType(self) :
		net = Network(TestFixtures.empty())
		net.processing1 = "MinimalProcessing"
		net.processing2 = "MinimalProcessing"
		self.assertEquals(
			"network.processing1 = 'MinimalProcessing'\n"
			"network.processing2 = 'MinimalProcessing'"
			, net.code())

	def test_addTwoProcessingsDifferentType(self) :
		net = Network(TestFixtures.empty())
		net.processing1 = "MinimalProcessing"
		net.processing2 = "PortSink"
		self.assertEquals(
			"network.processing1 = 'MinimalProcessing'\n"
			"network.processing2 = 'PortSink'"
			, net.code())

if __name__ == '__main__':
	unittest.main()
