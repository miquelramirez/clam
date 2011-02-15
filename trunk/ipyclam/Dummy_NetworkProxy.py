import unittest
import Connector

class Dummy_NetworkProxy :
	def __init__(self, definition) :
		self._processings = dict()
		for process in definition:
			self._processings[process['name']] = process
	def processingType(self, name) :
		return self._processings[name]["type"]
	def processingConfig(self, name) :
		return self._processings[name]["config"]
	def processingInports(self, name) :
		return self._processings[name]["inports"]
	def processingOutports(self, name) :
		return self._processings[name]["outports"]
	def processingIncontrols(self, name) :
		return self._processings[name]["incontrols"]
	def processingOutcontrols(self, name) :
		return self._processings[name]["outcontrols"]
	def processingConnectors(self, name, kind, direction) :
		nameKinds = [
			(Connector.Port, Connector.In, 'inports'),
			(Connector.Port, Connector.Out, 'outports'),
			(Connector.Control, Connector.In, 'incontrols'),
			(Connector.Control, Connector.Out, 'outcontrols'),
		]
		mapping = dict([((k,d),n) for k,d,n in nameKinds])
		return self._processings[name][mapping[kind,direction]]


class Dummy_NetworkProxyTest(unittest.TestCase) :
	def definition(self) :
		return [
			dict(
				type = "AudioSource",
				name = "Processing1",
				config = dict(
					ConfigParam1 = "default1",
					ConfigParam2 = "default2",
				),
				inports = [
					["Inport1", "type1"],
					["Inport2", "type1"]
				],
				outports = [
					["Outport1", "type1"],
					["Outport2", "type1"]
				],
				incontrols = [
					["Incontrol1", "type2"],
					["Incontrol2", "type2"]
				],
				outcontrols = [
					["Outcontrol1", "type2"],
					["Outcontrol2", "type2"]
				],
			),
			dict(
				type = "AudioSink",
				name = "Processing2",
				config = dict(
					ConfigParam1 = "default1",
					ConfigParam2 = "default2",
				),
				inports = [
					["Inport1", "type1"],
					["Inport2", "type1"]
				],
				outports = [
					["Outport1", "type1"],
					["Outport2", "type1"]
				],
				incontrols = [
					["Incontrol1", "type2"],
					["Incontrol2", "type2"]
				],
				outcontrols = [
					["Outcontrol1", "type2"],
					["Outcontrol2", "type2"]
				],
			),
		]

	def test_type(self) :
		proxy = Dummy_NetworkProxy(self.definition())
		self.assertEquals("AudioSource", proxy.processingType("Processing1"))

	def test_config(self) :
		proxy = Dummy_NetworkProxy(self.definition())
		self.assertEquals(dict(
				ConfigParam1 = "default1",
				ConfigParam2 = "default2",
			), proxy.processingConfig("Processing1"))

	def test_inports(self) :
		proxy = Dummy_NetworkProxy(self.definition())
		self.assertEquals([
				["Inport1", "type1"],
				["Inport2", "type1"]
			], proxy.processingConnectors(
				"Processing1", Connector.Port, Connector.In))

	def test_outports(self) :
		proxy = Dummy_NetworkProxy(self.definition())
		self.assertEquals([
				["Outport1", "type1"],
				["Outport2", "type1"]
			], proxy.processingConnectors(
				"Processing1", Connector.Port, Connector.Out))

	def test_incontrols(self) :
		proxy = Dummy_NetworkProxy(self.definition())
		self.assertEquals([
				["Incontrol1", "type2"],
				["Incontrol2", "type2"]
			], proxy.processingConnectors(
				"Processing1", Connector.Control, Connector.In))

	def test_outcontrols(self) :
		proxy = Dummy_NetworkProxy(self.definition())
		self.assertEquals([
				["Outcontrol1", "type2"],
				["Outcontrol2", "type2"]
			], proxy.processingConnectors(
				"Processing1", Connector.Control, Connector.Out))

	def test_withBadProcessingName(self) :
		proxy = Dummy_NetworkProxy(self.definition())
		self.assertRaises(KeyError, proxy.processingType, "Processing3")

	def test_typeSecondProcessing(self) :
		proxy = Dummy_NetworkProxy(self.definition())
		self.assertEquals("AudioSink", proxy.processingType("Processing2"))

if __name__ == '__main__':
	unittest.main()

