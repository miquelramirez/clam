import operator
import unittest


class Configuration(object):
	def __init__(self, config):
		object.__setattr__(self, "_dict", config.copy())
	def __getitem__(self, name):
		return self._dict[name]
	def __setitem__(self, name, value):
		if not name in self._dict.keys():
			raise KeyError(name)
		item = object.__getattribute__(self, "_dict")
		item[name] = value
	def __getattr__(self, name):
		if not name in self._dict.keys():
			raise AttributeError(name)
		return self.__getitem__(name)
	def __setattr__(self, name, value):
		if not name in self._dict.keys():
			raise AttributeError(name)
		self.__setitem__(name, value)
	def __dir__(self):
		return self._dict.keys()

class ConfigurationTests(unittest.TestCase):
	def config(self) :
		return dict(
			ConfigParam1 = "default1",
			ConfigParam2 = "default2",
		)
	def test_configGettingAsDictionary(self):
		c = Configuration(self.config())
		self.assertEqual(c["ConfigParam1"], "default1")
		self.assertEqual(c["ConfigParam2"], "default2")
	def test_configSettingAndGettingAsDictionary(self):
		c = Configuration(self.config())
		c["ConfigParam1"] = 'newvalue'
		self.assertEqual(c["ConfigParam1"], "newvalue")
	def test_configGettingAsAttribute(self):
		c = Configuration(self.config())
		self.assertEqual(c.ConfigParam1, "default1")
		self.assertEqual(c.ConfigParam2, "default2")
	def test_configSettingAndGettingAsAttribute(self):
		c = Configuration(self.config())
		c.ConfigParam1 = 'newvalue'
		self.assertEqual(c.ConfigParam1, "newvalue")
	def test_configSettingAsAttributeAndGettingAsDictionary(self):
		c = Configuration(self.config())
		c.ConfigParam1 = 'newvalue1'
		c.ConfigParam2 = 'newvalue2'
		self.assertEqual(c["ConfigParam1"], "newvalue1")
		self.assertEqual(c["ConfigParam2"], "newvalue2")
	def test_ValuesAsAttributesSettingAndFailing(self):
		c = Configuration(self.config())
		self.assertRaises(AttributeError, setattr, c, "ConfigParam3", "newvalue")
	def test_ValuesAsAttributesGettingAndFailing(self):
		c = Configuration(self.config())
		self.assertRaises(AttributeError, getattr, c, "ConfigParam3")
	def test_ValuesAsDictionarySettingAndFailing(self):
		c = Configuration(self.config())
		self.assertRaises(KeyError, operator.setitem, c, "ConfigParam3", "newvalue")
	def test_ValuesAsDictionaryGettingAndFailing(self):
		c = Configuration(self.config())
		self.assertRaises(KeyError, operator.getitem, c, "ConfigParam3")
	def test_dirFunction(self):
		c = Configuration(self.config())
		self.assertEquals(["ConfigParam1", "ConfigParam2"], dir(c))

if __name__ == '__main__':
	unittest.main()
