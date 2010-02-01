#!/usr/bin/python

# This script can be used, as a command line tool or as a python module,
# to performs a set of high level transformations on CLAM networks.
#
# The script is useful to upgrade the network to changes on C++
# code and also to perform automatized parameter changes from the
# commandline (not using the NetworkEditor).
#

# DONE: Basic operaions
# DONE: Making it easy to use on command line, not just as library
# DONE: Reading 'migration scripts' with a set of those commands
# DONE: Add CLAM version information into the networks and check it.
# TODO: More operators: reorderConfig, renameProcessingInstance...
# TODO: Being able to update also ui files


from xml.etree import ElementTree, ElementPath
import xml.dom
import sys
import re
import copy

class NotACommand(Exception) :
	def __init__(self, command, available) :
		self.command = command
		self.available = available
	def __str__(self) : 
		return "'%s' is not an available command. Try with: %s"%(
			self.command, ", ".join(self.available))

class ClamNetwork() :

	def __init__ (self, networkfile) :
		self.document = ElementTree.parse(networkfile)
		self.processings = self.document.findall("processing")
		self.connections = dict(
			incontrol = self.document.findall("control_connection/in"),
			outcontrol =  self.document.findall("control_connection/out"),
			inport = self.document.findall("port_connection/in"),
			outport = self.document.findall("port_connection/out"),
			)
		self.version = self.document.find("/").attrib.get("clamVersion","1.3.0")
		self.log = []
		self.verbose = False
		self.ensuredVersion = ""
		self.modified = False

	def _featuredVersion(self, fullVersion) :
		feature = ".".join(fullVersion.split(".")[:2])
	def runCommand(self, command) :
		import shlex
		tokens = shlex.split(command)
		try: method=getattr(self, tokens[0])
		except AttributeError:
			raise NotACommand(tokens[0], self._availableCommands())
		method(*tokens[1:])

	def _log(self, message) :
		if self.verbose : print >> sys.stderr,message
		self.modified = True
		self.log.append(message)
	def _versionNotApplies(self) :
		if not self.ensuredVersion : return False
		featuredVersion = self._featuredVersion(self.version)
		if featuredVersion == self._featuredVersion(self.ensuredVersion) :
			return False
		return True
	def _namesForType(self, type) :
		return [ processing.get("id")
			for processing in self.processings
			if processing.get('type') == type ]

	def _processingsOfType(self, type) :
		for processing in self.processings :
			if processing.get('type') == type :
				yield processing

	def _processingOfId(self, id) :
		for processing in self.processings :
			if processing.get('id') == id :
				return processing
		return None
	def _availableCommands(self) :
		return [ method for method in dir(self)
			if method[0] != '_' and callable(getattr(self,method)) ]

	def beVerbose(self, shouldBe=True) :
		self.verbose = shouldBe
	def dump(self, file=sys.stdout) :
		self.document.write(file)
	def dumpLog(self, file=sys.stdout) :
		print >> file, "\n".join(self.log)

	def ensureVersion(self, versionString) :
		'''Makes the next commands apply just if the network version
		versionString'. Use 'any' to remove the restriction'''
		if versionString.lower() is "any" : versionString = ''
		self.ensuredVersion = versionString
		
	def upgrade(self, version) :
		if self._versionNotApplies() : return
		self.document.find('/').attrib['clamVersion'] = version
		self.version = version

	def renameClass(self, oldType, newType) :
		if self._versionNotApplies() : return
		for processing in self._processingsOfType(oldType) : 
			processing.set('type', newType)
			self._log("Retyping processing %s: %s -> %s" %(
				processing.get("id"), oldType, newType))

	def renameConnector(self, processingType, connectorKind, oldName, newName) :
		if self._versionNotApplies() : return
		names = self._namesForType(processingType)
		for connection in self.connections[connectorKind] :
			processing, connector = connection.text.split(".")
			if processing not in names: continue
			if connector != oldName: continue
			connection.text = ".".join([processing, newName])
			self._log("Updating %s: %s.%s -> %s.%s" %(
				connectorKind, processing, oldName, processing, newName))

	def renameConnectorRegExp(self, processingType, connectorKind, oldName, newNameExpression) :
		if self._versionNotApplies() : return
		names = self._namesForType(processingType)
		regexpr = re.compile(oldName)
		for connection in self.connections[connectorKind] :
			processing, connector = connection.text.split(".")
			if processing not in names: continue
			match = regexpr.match(connector)
			if not match : continue
			newName= regexpr.sub(newNameExpression, connector)
			connection.text = ".".join([processing, newName])
			self._log("Updating %s: %s.%s -> %s.%s" %(
				connectorKind, processing, connector, processing, newName))
	
	def _removeConnections(self, processingName):
		connections = dict(outport=[],)
		for connection in self.connections['outport']:
			if connection.text.split('.')[0] != processingName:
				connections['outport'].append(connection)
		self.connections['outport'] = connections['outport']
	
	def _addConnections(self, fromProcessing, fromNr):
		out = ElementTree.Element('out')
		out.text = "%s.%s" % (fromProcessing, fromNr)
		self.connections['outport'] .append(out)

	def _findLastTag(self, tag):
		order = [
			'description',
			'processing',
			'port_connection',
			'control_connection',
			]
		children = self.document.getroot().getchildren()
		tagOrder = order.index(tag)
		for i, child in enumerate(children):
			if order.index(child.tag) > tagOrder: return i
		return len(children)

	def removeOutConnections(self, connectionType, processingName):
		self._removeConnections(processingName) # keep internal data structure intact
		network = self.document.getroot()
		connections = network.findall(connectionType)
		for connection in connections:
			m = re.search(processingName, "%s" % connection.find('out').text)
			if m:
				network.remove(connection)
				self._log("Removed %s from %s" %(m.group(0), connectionType))	
			
	def  addConnection(self, connectionType, fromProcessing, fromNr, toProcessing, toNr):
		self._addConnections(fromProcessing, fromNr) # keep internal data structure intact
		
		index = self._findLastTag(connectionType)
				
		network = self.document.getroot()
		connection = ElementTree.Element(connectionType)
		out = ElementTree.SubElement(connection, "out")
		out.text = "%s.%s" % (fromProcessing, fromNr)
		in_ = ElementTree.SubElement(connection, "in")
		in_.text = "%s.%s" % (toProcessing, toNr)
		
		network.insert(index, connection)
		self._log("Added %s.%s -> %s.%s in %s" %(fromProcessing, fromNr, \
			toProcessing, toNr, connectionType))	
	
	def duplicateProcessing(self, processingId, processingIdnew, xOffset, yOffset):
		processing = self._processingOfId(processingId) 	
		newProcessing = processing.makeelement(processing.tag,processing.attrib)
		newProcessing.attrib['id']=processingIdnew
		x, y = eval(newProcessing.attrib['position'])
		newProcessing.attrib['position'] = '%s, %s' % (x+xOffset, y+yOffset)
		
		for element in processing.getchildren():
			newProcessing.append(copy.copy(element))
		
		index = self._findLastTag("processing") 
		network = self.document.getroot()
		network.insert(index, newProcessing)
		
		# keep internal data structure intact
		self.processings.append(newProcessing)
		self._log("Duplicated %s as %s" %(processingId, processingIdnew))
		
	def renameConfig(self, processingType, oldName, newName) :
		"""Change the name of a config parameter"""
		if self._versionNotApplies() : return
		for processing in self._processingsOfType(processingType) : 
			configs = processing.findall(oldName)
			for config in configs :
				config.tag = newName
				self._log("Renaming configuration parameter for processing %s: %s -> %s" %(
				processing.get("id"), oldName, newName))

	def setConfigByType(self, processingType, name, value) :
		"""Change the value for a config parameter"""
		if self._versionNotApplies() : return
		for processing in self._processingsOfType(processingType) : 
			configs = processing.findall(name)
			for config in configs :
				config.text = value
				self._log("Setting configuration parameter %s.%s = %s" %(
					processing.get("id"), name, value))

	def transformConfigByType(self, processingType, name, expression) :
		"""Transform the value for a config parameter"""
		if self._versionNotApplies() : return
		for processing in self._processingsOfType(processingType) : 
			configs = processing.findall(name)
			for config in configs :
				value=config.text
				config.text = eval(expression)
				self._log("Setting configuration parameter %s.%s = %s" %(
					processing.get("id"), name, config.text))

	def setConfig(self, processingId, param, value) :
		"""Change the value for a config parameter"""
		if self._versionNotApplies() : return
		processing = self._processingOfId(processingId)
		if not processing : raise "Processing ID '%s' not found" % processingId
		configs = processing.findall(param)
		for config in configs :
			config.text = value
			self._log("Setting configuration parameter %s.%s = %s" %(
				processing.get("id"), param, value))

	def addConfig(self, processingType, name, default) :
		"""Change the name of a config parameter"""
		if self._versionNotApplies() : return
		for processing in self._processingsOfType(processingType) :
			if processing.findall(name) : continue # Already present, don't add
			parameterElement = ElementTree.Element(name) 
			parameterElement.text = unicode(default)
			processing.append(parameterElement)
			self._log("Adding configuration parameter %s.%s = %s" %(
				processing.get("id"), name, default))

	def removeConfig(self, processingType, name) :
		if self._versionNotApplies() : return
		for processing in self._processingsOfType(processingType) :
			for param in processing.findall(name) :
				if param.tag != name : continue
				processing.remove(param) 
				self._log("Removing configuration parameter %s.%s" %(
					processing.get("id"), name))

	def moveConfig(self, processingType, name, beforeTag=None) :
		raise "Not implemented!!!!"
		if self._versionNotApplies() : return
		for processing in self._processingsOfType(processingType) :
			for param in processing.findall(name) :
				processing.remove(param) 
				self._log("Removing configuration parameter %s.%s" %(
					processing.get("id"), name))

	def noOp(self) :
		if self.verbose : print "Dummy command"
		pass

	def help(self, command) :
		help(getattr(self,command))

	def commands(self) :
		print "Available commands:", ", ".join(self._availableCommands())


