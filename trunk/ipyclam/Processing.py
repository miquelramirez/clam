import unittest
from Dummy_NetworkProxy import Dummy_NetworkProxy
from Configuration import Configuration
from Connector import Connector
from Connectors import Connectors

class Processing(object):
	def __init__(self, name, proxy):
		self.__dict__["name"] = name
		self.__dict__["type"] = proxy.processingType(self.name)
		self.__dict__["_config"] = Configuration(proxy.processingConfig(self.name))
		self.__dict__["_inports"] = Connectors(Connector.Port, Connector.In, proxy.processingInports(self.name))
		self.__dict__["_outports"] = Connectors(Connector.Port, Connector.Out, proxy.processingOutports(self.name))
		self.__dict__["_incontrols"] = Connectors(Connector.Control, Connector.In, proxy.processingIncontrols(self.name))
		self.__dict__["_outcontrols"] = Connectors(Connector.Control, Connector.Out, proxy.processingOutcontrols(self.name))
	def __getitem__(self, name):
		if name in dir(self._config):
			return self._config[name]
		if name in dir(self._inports):
			return self._inports[name]
		if name in dir(self._outports):
			return self._outports[name]
		if name in dir(self._incontrols):
			return self._incontrols[name]
		if name in dir(self._outcontrols):
			return self._outcontrols[name]
		raise KeyError(name)
	def __setitem__(self, name, value):
		self._config[name] = value
	def __getattr__(self, name):
		try:
			return self.__getitem__(name)
		except KeyError:
			raise AttributeError(name)
	def __setattr__(self, name, value):
		self.__setitem__(name, value)
	def __dir__(self):
		return (
			dir(self._config) + 
			dir(self._inports) + 
			dir(self._outports) + 
			dir(self._incontrols) + 
			dir(self._outcontrols) + 
			[
				"type" , 
				"_config", 
				"_inports", 
				"_outports", 
				"_incontrols", 
				"_outcontrols",
			]
		)

class ProcessingTests(unittest.TestCase):
	def proxy(self) :
		return Dummy_NetworkProxy([
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
		])
	def test_type(self) :
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.type, "AudioSource")

	def test_set_type(self) :
		p = Processing("Processing1", self.proxy())
		p.type="AnotherType"
		self.assertEqual(p.type, "AudioSource")
	
	def test_name(self) :
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.name, "Processing1")
	def test_ConfigGettingAsDictionary(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p["ConfigParam1"], "default1")
	def test_ConfigSettingAndGettingAsDictionary(self):
		p = Processing("Processing1", self.proxy())
		p["ConfigParam1"] = 'newvalue'
		self.assertEqual(p["ConfigParam1"], "newvalue")
	def test_configGettingAsAttribute(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.ConfigParam1, "default1")
	def test_configSettingAndGettingAsAttribute(self):
		p = Processing("Processing1", self.proxy())
		p.ConfigParam1 = 'newvalue'
		self.assertEqual(p.ConfigParam1, "newvalue")
	def test_ConfigGettersBy_configInterface(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p._config.ConfigParam1, "default1")
		self.assertEqual(p._config["ConfigParam2"], "default2")
	def test_ConfigSettersBy_configInterface(self):
		p = Processing("Processing1", self.proxy())
		p._config.ConfigParam1 = "newvalue1"
		p._config["ConfigParam2"] = "newvalue2"
		self.assertEqual(p._config.ConfigParam1, "newvalue1")
		self.assertEqual(p._config["ConfigParam2"], "newvalue2")
	def test_InportsGettingByNameAsDictionary(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p["Inport1"].name, "Inport1")
		self.assertEqual(p["Inport1"].kind, "Port")
		self.assertEqual(p["Inport1"].direction, "In")
	def test_InportsGettingByNameAsAttribute(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.Inport2.name, "Inport2")
		self.assertEqual(p.Inport2.kind, "Port")
		self.assertEqual(p.Inport2.direction, "In")
	def test_OutportsGettingByNameAsDictionary(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p["Outport1"].name, "Outport1")
		self.assertEqual(p["Outport1"].kind, "Port")
		self.assertEqual(p["Outport1"].direction, "Out")
	def test_OutportsGettingByNameAsAttribute(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.Outport2.name, "Outport2")
		self.assertEqual(p.Outport2.kind, "Port")
		self.assertEqual(p.Outport2.direction, "Out")
	def test_IncontrolsGettingByNameAsDictionary(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p["Incontrol1"].name, "Incontrol1")
		self.assertEqual(p["Incontrol1"].kind, "Control")
		self.assertEqual(p["Incontrol1"].direction, "In")
	def test_IncontrolsGettingByNameAsAttribute(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.Incontrol2.name, "Incontrol2")
		self.assertEqual(p.Incontrol2.kind, "Control")
		self.assertEqual(p.Incontrol2.direction, "In")
	def test_OutcontrolsGettingByNameAsDictionary(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p["Outcontrol1"].name, "Outcontrol1")
		self.assertEqual(p["Outcontrol1"].kind, "Control")
		self.assertEqual(p["Outcontrol1"].direction, "Out")
	def test_OutcontrolsGettingByNameAsAttribute(self):
		p = Processing("Processing1", self.proxy())
		self.assertEqual(p.Outcontrol2.name, "Outcontrol2")
		self.assertEqual(p.Outcontrol2.kind, "Control")
		self.assertEqual(p.Outcontrol2.direction, "Out")
	def test_dirFunction(self):
		p = Processing("Processing1", self.proxy())
		self.assertEquals(sorted(
			['ConfigParam1', 'ConfigParam2', 
			'Inport1', 'Inport2',
			'Outport1', 'Outport2',
			'Incontrol1', 'Incontrol2',
			'Outcontrol1', 'Outcontrol2',
			'type', '_config', '_inports', '_outports', '_incontrols', '_outcontrols']), dir(p))

if __name__ == '__main__':
	unittest.main()
