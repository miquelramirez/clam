import cStringIO
from Pool import *
from Aggregator import *
from Pool import *
from Schema import *

class MetadataSourceAggregator :

	class Exception :
		def __init__(self, message) :
			self.what = message
		def __str__(self) :
			return self.what

	def __init__(self, sources, attributeMap) :
		self.sources = dict(sources)
		self.attributeMap = attributeMap
		self.sourceKeys = [key for (key, source) in sources ]
		# TODO: Check that the sources in the map are the ones on the sources

	def _AggregatorScriptFor(self, descriptors) :
		script = ""
		sourceIds = []
		for (target, source, sourceAttribute) in self.attributeMap :
			if target not in descriptors: continue
			if source not in sourceIds:
				sourceIds.append(source)
			i = sourceIds.index(source) + 1
			script += "copy " + str(i) + " " + sourceAttribute + " " + target + "\n"
		return script, sourceIds

	def _DisgregatorScripts(self, descriptors) :
		scripts = dict.fromkeys(self.sourceKeys, "")
		sourceIds = []
		for (target, source, sourceAttribute) in self.attributeMap :
			if target not in descriptors: continue
			if source not in sourceIds:
				sourceIds.append(source)
			scripts[source] += "copy 1 " + target + " " + sourceAttribute + "\n"
		return scripts

	def QueryDescriptors(self, id, descriptors) :
		(aggregatorScript, sourceIds) = self._AggregatorScriptFor(descriptors)
		aggregator = Aggregator(cStringIO.StringIO(aggregatorScript))
		result = Pool()
		sourcesPools = []
		for sourceId in sourceIds :
			sourcePool = self.sources[sourceId].QueryDescriptors(id)
			sourcesPools.append(sourcePool)
		aggregator.run(result, sourcesPools)
		return result

	def QuerySchema(self, descriptors) :
		(aggregatorScript, sourceIds) = self._AggregatorScriptFor(descriptors)
		aggregator = Aggregator(cStringIO.StringIO(aggregatorScript))
		result = Schema()
		aggregator.run(result, [
			self.sources[sourceId].QuerySchema() 
				for sourceId in sourceIds
			])
		return result

	def UpdateDescriptors(self, id, pool, descriptors=None) :
		if descriptors == None :
			descriptors = pool.PresentAttributes()
		scripts = self._DisgregatorScripts(descriptors)
		for source, script in scripts.items() :
			if script=="": continue
			disgregator = Aggregator(cStringIO.StringIO(script))
			try :
				result = self.sources[source].QueryDescriptors(id)
			except: 
				result = Pool()
			disgregator.run(result, [pool] )
			self.sources[source].UpdateDescriptors(id,result)

	def CheckMissingDescriptors(self, idlist, descriptorlist) :
		result={}

		for id in idlist:
			result[id]=[]
	
		checkedsources=[]
	
		for (target, source, sourceAttribute) in self.attributeMap :
			#Check the current source to look for missing descriptors
			if target in descriptorlist:
				notfoundlist=self.sources[source].CheckMissingDescriptor(sourceAttribute, idlist)
				if self.sources[source] not in checkedsources:
					checkedsources.append( self.sources[source] )
				for item in notfoundlist:
					result[item].append(target)

		for id in idlist:
			for csource in checkedsources:
				try:
					csource.idsToRecalculate.remove(str(id))
				except ValueError:
					pass

		return result

	def AvailableDescriptors(self, source=None) :
		if source==None:
			return  [ a[0] for a in self.attributeMap ]
		else:
			return  [ a[0] for a in self.attributeMap if a[1]==source ]