def test() :
	testInput = "../../NetworkEditor/example-data/FilePlayer.clamnetwork"
	network = ClamNetwork(file(testInput))
	commandFileContent =  [
		'beVerbose',
		'ensureVersion 1.3',
		'renameClass OutControlSender Foo',
		'renameConnector AudioMixer inport "Input 0" lala',
		'renameConfig AudioMixer NumberOfInPorts NInputs',
		'setConfigByType AudioMixer NInputs 14',
		'upgrade 1.3.2',
		# not executed because we are still ensuring 1.3
		'renameConnector AudioMixer inport NInputs NumberOfInPorts',
		'ensureVersion 1.3.2',
		'setConfig "Gain 0" Max 2',
		'addConfig AudioMixer Ramping 1',
		'addConfig AudioMixer NInputs 96',
		'removeConfig AudioMixer FrameSize',
		'upgrade 1.4',
				
		#'removeOutConnections Vbap3D'
		#'addConnection Vbap3D 01 AudioSink 1'
		#'duplicateProcessing AudioMixer AudioMixerCopy_1 0 100',
	]
	

	for command in commandFileContent :
		network.runCommand(command)
	try:
		network.setConfig('NonExistingId', "Max", "2")
	except: print "Exception caugth"
	network.dump()
	network.commands()
	sys.exit(0)

