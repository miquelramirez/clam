import re
import cStringIO
import sys

class Aggregator :
	class Exception :
		def __init__(self, message) :
			self.what = message
		def __str__(self) :
			return self.what

	class Copier :
		regexp = re.compile("^copy\s*(\d+)\s*(\w*)::(\w*)\s*(\w*)::(\w*)")
		def __init__(self, aggregator, source, scope, attribute, targetScope, targetAttribute) :
			self.aggregator = aggregator
			self.source = int(source)
			self.scope = scope
			self.attribute = attribute
			self.targetScope = targetScope
			self.targetAttribute = targetAttribute

		def apply(self) :
			target = self.aggregator.target
			source = self.aggregator.sources[self.source-1]
			target.InsertAttribute(source,
				self.scope, self.attribute,
				self.targetScope, self.targetAttribute)

		def dump(self, file) :
			print >> file, "copy", self.source, \
				(self.scope + "::" + self.attribute), \
				(self.targetScope + "::" + self.targetAttribute)

	def __init__(self, script) :
		errors = ""
		self.copiers = []
		self.requiredSources = 0
		lineNumber = 0
		for line in script :
			lineNumber=lineNumber+1
			usefulContent = line.split("#")[0].strip()
			if usefulContent=="": continue
			match = Aggregator.Copier.regexp.match(usefulContent)
			if not match :
				errors += "Parser error at line %d\n>%s"%(lineNumber,line)
				continue
			fields = match.groups()
			self.copiers.append(
				Aggregator.Copier(self, int(fields[0]),
					fields[1], fields[2],
					fields[3], fields[4]))
			if self.requiredSources < int(fields[0]) :
				self.requiredSources = int(fields[0])
		if errors != "" :
			raise Aggregator.Exception(errors)

	def run(self, target, sources) :
		if len(sources) < self.requiredSources :
			raise "The aggregation script requires %d sources but only %d provided"% \
				(self.requiredSources,len(sources))
		self.target = target
		self.sources = sources
		for copier in self.copiers :
			copier.apply()

	def dump(self, file) :
		for copier in self.copiers :
			copier.dump(file)


