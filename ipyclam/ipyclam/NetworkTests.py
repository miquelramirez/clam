from Network import Network
import operator
import unittest
import TestFixtures

class NetworkTests(object):

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

	def test_ProcessingGettingAsDictionary(self) :
		net = Network(self.fixture1())
		self.assertEqual("proc1", net["proc1"].name)
		self.assertEqual("proc2", net["proc2"].name)

	def test_ProcessingGettingAsAttribute(self) :
		net = Network(self.fixture1())
		self.assertEqual("proc1", net.proc1.name)
		self.assertEqual("proc2", net.proc2.name)

	def test_dirFunction(self) :
		net = Network(self.fixture1())
		self.assertEquals(["description", "proc1", "proc2"], dir(net))

	def test_processingNames(self):
		net = Network(self.fixture1())
		self.assertEquals(["proc1", "proc2"], net.processingNames())

	def test_ProcessingAsAttributesGettingAndFailing(self):
		net = Network(self.fixture1())
		self.assertRaises(AttributeError, getattr, net, "NonExistingProcessing")

	def test_ValuesAsDictionaryGettingAndFailing(self):
		net = Network(self.fixture1())
		self.assertRaises(KeyError, operator.getitem, net, "NonExistingProcessing")

	def test_codeEmptyNetwork(self) :
		net = Network(self.empty())
		self.assertEquals("\n\n\n", net.code())

	def test_addProcessing(self) :
		net = Network(self.empty())
		net.processing1 = "DummyPortSource"
		self.assertEquals(
			"network.processing1 = 'DummyPortSource'\n"
			"\n"
			"\n"
			, net.code())

	def test_addProcessingAsItem(self) :
		net = Network(self.empty())
		net["processing1"] = "DummyPortSource"
		self.assertEquals(
			"network.processing1 = 'DummyPortSource'\n"
			"\n"
			"\n"
			, net.code())

	def test_addTwoProcessingsSameType(self) :
		net = Network(self.empty())
		net.processing1 = "DummyPortSource"
		net.processing2 = "DummyPortSource"
		self.assertEquals(
			"network.processing1 = 'DummyPortSource'\n"
			"network.processing2 = 'DummyPortSource'\n"
			"\n"
			"\n"
			, net.code())

	def test_addTwoProcessingsDifferentType(self) :
		net = Network(self.empty())
		net.processing1 = "DummyPortSource"
		net.processing2 = "DummyPortSink"
		self.assertEquals(
			"network.processing1 = 'DummyPortSource'\n"
			"network.processing2 = 'DummyPortSink'\n"
			"\n"
			"\n"
			, net.code())

	def test_addPortSourceAndPortSinkAndConnectPorts(self) :
		net = Network(self.empty())
		net.processing1 = "DummyPortSource"
		net.processing2 = "DummyPortSink"
		net.processing1.OutPort1 > net.processing2.InPort1
		self.assertEquals(
			"network.processing1 = 'DummyPortSource'\n"
			"network.processing2 = 'DummyPortSink'\n"
			"\n"
			"network.processing1.OutPort1 > network.processing2.InPort1\n"
			, net.code())

	def test_addControlSourceAndControlSinkAndConnectControls(self) :
		net = Network(self.empty())
		net.processing1 = "DummyControlSource"
		net.processing2 = "DummyControlSink"
		net.processing1.OutControl1 > net.processing2.InControl1
		self.assertEquals(
			"network.processing1 = 'DummyControlSource'\n"
			"network.processing2 = 'DummyControlSink'\n"
			"\n"
			"\n"
			"network.processing1.OutControl1 > network.processing2.InControl1"
			, net.code())

	def test_addPortControlSourceAndPortControlSinkAndConnectPortsAndControls(self) :
		net = Network(self.empty())
		net.processing1 = "DummyPortSource"
		net.processing2 = "DummyPortSink"
		net.processing3 = "DummyControlSource"
		net.processing4 = "DummyControlSink"
		net.processing1.OutPort1 > net.processing2.InPort1
		net.processing3.OutControl1 > net.processing4.InControl1
		self.assertEquals(
			"network.processing1 = 'DummyPortSource'\n"
			"network.processing2 = 'DummyPortSink'\n"
			"network.processing3 = 'DummyControlSource'\n"
			"network.processing4 = 'DummyControlSink'\n"
			"\n"
			"network.processing1.OutPort1 > network.processing2.InPort1\n"
			"network.processing3.OutControl1 > network.processing4.InControl1"
			, net.code())

	def test_types(self) :
		net = Network(self.empty())
		self.assertEqual("DummyControlSink", net.types.DummyControlSink)

	def test_setDescription(self):
		net = Network(self.empty())
		net.description = "A description"
		self.assertEquals("A description", net.description)

	def test_changeNetworkVarForCode(self):
		net = Network(self.empty())
		net.processing1 = "DummyControlSource"
		net.processing2 = "DummyControlSink"
		net.processing1.OutControl1 > net.processing2.InControl1
		self.assertEquals(
			"net.processing1 = 'DummyControlSource'\n"
			"net.processing2 = 'DummyControlSink'\n"
			"\n"
			"\n"
			"net.processing1.OutControl1 > net.processing2.InControl1"
			, net.code("net"))

	def test_deleteProcessingAsAttribute(self):
		net = Network(self.empty())
		net.processing1 = "DummyPortSource"
		net.processing2 = "DummyPortSink"
		del net.processing1
		self.assertEquals(
			"network.processing2 = 'DummyPortSink'\n"
			"\n"
			"\n"
			, net.code())

	def test_deleteProcessingAsItem(self):
		net = Network(self.empty())
		net["processing1"] = "DummyPortSource"
		net["processing2"] = "DummyPortSink"
		del net["processing1"]
		self.assertEquals(
			"network.processing2 = 'DummyPortSink'\n"
			"\n"
			"\n"
			, net.code())

	def test_codeWhenNotAlphaNumericProcessingAndConnectorNames(self) :
		net = Network(self.empty())
		net.processing1 = "ProcessingWithNameSpacedPorts"
		net.processing2 = "ProcessingWithNameSpacedPorts"
		net.processing3 = "ProcessingWithNameSpacedControls"
		net.processing4 = "ProcessingWithNameSpacedControls"
		net.processing1["An outport"] > net.processing2["An inport"]
		net.processing3["An outcontrol"] > net.processing4["An incontrol"]
		self.assertEquals(
			"network.processing1 = 'ProcessingWithNameSpacedPorts'\n"
			"network.processing2 = 'ProcessingWithNameSpacedPorts'\n"
			"network.processing3 = 'ProcessingWithNameSpacedControls'\n"
			"network.processing4 = 'ProcessingWithNameSpacedControls'\n"
			"\n"
			"network.processing1[\"An outport\"] > network.processing2[\"An inport\"]\n"
			"network.processing3[\"An outcontrol\"] > network.processing4[\"An incontrol\"]"
			, net.code())

	def test_code_whenNameHasUnderlines(self) :
		net = Network(self.empty())
		net.name_with_underlines = "DummyPortSource"
		self.assertEquals(
			"network.name_with_underlines = 'DummyPortSource'\n"
			"\n"
			"\n"
			, net.code())

	def test_code_whenNameIsAKeyword(self) :
		net = Network(self.empty())
		net["while"] = "DummyPortSource"
		self.assertEquals(
			"network[\"while\"] = 'DummyPortSource'\n"
			"\n"
			"\n"
			, net.code())

	def test_addProcessingWithName_code_AndFail(self):
		net = Network(self.empty())
		try:
			net.code = "DummyPortSource"
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals("Wrong processing name: code is a method", e.__str__())

	def test_addProcessingWithName_types_AndFail(self):
		net = Network(self.empty())
		try:
			net.types = "DummyPortSource"
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals("Wrong processing name: types is a method", e.__str__())

	def test_codeShowsDescription(self):
		net = Network(self.empty())
		net.description = "A description"
		net.proc1 = "DummyPortSink"
		self.assertEquals("A description", net.description)
		self.assertEquals(
			"network.description = 'A description'\n"
			"network.proc1 = 'DummyPortSink'\n"
			"\n"
			"\n"
			, net.code())

	def test_codeForNumericConnectors(self):
		net = Network(self.empty())
		net.proc1 = "ProcessingWithNumericPorts"
		net.proc2 = "ProcessingWithNumericPorts"
		net.proc1["2"] > net.proc2["1"]
		self.assertEquals(
			"network.proc1 = 'ProcessingWithNumericPorts'\n"
			"network.proc2 = 'ProcessingWithNumericPorts'\n"
			"\n"
			"network.proc1[\"2\"] > network.proc2[\"1\"]\n"
			, net.code())

	def test_in(self) :
		net = Network(self.empty())
		self.assertFalse("proc1" in net)
		net.proc1 = "DummyProcessingWithStringConfiguration"
		self.assertTrue("proc1" in net)

	def test_renameProcessing(self):
		net = Network(self.empty())
		net.NameToBeChanged = "DummyProcessingWithStringConfiguration"
		net.NameToBeChanged.name = "NewName"
		self.assertTrue('NameToBeChanged' not in net)
		self.assertTrue('NewName' in net)

	def test_connect_slice(self):
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1._outports[::2] > net.proc2._inports[::2]
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport1\n"
			"network.proc1.outport3 > network.proc2.inport3\n"
			"network.proc1.outport5 > network.proc2.inport5\n"
			, net.code())

	def test_connect_slice_reverse(self):
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1._outports[::2] > net.proc2._inports[::-2]
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport6\n"
			"network.proc1.outport3 > network.proc2.inport4\n"
			"network.proc1.outport5 > network.proc2.inport2\n"
			, net.code())

	# processing, connectors, slice, connector

	# broadcast connections one to many
	def test_connect_portToSlice(self):
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1.outport3 > net.proc2._inports[::2]
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport3 > network.proc2.inport1\n"
			"network.proc1.outport3 > network.proc2.inport3\n"
			"network.proc1.outport3 > network.proc2.inport5\n"
			, net.code())

	def test_connect_sliceToPort(self):
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1._outports[::2] > net.proc2.inport3
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport3\n"
			"network.proc1.outport3 > network.proc2.inport3\n"
			"network.proc1.outport5 > network.proc2.inport3\n"
			, net.code())

	def test_connect_portToImports(self):
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1.outport3 > net.proc2._inports
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport3 > network.proc2.inport1\n"
			"network.proc1.outport3 > network.proc2.inport2\n"
			"network.proc1.outport3 > network.proc2.inport3\n"
			"network.proc1.outport3 > network.proc2.inport4\n"
			"network.proc1.outport3 > network.proc2.inport5\n"
			"network.proc1.outport3 > network.proc2.inport6\n"
			, net.code())

	def test_connect_outportsToPort(self):
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1._outports > net.proc2.inport3
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport3\n"
			"network.proc1.outport2 > network.proc2.inport3\n"
			"network.proc1.outport3 > network.proc2.inport3\n"
			"network.proc1.outport4 > network.proc2.inport3\n"
			"network.proc1.outport5 > network.proc2.inport3\n"
			"network.proc1.outport6 > network.proc2.inport3\n"
			, net.code())

	def test_connect_portToProcessing(self):
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1.outport3 > net.proc2
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport3 > network.proc2.inport1\n"
			"network.proc1.outport3 > network.proc2.inport2\n"
			"network.proc1.outport3 > network.proc2.inport3\n"
			"network.proc1.outport3 > network.proc2.inport4\n"
			"network.proc1.outport3 > network.proc2.inport5\n"
			"network.proc1.outport3 > network.proc2.inport6\n"
			, net.code())

	def test_connect_processingToPort(self):
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1 > net.proc2.inport3
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport3\n"
			"network.proc1.outport2 > network.proc2.inport3\n"
			"network.proc1.outport3 > network.proc2.inport3\n"
			"network.proc1.outport4 > network.proc2.inport3\n"
			"network.proc1.outport5 > network.proc2.inport3\n"
			"network.proc1.outport6 > network.proc2.inport3\n"
			, net.code())

	def test_connect_outportsToSlice(self):
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1._outports > net.proc2._inports[::2]
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport1\n"
			"network.proc1.outport2 > network.proc2.inport3\n"
			"network.proc1.outport3 > network.proc2.inport5\n"
			, net.code())

	def test_connect_sliceToInports(self):
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1._outports[::2] > net.proc2._inports
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport1\n"
			"network.proc1.outport3 > network.proc2.inport2\n"
			"network.proc1.outport5 > network.proc2.inport3\n"
			, net.code())

	def test_connect_processingToSlice(self):
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1 > net.proc2._inports[::2]
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport1\n"
			"network.proc1.outport2 > network.proc2.inport3\n"
			"network.proc1.outport3 > network.proc2.inport5\n"
			, net.code())

	def test_connect_sliceToProcessing(self):
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1._outports[::2] > net.proc2
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport1\n"
			"network.proc1.outport3 > network.proc2.inport2\n"
			"network.proc1.outport5 > network.proc2.inport3\n"
			, net.code())

	def test_change_config_attributes(self):
		net = Network(self.fixture1())
		net.proc1["ConfigParam1"] = 'newvalue'
		self.assertEqual(net.proc1["ConfigParam1"], "newvalue")

	def test_code_for_changing_config_attributes(self):
		net = Network(self.empty())
		net.proc1 = "DummyProcessingWithStringConfiguration"
		net.proc1.AString = 'newvalue'
		self.assertMultiLineEqual(
			"network.proc1 = 'DummyProcessingWithStringConfiguration'\n"
			"network.proc1['AString'] = 'newvalue'\n"
			"network.proc1['OtherString'] = 'Another default value'\n"
			"\n\n"
			, net.code(fullConfig=True))

	def test_config_notPersistent(self):
		net = Network(self.empty())
		net.proc1 = "DummyProcessingWithStringConfiguration"
		net.proc1.AString = 'newvalue'
		net.proc2 = "DummyProcessingWithStringConfiguration"
		self.assertEquals(
			"network.proc1 = 'DummyProcessingWithStringConfiguration'\n"
			"network.proc2 = 'DummyProcessingWithStringConfiguration'\n"
			"network.proc1['AString'] = 'newvalue'\n"
			"network.proc1['OtherString'] = 'Another default value'\n"
			"network.proc2['AString'] = 'DefaultValue'\n"
			"network.proc2['OtherString'] = 'Another default value'\n"
			"\n\n"
			, net.code(fullConfig=True))

	def test_withClause_holdsConfiguration(self):
		net = Network(self.empty())
		net.proc1 = "DummyProcessingWithStringConfiguration"
		with net.proc1._config as c :
			c.AString = 'newvalue'
			c.OtherString = 'othernewvalue'
			self.assertMultiLineEqual(
				"network.proc1 = 'DummyProcessingWithStringConfiguration'\n"
				"network.proc1['AString'] = 'DefaultValue'\n"
				"network.proc1['OtherString'] = 'Another default value'\n"
				"\n\n"
				, net.code(fullConfig=True))
		self.assertMultiLineEqual(
			"network.proc1 = 'DummyProcessingWithStringConfiguration'\n"
			"network.proc1['AString'] = 'newvalue'\n"
			"network.proc1['OtherString'] = 'othernewvalue'\n"
			"\n\n"
			, net.code(fullConfig=True))

	def test_connect_from_processing_to_processing(self):
		net = Network(self.empty())
		net.proc1 = "Dummy2IOPortsControls"
		net.proc2 = "Dummy2IOPortsControls"
		self.assertEquals(4, net.proc1 > net.proc2)
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy2IOPortsControls'\n"
			"network.proc2 = 'Dummy2IOPortsControls'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport1\n"
			"network.proc1.outport2 > network.proc2.inport2\n"
			"network.proc1.outcontrol1 > network.proc2.incontrol1\n"
			"network.proc1.outcontrol2 > network.proc2.incontrol2"
			, net.code())

