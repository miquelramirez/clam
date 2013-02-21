from ProcessingTypes import ProcessingTypes
import unittest
import TestFixtures

class ProcessingTypesTests(unittest.TestCase):
	def proxy(self):
		return TestFixtures.proxy()

	def test_ProcessingGettingAsAttribute(self) :
		type = ProcessingTypes(self.proxy())
		self.assertEqual("DummyControlSource", type.DummyControlSource)

	def test_BadProcessingGettingAsAttributeAndFail(self) :
		type = ProcessingTypes(self.proxy())
		try:
			type.BadProcessingType
			self.fail("Exception expected")
		except AttributeError, e:
			self.assertEqual(("BadProcessingType",), e.args)

	def test_dir(self) :
		type = ProcessingTypes(self.proxy())
		self.assertTrue("DummyControlSource" in dir(type))

class Clam_ProcessingTypesTests(ProcessingTypesTests):
	def proxy(self):
		import Clam_NetworkProxy
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		network = Network.Network(proxy)
		network.Processing1 = "DummyTypeProcessing1"
		network.Processing2 = "DummyTypeProcessing2"
		network.Processing1.OutPort1 > network.Processing2.Inport2
		network.Processing2.Outport2 > network.Processing1.InPort2
		network.Processing1.OutControl1 > network.Processing2.Incontrol2
		network.Processing1.OutControl2 > network.Processing2.Incontrol2
		network.Processing2.Outcontrol1 > network.Processing1.InControl2
		return proxy

if __name__ == "__main__":
	unittest.main()

