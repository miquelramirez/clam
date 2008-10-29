#!/usr/bin/python

from xml.etree import ElementTree, ElementPath
import xml.dom
import sys

class ClamNetwork() :
	def __init__ (self, networkfile) :
		self.document = ElementTree.parse(networkfile)
		self.processings = self.document.findall("processing")
		self.connections = dict(
			incontrol = self.document.findall("control_connection/in"),
			outcontrol = self.document.findall("control_connection/out"),
			inport = self.document.findall("port_connection/in"),
			outport = self.document.findall("port_connection/out"),
			)
		self.log = []
		self.verbose = False
	def beVerbose(self, shouldBe=True) :
		self.verbose = shouldBe
	def dump(self, file=sys.stdout) :
		xml.etree.ElementTree.dump(self.document)
	def dumpLog(self, file=sys.stdout) :
		print >> file, "\n".join(self.log)
	def _log(self, message) :
		if self.verbose : print message
		self.log.append(message)

	def renameClass(self, oldType, newType) :
		for processing in self.processings : 
			if processing.get('type') != oldType : continue
			processing.set('type', newType)
			self._log("Retyping processing %s: %s -> %s" %(
				processing.get("id"), oldType, newType))

	def namesForType(self, type) :
		return [ processing.get("id")
			for processing in self.processings
			if processing.get('type') == type ]

	def renameConnector(self, processingType, connectorKind, oldName, newName) :
		names = self.namesForType(processingType)
		for connection in self.connections["inport"] :
			processing, connector = connection.text.split(".")
			if processing not in names: continue
			if connector != oldName: continue
			connection.text = ".".join([processing, newName])
			self._log("Updating  %s: %s.%s -> %s.%s" %(
				connectorKind, processing, oldName, processing, newName))

	def renameConfig(self, processingType, oldName, newName) :
		"""Change the name of a config parameter"""
		for processing in self.processings : 
			if processing.get('type') != processingType : continue
			configs = processing.findall(oldName)
			for config in configs :
				config.tag = newName
				self._log("Renaming configuration parameter for processing %s: %s -> %s" %(
				processing.get("id"), oldName, newName))

	def setConfigByType(self, processingType, name, value) :
		"""Change the value for a config parameter"""
		for processing in self.processings : 
			if processing.get('type') != processingType : continue
			configs = processing.findall(name)
			for config in configs :
				config.text = value
				self._log("Revaluing configuration parameter %s.%s = %s" %(
					processing.get("id"), name, value))

	def setConfig(self, processingId, param, value) :
		"""Change the value for a config parameter"""
		for processing in self.processings : 
			if processing.get('id') != processingId : continue
			configs = processing.findall(param)
			for config in configs :
				config.text = value
				self._log("Revaluing configuration parameter %s.%s = %s" %(
					processing.get("id"), param, value))

	def addConfig(self, processingType, name, default) :
		"""Change the name of a config parameter"""
		for processing in self.processings :
			if processing.get('type') != processingType : continue
			if processing.findall(name) : continue # Already present, don't add
			parameterElement = ElementTree.Element(name) 
			parameterElement.text = unicode(default)
			processing.append(parameterElement)
			self._log("Adding configuration parameter %s.%s = %s" %(
				processing.get("id"), name, default))

	def removeConfig(self, processingType, name) :
		for processing in self.processings :
			if processing.get('type') != processingType : continue
			for param in processing.findall(name) :
				if param.tag != name : continue
				processing.remove(param) 
				self._log("Removing configuration parameter %s.%s" %(
					processing.get("id"), name))
	def moveConfig(self, processingType, name, beforeTag=None) :
		for processing in self.processings :
			if processing.get('type') != processingType : continue
			for param in processing.findall(name) :
				processing.remove(param) 
				self._log("Removing configuration parameter %s.%s" %(
					processing.get("id"), name))


if __name__ == "__main__" :
	network = ClamNetwork(file("FilePlayer.clamnetwork"))
	network.beVerbose()
	network.renameClass('OutControlSender', 'Foo')
	network.renameConnector('AudioMixer', "inport", "Input 0", "lala")
	network.renameConfig('AudioMixer', "NumberOfInPorts", "NInputs")
	network.setConfigByType('AudioMixer', "NInputs", "14")
	network.setConfig('Gain 0', "Max", "2")
	network.addConfig('AudioMixer', 'Ramping', '1')
	network.addConfig('AudioMixer', 'NInputs', '96')
	network.removeConfig('AudioMixer', 'FrameSize')
	network.dump()


