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
		self._dict = _dummyConfigurations[config].copy()

	def __getitem__(self, name):
		return self._dict[name]

	def __setitem__(self, name, value):
		if type(value) != type(_dummyConfigurations[self._dict["typename"]][name]):
			raise TypeError("%s value expected, got %s"%( type(_dummyConfigurations[self._dict["typename"]][name]).__name__, type(value).__name__) )
		self._dict[name] = value

	def keys(self):
		keys = self._dict.keys()
		keys.remove("typename")
		return keys

import operator
import unittest
class Dummy_ConfigurationProxyTests(unittest.TestCase):
	def test_configGetting(self):
		c = Dummy_ConfigurationProxy("StringParameters")
		self.assertEqual(c["ConfigParam1"], "Param1")
		self.assertEqual(c["ConfigParam2"], "Param2")
	def test_configSettingAndGetting(self):
		c = Dummy_ConfigurationProxy("StringParameters")
		c["ConfigParam1"] = 'newvalue'
		self.assertEqual(c["ConfigParam1"], "newvalue")
	def test_keys(self):
		c = Dummy_ConfigurationProxy("StringParameters")
		self.assertEqual(["ConfigParam3", "ConfigParam2", "ConfigParam1"], c.keys() )

if __name__ == '__main__':
	unittest.main()
