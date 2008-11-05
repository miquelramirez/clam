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
			outcontrol = self.document.findall("control_connection/out"),
			inport = self.document.findall("port_connection/in"),
			outport = self.document.findall("port_connection/out"),
			)
		self.version = self.document.find("/").attrib.get("version","1.3")
		self.log = []
		self.verbose = False
		self.ensuredVersion = ""

	def runCommand(self, command) :
		import shlex
		tokens = shlex.split(command)
		try: method=getattr(self, tokens[0])
		except AttributeError:
			raise NotACommand(tokens[0], self._availableCommands())
		method(*tokens[1:])

	def _log(self, message) :
		if self.verbose : print message
		self.log.append(message)
	def _versionNotApplies(self) :
		return self.ensuredVersion and self.version != self.ensuredVersion
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
		self.document.find('/').attrib['version'] = version
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
		for connection in self.connections["inport"] :
			processing, connector = connection.text.split(".")
			if processing not in names: continue
			if connector != oldName: continue
			connection.text = ".".join([processing, newName])
			self._log("Updating %s: %s.%s -> %s.%s" %(
				connectorKind, processing, oldName, processing, newName))

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

	def setConfig(self, processingId, param, value) :
		"""Change the value for a config parameter"""
		if self._versionNotApplies() : return
		processing = self._processingOfId(processingId)
		if not processing : raise "Processing ID not found"
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
		print "Available commands:", ", ".join(_availableCommands)


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
	]
	for command in commandFileContent :
		network.runCommand(command)
	try:
		network.setConfig('NonExistingId', "Max", "2")
	except: print "Exception caugth"
#	network.dump()
	network.commands()
	sys.exit(0)

if __name__ == "__main__" :
	from optparse import OptionParser
	parser = OptionParser(
		usage="usage: %prog [-c COMMAND|-f SCRIPTFILE] network1 network2...")
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
		print "Processing", filename
		network = ClamNetwork(file(filename))
		for command in commands :
			network.runCommand(command)

		output = sys.stdout
		if options.apply :
			print "Updating", filename
			output = open(filename,"w")
		network.dump(output)



