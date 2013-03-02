import Clam_ConfigurationProxy
import DummyProcessings
import unittest

class Clam_ConfigurationProxyTests(unittest.TestCase):

	def test_createConfigurationProxy(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		self.assertEqual(
		"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
		"<Configuration>\n"
		"\n"
		"  <AString>DefaultValue</AString>\n"
		"\n"
		"  <OtherString>Another default value</OtherString>\n"
		"\n"
		"</Configuration>\n"
		, engine.xml())

	def test_getValueFromStringAttributes(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		self.assertEqual("DefaultValue", engine["AString"])
		self.assertEqual("Another default value", engine["OtherString"])

	def test_raiseAttributeErrorGettingValueFromWrongAttribute(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		try:
			engine["WrongAttribute"]
			self.fail("Exception expected")
		except KeyError, e:
			self.assertEqual("WrongAttribute", e.args[0])

	def test_getDefaultValue_whenInt(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithIntConfiguration")
		self.assertEqual(42, engine["FirstInt"])
		self.assertEqual(15, engine["SecondInt"])

	def test_getDefaultValue_whenOtherTypes(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		self.assertEqual(42, engine["IntAttribute"])
		self.assertEqual(False, engine["BoolAttribute"])
		self.assertEqual(3.140000104904175, engine["FloatAttribute"])
		self.assertEqual(134.15, engine["DoubleAttribute"])

	def test_setValueFromStringAttributes(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		engine["AString"] = "newvalue"
		self.assertEqual("newvalue", engine["AString"])

	def test_setValueFromIntegerAttributes(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithIntConfiguration")
		engine["FirstInt"] = 24
		self.assertEqual(24, engine["FirstInt"])

	def test_setValueFromDifferentTypesOfAttributes(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		engine["BoolAttribute"] = True
		engine["FloatAttribute"] = 4.15
		engine["DoubleAttribute"] = 124.78
		self.assertEqual(True, engine["BoolAttribute"])
		self.assertEqual(4.150000095367432, engine["FloatAttribute"])
		self.assertEqual(124.78, engine["DoubleAttribute"])

	def test_setAttr_wrongName(self):
		c = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithIntConfiguration")
		try:
			c["WrongParam"] = 24
			self.fail("Exception expected")
		except KeyError, e:
			self.assertEquals("WrongParam", e.args[0])

	def test_set_wrongTypeForString(self):
		c = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		try:
			c["AString"] = 2
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals(
				"While setting parameter 'AString', "
				"cannot convert a Python value of type 'int' "
				"into a C++ value of type 'string'."
				, e.args[0])

	def test_set_wrongTypeForInt(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithIntConfiguration")
		try:
			engine["FirstInt"] = "WrongType"
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals(
				"While setting parameter 'FirstInt', "
				"cannot convert a Python value of type 'str' "
				"into a C++ value of type 'int'."
				, e.args[0])

	def test_set_wrongTypeForBool(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		try:
			engine["BoolAttribute"] = 2.15
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals(
				"While setting parameter 'BoolAttribute', "
				"cannot convert a Python value of type 'float' "
				"into a C++ value of type 'bool'."
				, e.args[0])

	def test_set_wrongTypeForFloat(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		try:
			engine["FloatAttribute"] = "Value"
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals(
				"While setting parameter 'FloatAttribute', "
				"cannot convert a Python value of type 'str' "
				"into a C++ value of type 'float'."
				, e.args[0])

	def test_keys(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		self.assertEqual( ["AString", "OtherString"], engine.keys() )

	def test_nonDefault(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxyWithProc("AudioSource")
		self.assertEqual(False, engine.nonDefault("NSources"))
		engine["NSources"] = 2
		self.assertEqual(True, engine.nonDefault("NSources"))

	def test_hold_apply(self):
		c = Clam_ConfigurationProxy.createConfigurationProxyWithProc("AudioSource")
		c.hold()
		c["NSources"] = 2
		self.assertEqual(1, c["NSources"])
		c.apply()
		self.assertEqual(2, c["NSources"])

	def test_clone(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxyWithProc("AudioSource")
		configClone = engine.clone()
		configClone["NSources"] = 2
		self.assertEqual(1, engine["NSources"])
		self.assertEqual(2, configClone["NSources"])

	def test_copyConfig(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxyWithProc("AudioSource")
		config = Clam_ConfigurationProxy.createConfigurationProxyWithProc("AudioSource")
		config["NSources"] = 2
		self.assertEqual(1, engine["NSources"])
		engine.copyConfig(config)
		self.assertEqual(2, config["NSources"])
		self.assertEqual(2, engine["NSources"])

	def test_get_optional_attribute(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithOptionalAttributes")
		self.assertEqual(None, engine["SecondInt"])

	def test_set_optional_attribute(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithOptionalAttributes")
		engine["SecondInt"] = 22
		self.assertEqual(22, engine["SecondInt"])

	def test_set_attribute_as_removed(self):
		engine = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithOptionalAttributes")
		engine["FirstInt"] = None
		self.assertEqual(None, engine["FirstInt"])

if __name__ == '__main__':
	unittest.main()