"""
	Tests for Network isStopped, isPlaying, isPaused
"""

class NetworkTests_Dummy(NetworkTests, unittest.TestCase):
	def empty(self):
		import Dummy_Engine
		return Dummy_Engine.Dummy_Engine()

	@unittest.skip("Not working yet")
	def test_withClause_holdsConfiguration(self):
		pass

class NetworkTests_Clam(NetworkTests, unittest.TestCase):
	def empty(self):
		import Clam_Engine
		return Clam_Engine.Clam_Engine()

	def test_connect_outportsToPort(self):
		"CLAM limits inports connections"
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1._outports > net.proc2.inport3
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport3\n"
			, net.code())

	def test_connect_processingToPort(self):
		"CLAM limits inports connections"
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1 > net.proc2.inport3
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport3\n"
			, net.code())

	def test_connect_sliceToPort(self):
		"CLAM limits inports connections"
		net = Network(self.empty())
		net.proc1 = "Dummy6IOPorts"
		net.proc2 = "Dummy6IOPorts"
		net.proc1._outports[::2] > net.proc2.inport3
		self.maxDiff = 1000
		self.assertMultiLineEqual(
			"network.proc1 = 'Dummy6IOPorts'\n"
			"network.proc2 = 'Dummy6IOPorts'\n"
			"\n"
			"network.proc1.outport1 > network.proc2.inport3\n"
			, net.code())


	def test_processingConfig(self):
		net = Network(self.empty())
		net.proc1 = "DummyProcessingWithCompleteConfiguration"
		self.assertEquals(42, net.proc1.IntAttribute)

	def test_set_config_attribute(self) :
		net = Network(self.empty())
		net.p = "AudioSource"
		self.assertEquals(['1'], net.p._outports.__dir__() )
		net.p.NSources = 2
		self.assertEquals(['1', '2'], net.p._outports.__dir__() )
		self.assertEquals(2, net.p.NSources)

	def test_set_config_attribute_with_hold_apply(self) :
		net = Network(self.empty())
		net.p = "AudioSource"
		c = net.p._config
		c.hold()
		c.NSources = 2
		self.assertEquals(1, net.p.NSources)
		c.apply()
		self.assertEquals(2, net.p.NSources)

	def test_clone_and_apply(self) :
		net = Network(self.empty())
		net.p = "AudioSource"
		c = net.p._config.clone()
		c.NSources = 2
		self.assertEquals(1, net.p.NSources)
		self.assertEquals(2, c.NSources)
		net.p._config = c
		self.assertEquals(2, net.p.NSources)


if __name__ == '__main__':
	unittest.main()
