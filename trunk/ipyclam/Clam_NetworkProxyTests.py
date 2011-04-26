import Clam_NetworkProxy
import DummyProcessingsModule
import unittest
import TestFixtures

class Clam_NetworkProxyTests(unittest.TestCase):
	def test_availableTypes(self):
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		self.assertTrue('DummyProcessing' in proxy.availableTypes())

if __name__ == '__main__':
	unittest.main()