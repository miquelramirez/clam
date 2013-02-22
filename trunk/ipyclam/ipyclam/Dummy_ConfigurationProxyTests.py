from Dummy_ConfigurationProxy import Dummy_ConfigurationProxy

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
			c["WrongParam"] = "ParamValue"
			self.fail("Exception expected")
		except KeyError, e:
			self.assertEquals("WrongParam", e.args[0])

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

	def test_hold_apply(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		c.hold()
		c["ConfigParam1"] = 'newvalue'
		self.assertEqual('Param1', c["ConfigParam1"])
		c.apply()
		self.assertEqual('newvalue', c["ConfigParam1"])

	def test_hold_discard(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		c.hold()
		c["ConfigParam1"] = 'newvalue'
		self.assertEqual('Param1', c["ConfigParam1"])
		c.discard()
		self.assertEqual('Param1', c["ConfigParam1"])

	def test_hold_hold(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		c.hold()
		c["ConfigParam1"] = 'newvalue'
		self.assertEqual('Param1', c["ConfigParam1"])
		try :
			c.hold()
		except AssertionError, e:
			self.assertEqual(e.message,
				"Configuration is already held")
		else:
			self.fail("Should have failed an assertion")

	def test_discard_withoutHold(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		try :
			c.discard()
		except AssertionError, e:
			self.assertEqual(e.message,
				"Discarding a configuration requires to be held")
		else:
			self.fail("Should have failed an assertion")

	def test_clone(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		cClone = c.clone()
		cClone["ConfigParam1"] = 'newvalue'
		self.assertEqual(True, cClone.nonDefault("ConfigParam1"))
		self.assertEqual(False, c.nonDefault("ConfigParam1"))

	def test_optionality(self):
		c = Dummy_ConfigurationProxy(TestFixtures.dummyConfigWithStrings())
		c["ConfigParam1"] = None
		self.assertEqual(None, c["ConfigParam1"])
		c["ConfigParam1"] = 'new value'
		self.assertEqual('new value', c["ConfigParam1"])

if __name__ == '__main__':
	unittest.main()

