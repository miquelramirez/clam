from ProcessingTypes import ProcessingTypes
import unittest
import TestFixtures

class ProcessingTypesTests(unittest.TestCase):
	def empty(self):
		import Dummy_Engine
		return Dummy_Engine.Dummy_Engine()

	def fixture1(self):
		engine = self.empty()
		engine.addProcessing("DummyTypeProcessing1", "proc1")
		engine.addProcessing("DummyTypeProcessing2", "proc2")
		engine.connect("Port","proc1","OutPort1","proc2","Inport2")
		engine.connect("Port","proc2","Outport2","proc1","InPort2")
		engine.connect("Control","proc1","OutControl1","proc2","Incontrol2")
		engine.connect("Control","proc1","OutControl2","proc2","Incontrol2")
		engine.connect("Control","proc2","Outcontrol1","proc1","InControl2")
		return engine

	def test_ProcessingGettingAsAttribute(self) :
		type = ProcessingTypes(self.fixture1())
		self.assertEqual("DummyControlSource", type.DummyControlSource)

	def test_BadProcessingGettingAsAttributeAndFail(self) :
		type = ProcessingTypes(self.fixture1())
		try:
			type.BadProcessingType
			self.fail("Exception expected")
		except AttributeError, e:
			self.assertEqual(("BadProcessingType",), e.args)

	def test_dir(self) :
		type = ProcessingTypes(self.fixture1())
		self.assertTrue("DummyControlSource" in dir(type))

class Clam_ProcessingTypesTests(ProcessingTypesTests):
	def empty(self):
		import Clam_Engine
		return Clam_Engine.Clam_Engine()

if __name__ == "__main__":
	unittest.main()

