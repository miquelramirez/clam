
class Configuration(object):
	def __init__(self, proxy):
		object.__setattr__(self, "_proxy", proxy)

	def __getitem__(self, name):
		if not name in self._proxy.keys():
			raise KeyError(name)
		return self._proxy[name]
	def __setitem__(self, name, value):
		if not name in self._proxy.keys():
			raise KeyError(name)
		self._proxy[name] = value

	def __getattr__(self, name):
		if not name in self._proxy.keys():
			raise AttributeError(name)
		return self.__getitem__(name)
	def __setattr__(self, name, value):
		if not name in self._proxy.keys():
			raise AttributeError(name)
		self.__setitem__(name, value)

	def __dir__(self):
		return self._proxy.keys()

import operator
import unittest
import TestFixtures
import Dummy_ConfigurationProxy

class ConfigurationTests(unittest.TestCase):
	def stringParametersConfig(self) :
		return Dummy_ConfigurationProxy.Dummy_ConfigurationProxy(
			TestFixtures.dummyConfigWithStrings())
	def test_configGettingAsDictionary(self):
		c = Configuration(self.stringParametersConfig())
		self.assertEqual(c["ConfigParam1"], "Param1")
		self.assertEqual(c["ConfigParam2"], "Param2")

	def test_configSettingAndGettingAsDictionary(self):
		c = Configuration(self.stringParametersConfig())
		c["ConfigParam1"] = 'newvalue'
		self.assertEqual(c["ConfigParam1"], "newvalue")

	def test_configGettingAsAttribute(self):
		c = Configuration(self.stringParametersConfig())
		self.assertEqual(c.ConfigParam1, "Param1")
		self.assertEqual(c.ConfigParam2, "Param2")

	def test_configSettingAndGettingAsAttribute(self):
		c = Configuration(self.stringParametersConfig())
		c.ConfigParam1 = 'newvalue'
		self.assertEqual(c.ConfigParam1, "newvalue")

	def test_configSettingAsAttributeAndGettingAsDictionary(self):
		c = Configuration(self.stringParametersConfig())
		c.ConfigParam1 = 'newvalue1'
		c.ConfigParam2 = 'newvalue2'
		self.assertEqual(c["ConfigParam1"], "newvalue1")
		self.assertEqual(c["ConfigParam2"], "newvalue2")

	def test_ValuesAsAttributesSettingAndFailing(self):
		c = Configuration(self.stringParametersConfig())
		self.assertRaises(AttributeError, setattr, c, "ConfigParam4", "newvalue")

	def test_ValuesAsAttributesGettingAndFailing(self):
		c = Configuration(self.stringParametersConfig())
		self.assertRaises(AttributeError, getattr, c, "ConfigParam4")

	def test_ValuesAsDictionarySettingAndFailing(self):
		c = Configuration(self.stringParametersConfig())
		self.assertRaises(KeyError, operator.setitem, c, "ConfigParam4", "newvalue")

	def test_ValuesAsDictionaryGettingAndFailing(self):
		c = Configuration(self.stringParametersConfig())
		self.assertRaises(KeyError, operator.getitem, c, "ConfigParam4")

	def test_dirFunction(self):
		c = Configuration(self.stringParametersConfig())
		self.assertEquals(["ConfigParam1", "ConfigParam2", "ConfigParam3"], dir(c))

	def test_setValueOfDifferentTypeAndFail(self):
		c = Configuration(self.stringParametersConfig())
		try:
			c.ConfigParam1 = 1
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals("str value expected, got int", e.args[0])

if __name__ == '__main__':
	unittest.main()
