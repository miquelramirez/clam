class Dummy_ConfigurationProxy(object):
	def __init__(self, config):
		self._default = config.copy()
		self._dict = config

	def __getitem__(self, name):
		if type(self._dict[name]) == dict:
			import Configuration
			return Configuration.Configuration( Dummy_ConfigurationProxy(self._dict[name]) )
		return self._dict[name]

	def __setitem__(self, name, value):
		if type(value) != type(self._default[name]):
			raise TypeError("%s value expected, got %s"%( type(self._default[name]).__name__, type(value).__name__))
		self._dict[name] = value

	def keys(self):
		return self._dict.keys()

	def nonDefault(self, attribute):
		return not self._default[attribute] == self._dict[attribute]

import operator
import unittest
import TestFixtures
class Dummy_ConfigurationProxyTests(unittest.TestCase):

	def test_get_default(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		self.assertEqual(c["ConfigParam1"], "Param1")
		self.assertEqual(c["ConfigParam2"], "Param2")

	def test_set(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		c["ConfigParam1"] = 'newvalue'
		self.assertEqual(c["ConfigParam1"], "newvalue")

	def test_keys(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		self.assertEqual(["ConfigParam3", "ConfigParam2", "ConfigParam1"], c.keys() )

	def test_set_wrongType(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		try:
			c["ConfigParam1"] = 1
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals("str value expected, got int", e.args[0])

	def test_set_wrongName(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		try:
			c["WrongParam1"] = "ParamValue"
			self.fail("Exception expected")
		except KeyError, e:
			self.assertEquals("WrongParam1", e.args[0])

	def test_get_wrongName(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		try:
			value = c["WrongParam1"]
			self.fail("Exception expected")
		except KeyError, e:
			self.assertEquals("WrongParam1", e.args[0])

	def test_check_nondefault_value(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		c["ConfigParam1"] = 'newvalue'
		self.assertEqual(True, c.nonDefault("ConfigParam1"))
		self.assertEqual(False, c.nonDefault("ConfigParam2"))

	def test_nestedconfig(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithNestedConfigs())
		self.assertEquals('defaultnested1', c["ConfigParam3"]["NestedParam1"])


if __name__ == '__main__':
	unittest.main()

