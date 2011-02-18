import Processing

class Network(object):
	def __init__(self, networkProxy):
		for processName in networkProxy.processingsName():
			self.__dict__[processName] = Processing.Processing(processName, networkProxy)

	def __getitem__(self, name):
		if name in dir(self):
			return self.__dict__[name]
		raise KeyError(name)

	def __setitem__(self, name, value):
		pass

	def __getattr__(self, name):
		try:
			return self.__getitem__(name)
		except KeyError:
			raise AttributeError(name)

	def __setattr__(self, name, value):
		pass

	def __dir__(self):
		return self.__dict__.keys()

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
		self.assertRaises(AttributeError, getattr, net, "Processing3")

	def test_ValuesAsDictionaryGettingAndFailing(self):
		net = Network(TestFixtures.proxy())
		self.assertRaises(KeyError, operator.getitem, net, "Processing3")


if __name__ == '__main__':
	unittest.main()