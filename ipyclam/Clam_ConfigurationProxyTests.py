import Clam_ConfigurationProxy
import DummyProcessingsModule
import unittest

class Clam_ConfigurationProxyTests(unittest.TestCase):

	def test_createConfigurationProxy(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		self.assertEqual(
		"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
		"<Configuration>\n"
		"\n"
		"  <AString>DefaultValue</AString>\n"
		"\n"
		"  <OtherString>Another default value</OtherString>\n"
		"\n"
		"</Configuration>\n"
		, proxy.xml())

	def test_getValueFromStringAttributes(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		self.assertEqual("DefaultValue", proxy["AString"])
		self.assertEqual("Another default value", proxy["OtherString"])

	def test_raiseAttributeErrorGettingValueFromWrongAttribute(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		try:
			proxy["WrongAttribute"]
			self.fail("Exception expected")
		except KeyError, e:
			self.assertEqual("WrongAttribute", e.args[0])

	def test_getValueFromIntAttributes(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithIntConfiguration")
		self.assertEqual(42, proxy["FirstInt"])
		self.assertEqual(15, proxy["SecondInt"])

	def test_getValueFromDifferentTypesOfAttributes(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		self.assertEqual(42, proxy["IntAttribute"])
		self.assertEqual(False, proxy["BoolAttribute"])
		self.assertEqual(3.140000104904175, proxy["FloatAttribute"])
		self.assertEqual(134.15, proxy["DoubleAttribute"])

	def test_setValueFromStringAttributes(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		proxy["AString"] = "newvalue"
		self.assertEqual("newvalue", proxy["AString"])

	def test_setValueFromIntegerAttributes(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithIntConfiguration")
		proxy["FirstInt"] = 24
		self.assertEqual(24, proxy["FirstInt"])

	def test_setValueFromDifferentTypesOfAttributes(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		proxy["BoolAttribute"] = True
		proxy["FloatAttribute"] = 4.15
		proxy["DoubleAttribute"] = 124.78
		self.assertEqual(True, proxy["BoolAttribute"])
		self.assertEqual(4.150000095367432, proxy["FloatAttribute"])
		self.assertEqual(124.78, proxy["DoubleAttribute"])

	def test_setAttr_wrongName(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithIntConfiguration")
		try:
			proxy["WrongAttribute"] = 24
			self.fail("Exception expected")
		except KeyError, e:
			self.assertEquals("WrongAttribute", e.args[0])

	def test_set_wrongTypeForString(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		try:
			proxy["AString"] = 2
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals(
				"While setting parameter 'AString', "
				"cannot convert a Python value of type 'int' "
				"into a C++ value of type 'string'."
				, e.args[0])

	def test_set_wrongTypeForInt(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithIntConfiguration")
		try:
			proxy["FirstInt"] = "WrongType"
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals(
				"While setting parameter 'FirstInt', "
				"cannot convert a Python value of type 'str' "
				"into a C++ value of type 'int'."
				, e.args[0])

	def test_set_wrongTypeForBool(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		try:
			proxy["BoolAttribute"] = 2.15
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals(
				"While setting parameter 'BoolAttribute', "
				"cannot convert a Python value of type 'float' "
				"into a C++ value of type 'bool'."
				, e.args[0])

	def test_set_wrongTypeForFloat(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		try:
			proxy["FloatAttribute"] = "Value"
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals(
				"While setting parameter 'FloatAttribute', "
				"cannot convert a Python value of type 'str' "
				"into a C++ value of type 'float'."
				, e.args[0])

	def test_keys(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		self.assertEqual( ["AString", "OtherString"], proxy.keys() )

	def test_nonDefault(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxyWithProc("AudioSource")
		self.assertEqual(False, proxy.nonDefault("NSources"))
		proxy["NSources"] = 2
		self.assertEqual(True, proxy.nonDefault("NSources"))

	# TODO: reevaluate if this is the required behaviour
	def test_hold_apply(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxyWithProc("AudioSource")
		proxy.hold()
		proxy["NSources"] = 2
		self.assertEqual(1, proxy["NSources"])
		proxy.apply()
		self.assertEqual(2, proxy["NSources"])

	def test_clone(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxyWithProc("AudioSource")
		configClone = proxy.clone()
		configClone["NSources"] = 2
		self.assertEqual(1, proxy["NSources"])
		self.assertEqual(2, configClone["NSources"])

	def test_copyConfig(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxyWithProc("AudioSource")
		config = Clam_ConfigurationProxy.createConfigurationProxyWithProc("AudioSource")
		config["NSources"] = 2
		self.assertEqual(1, proxy["NSources"])
		proxy.copyConfig(config)
		self.assertEqual(2, config["NSources"])
		self.assertEqual(2, proxy["NSources"])

	def test_get_optional_attribute(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithOptionalAttributes")
		self.assertEqual(None, proxy["SecondInt"])

	def test_set_optional_attribute(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithOptionalAttributes")
		proxy["SecondInt"] = 22
		self.assertEqual(22, proxy["SecondInt"])

	def test_set_attribute_as_removed(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithOptionalAttributes")
		proxy["FirstInt"] = None
		self.assertEqual(None, proxy["FirstInt"])

if __name__ == '__main__':
	unittest.main()
