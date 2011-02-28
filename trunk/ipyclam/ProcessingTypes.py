class ProcessingTypes(object):

	def __init__(self, proxy):
		self._proxy = proxy

	def __getattr__(self, name) :
		types = self._proxy.availableTypes()
		if name not in types:
			raise AttributeError(name)
		return types[types.index(name)]

	def __dir__(self) :
		return self._proxy.availableTypes()


import unittest
import TestFixtures

class ProcessingTypesTests(unittest.TestCase):

	def test_ProcessingGettingAsAttribute(self) :
		type = ProcessingTypes(TestFixtures.proxy())
		self.assertEqual("MinimalProcessing", type.MinimalProcessing)

	def test_BadProcessingGettingAsAttributeAndFail(self) :
		type = ProcessingTypes(TestFixtures.proxy())
		try:
			type.BadProcessingType
			self.fail("Exception expected")
		except AttributeError, e:
			self.assertEqual(("BadProcessingType",), e.args)

	def test_dir(self) :
		type = ProcessingTypes(TestFixtures.proxy())
		self.assertEqual(["ControlSink", "ControlSource", "MinimalProcessing", "OtherControlSink", "PortSink", "PortSource"], dir(type))

if __name__ == "__main__":
	unittest.main()

