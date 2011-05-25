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

	def test_getValueFromStringAttributes(self):
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

if __name__ == '__main__':
	unittest.main()