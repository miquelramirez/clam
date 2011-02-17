import Connector
import Configuration
import Connectors

class Processing(object):
	def __init__(self, name, proxy):
		self.__dict__["name"] = name
		self.__dict__["type"] = proxy.processingType(name)
		self.__dict__["_config"] = Configuration.Configuration(proxy.processingConfig(self.name))
		self.__dict__["_inports"] = Connectors.Connectors(proxy, name, Connector.Port, Connector.In)
		self.__dict__["_outports"] = Connectors.Connectors(proxy, name, Connector.Port, Connector.Out)
		self.__dict__["_incontrols"] = Connectors.Connectors(proxy, name, Connector.Control, Connector.In)
		self.__dict__["_outcontrols"] = Connectors.Connectors(proxy, name, Connector.Control, Connector.Out)
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
		if name is 'type': raise AttributeError("Attribute 'type' is read only")
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

import unittest
import TestFixtures
class ProcessingTests(unittest.TestCase):

	def test_type(self) :
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.type, "AudioSource")

	def test_typeIsReadOnly(self) :
		p = Processing("Processing1", TestFixtures.proxy())
		with self.assertRaises(AttributeError) as contexManager :
			p.type="AnotherType"
		self.assertEquals("Attribute 'type' is read only", contexManager.exception.args[0])

	def test_name(self) :
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.name, "Processing1")

	def test_ConfigGettingAsDictionary(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p["ConfigParam1"], "default1")

	def test_ConfigSettingAndGettingAsDictionary(self):
		p = Processing("Processing1", TestFixtures.proxy())
		p["ConfigParam1"] = 'newvalue'
		self.assertEqual(p["ConfigParam1"], "newvalue")

	def test_configGettingAsAttribute(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.ConfigParam1, "default1")

	def test_configSettingAndGettingAsAttribute(self):
		p = Processing("Processing1", TestFixtures.proxy())
		p.ConfigParam1 = 'newvalue'
		self.assertEqual(p.ConfigParam1, "newvalue")

	def test_ConfigGettersBy_configInterface(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p._config.ConfigParam1, "default1")
		self.assertEqual(p._config["ConfigParam2"], "default2")

	def test_ConfigSettersBy_configInterface(self):
		p = Processing("Processing1", TestFixtures.proxy())
		p._config.ConfigParam1 = "newvalue1"
		p._config["ConfigParam2"] = "newvalue2"
		self.assertEqual(p._config.ConfigParam1, "newvalue1")
		self.assertEqual(p._config["ConfigParam2"], "newvalue2")

	def test_InportsGettingByNameAsDictionary(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p["InPort1"].name, "InPort1")
		self.assertEqual(p["InPort1"].kind, "Port")
		self.assertEqual(p["InPort1"].direction, "In")

	def test_InportsGettingByNameAsAttribute(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.InPort2.name, "InPort2")
		self.assertEqual(p.InPort2.kind, "Port")
		self.assertEqual(p.InPort2.direction, "In")

	def test_OutportsGettingByNameAsDictionary(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p["OutPort1"].name, "OutPort1")
		self.assertEqual(p["OutPort1"].kind, "Port")
		self.assertEqual(p["OutPort1"].direction, "Out")

	def test_OutportsGettingByNameAsAttribute(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.OutPort2.name, "OutPort2")
		self.assertEqual(p.OutPort2.kind, "Port")
		self.assertEqual(p.OutPort2.direction, "Out")

	def test_IncontrolsGettingByNameAsDictionary(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p["InControl1"].name, "InControl1")
		self.assertEqual(p["InControl1"].kind, "Control")
		self.assertEqual(p["InControl1"].direction, "In")

	def test_IncontrolsGettingByNameAsAttribute(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.InControl2.name, "InControl2")
		self.assertEqual(p.InControl2.kind, "Control")
		self.assertEqual(p.InControl2.direction, "In")

	def test_OutcontrolsGettingByNameAsDictionary(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p["OutControl1"].name, "OutControl1")
		self.assertEqual(p["OutControl1"].kind, "Control")
		self.assertEqual(p["OutControl1"].direction, "Out")

	def test_OutcontrolsGettingByNameAsAttribute(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEqual(p.OutControl2.name, "OutControl2")
		self.assertEqual(p.OutControl2.kind, "Control")
		self.assertEqual(p.OutControl2.direction, "Out")

	def test_dirFunction(self):
		p = Processing("Processing1", TestFixtures.proxy())
		self.assertEquals(sorted(
			['ConfigParam1', 'ConfigParam2', 
			'InPort1', 'InPort2','InPort3','InPort4',
			'OutPort1', 'OutPort2',
			'InControl1', 'InControl2',
			'OutControl1', 'OutControl2',
			'type', '_config', '_inports', '_outports', '_incontrols', '_outcontrols']), dir(p))

if __name__ == '__main__':
	unittest.main()
