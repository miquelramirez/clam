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
		except AttributeError, e:
			self.assertEqual("WrongAttribute", e.args[0])

	def test_getValueFromIntAttributes(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithIntConfiguration")
		self.assertEqual(42, proxy["FirstInt"])
		self.assertEqual(15, proxy["SecondInt"])

	def test_getValueFromDifferentTypesOfAttributes(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		self.assertEqual(42, proxy["IntAttribute"])
		self.assertEqual('c', proxy["CharAttribute"])
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

	def test_getValueFromBoolAndFloatAttributes(self):
		proxy = Clam_ConfigurationProxy.createConfigurationProxy("DummyProcessingWithCompleteConfiguration")
		proxy["BoolAttribute"] = True
		proxy["FloatAttribute"] = 4.15
		self.assertEqual(True, proxy["BoolAttribute"])
		self.assertEqual(4.150000095367432, proxy["FloatAttribute"])

if __name__ == '__main__':
	unittest.main()