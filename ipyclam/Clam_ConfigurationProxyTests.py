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
		"  <UnString>ValorPerDefecte</UnString>\n"
		"\n"
		"  <UnAltreString>Un altre valor per defecte</UnAltreString>\n"
		"\n"
		"</Configuration>\n"
		, proxy.xml())

	def _test_getValueFromStringAttributes(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		self.assertEqual("ValorPerDefecte", proxy["UnString"])
		self.assertEqual("Un altre valor per defecte", proxy["UnAltreString"])

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

	def _test_setValueFromStringAttributes(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		proxy["UnString"] = "newvalue"
		self.assertEqual("newvalue", proxy["UnString"])

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

	def _test_set_wrongTypeForString(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		try:
			proxy["UnString"] = 2
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals("str value expected, got int", e.args[0])

	def test_set_wrongTypeForInt(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithIntConfiguration")
		try:
			proxy["FirstInt"] = "WrongType"
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals("int value expected, got str", e.args[0])

	def test_set_wrongTypeForBool(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		try:
			proxy["BoolAttribute"] = 2.15
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals("bool value expected, got float", e.args[0])

	def test_set_wrongTypeForFloat(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		try:
			proxy["FloatAttribute"] = False
			self.fail("Exception expected")
		except TypeError, e:
			self.assertEquals("float value expected, got bool", e.args[0])

	def test_keys(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithStringConfiguration")
		self.assertEqual( ["UnString", "UnAltreString"], proxy.keys() )

	def test_nonDefault(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		proxy["IntAttribute"] = 27
		proxy["BoolAttribute"] = True
		self.assertEqual(True, proxy.nonDefault("IntAttribute"))
		self.assertEqual(True, proxy.nonDefault("BoolAttribute"))
		self.assertEqual(False, proxy.nonDefault("FloatAttribute"))

	def test_hold_apply(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxyWithProc("AudioSource")
		proxy.hold()
		proxy["NSources"] = 2
		self.assertEqual(1, proxy["NSources"])
		proxy.apply()
		self.assertEqual(2, proxy["NSources"])

if __name__ == '__main__':
	unittest.main()