if __name__ == "__main__" :
	from optparse import OptionParser
	parser = OptionParser(
		usage="usage: %prog [-c COMMAND|-f SCRIPTFILE] network1 network2...",
		version="%prog 0.9"
		)
	parser.add_option("-v", "--verbose", dest='verbose', action='store_true')
	parser.add_option("-q", "--quiet", dest='verbose', action='store_false')
	parser.add_option("-f", "--file", dest='scriptFile',
			help="Specifies a file with commands", metavar="SCRIPTFILE")
	parser.add_option("-c", "--command", dest='commands', action='append',
			help="Specifies a command to be executed on the networks", metavar="COMMAND")
	parser.add_option("-t", "--test", dest='test', action="store_true",
			help="Runs the library test")
	parser.add_option("-x", "--apply", dest='apply', action="store_true",
			help="Actually modify the files (by default they are just dumped to the screen).")
	options, args = parser.parse_args()

	if options.test :
		test()

	commands = []
	if options.commands :
		commands = options.commands
	elif options.scriptFile :
		commands = file(options.scriptFile).readlines()

	if not commands :
		print >> sys.stderr, "No command specified, use either -c or -f options"
	if not args :
		print >> sys.stderr, "No file to be processed"
		

	for filename in args :
		print >> sys.stderr, "Processing", filename
		network = ClamNetwork(file(filename))
		for command in commands :
			if not command.strip() or command.strip()[0]=='#' :
				continue
			network.runCommand(command)

		output = sys.stdout
		if network.modified :
			if options.apply :
				print "Updating", filename
				output = open(filename,"w")
			network.dump(output)
		print >> sys.stderr, "No changes applied."



