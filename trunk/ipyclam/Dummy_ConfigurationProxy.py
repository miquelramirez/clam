_dummyConfigurations = dict(
	StringParameters = dict(
		typename = "StringParameters",
		ConfigParam1 = "Param1",
		ConfigParam2 = "Param2",
		ConfigParam3 = "Param3"
	)
)

class Dummy_ConfigurationProxy(object):
	def __init__(self, config):
		self._default = config
		self._dict = config.copy()

	def __getitem__(self, name):
		return self._dict[name]

	def __setitem__(self, name, value):
		if type(value) != type(self._default[name]):
			raise TypeError("%s value expected, got %s"%( type(self._default[name]).__name__, type(value).__name__))
		self._dict[name] = value

	def keys(self):
		return self._dict.keys()

import operator
import unittest
import TestFixtures
class Dummy_ConfigurationProxyTests(unittest.TestCase):
	def test_configGetting(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		self.assertEqual(c["ConfigParam1"], "Param1")
		self.assertEqual(c["ConfigParam2"], "Param2")

	def test_configSettingAndGetting(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		c["ConfigParam1"] = 'newvalue'
		self.assertEqual(c["ConfigParam1"], "newvalue")

	def test_keys(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		self.assertEqual(["ConfigParam3", "ConfigParam2", "ConfigParam1"], c.keys() )

if __name__ == '__main__':
	unittest.main()
