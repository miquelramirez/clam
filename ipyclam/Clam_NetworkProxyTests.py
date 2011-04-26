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

	def test_addTwoProcessings(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "AudioSink"
		net.Processing2 = "AudioSource"
		self.assertEqual(['Processing1', 'Processing2'], proxy.processingNames())

	def test_hasProcessingWhenEmpty(self):
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		self.assertFalse(proxy.hasProcessing('NotAddedProcessing'))

	def test_hasProcessingWhenAddingTwo(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.AddedProcessing1 = "AudioSink"
		net.AddedProcessing2 = "AudioSource"
		self.assertTrue(proxy.hasProcessing('AddedProcessing1'))
		self.assertTrue(proxy.hasProcessing('AddedProcessing2'))

	def test_processingType(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.AddedProcessing1 = "AudioSink"
		self.assertEqual("AudioSink", proxy.processingType("AddedProcessing1"))


if __name__ == '__main__':
	unittest.main()