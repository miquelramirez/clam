import Clam_NetworkProxy
import DummyProcessingsModule
import unittest
import TestFixtures

class Clam_NetworkProxyTests(unittest.TestCase):

	def test_availableTypes(self):
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		self.assertTrue('DummyProcessing' in proxy.availableTypes())

	def test_addProcessing(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "DummyProcessing"
		self.assertEqual(
			"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
			"<Network clamVersion=\"1.4.1\" id=\"Unnamed Network\">\n"
			"\n"
			"  <processing id=\"Processing1\" type=\"DummyProcessing\"/>\n"
			"\n"
			"</Network>\n"
			, proxy.xml())

	def test_processingNamesWhenEmpty(self):
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		self.assertEqual([], proxy.processingNames())

	def test_processingNamesWhenAddingAProcessing(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "AudioSink"
		self.assertEqual(['Processing1'], proxy.processingNames())

if __name__ == '__main__':
	unittest.main()