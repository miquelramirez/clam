from xml import xpath
from xml.dom.ext.reader import Sax2
import xml.dom.ext 
import sys
import cStringIO

class Pool :
	class Exception :
		def __init__(self, message) :
			self.what = message
		def __str__(self) :
			return self.what

	def __init__(self, file=None, domnodelist=None) :
		# EP Constructor a partir d'un llistat de nodes, caldra crear manualment l'arrel
		#  (per passar la llista, millor xpath per seleccionar attributs i tal)
		if file is None:
			file = cStringIO.StringIO("<DescriptorsPool/>")

		self.doc = Sax2.Reader().fromStream(file)

		if domnodelist is not None:
			root = self.doc.getElementsByTagName("DescriptorsPool")[0]
			for node in domnodelist:
				cloned = self.doc.importNode(node, True)
				root.appendChild(cloned)

	def Dump(self, file=sys.stdout):
		xml.dom.ext.PrettyPrint(self.doc, file)
		file.flush()

	def GetScopeSize(self, scope) :
		path = '//ScopePool[@name="%s"]/@size'%(scope)
		nodes = xpath.Evaluate(path, self.doc.documentElement)
		for node in nodes :
			return int(node.nodeValue)
		raise Pool.Exception("Scope '" + scope + "' not found")

	def SelectAttributes(self, scope, name) :
		path = '//ScopePool[@name="%s"]/AttributePool[@name="%s"]'%(scope,name)
		return xpath.Evaluate(path, self.doc.documentElement)

	def SelectScope(self, scope) :
		path = '//ScopePool[@name="%s"]'%(scope)
		return xpath.Evaluate(path, self.doc.documentElement)

	def RemoveAttribute(self, scope, name) :
		location = self.SelectScope(scope)
		if len(location)!=1:
			raise Pool.Exception("Scope '"+scope+"' not found while removing '"+ 
				scope+"::"+name+"'")
		toRemove = self.SelectAttributes(scope, name)
		if len(toRemove)!=1:
			raise Pool.Exception("Attribute '"+name+"' not found while removing '"+ 
				scope+"::"+name+"'")
		location[0].removeChild(toRemove[0])
		

	def InsertAttribute(self, source, scope, name, targetScope, targetName=None) :
		if targetName == None : targetName = name
		nodesToMove = source.SelectAttributes(scope,name)
		if len(nodesToMove)<1 :
			raise Pool.Exception("Attribute '"+scope+"::"+name+"' not found")
		sourceSize = source.GetScopeSize(scope)
		self.AssureScopeWithPopulation(targetScope, sourceSize)
		try :
			self.RemoveAttribute(targetScope, targetName)
		except : pass # if it does not exist that's ok
		self._InsertNode(nodesToMove, targetScope, targetName)

	def _InsertNode(self, nodesToMove, scope, name = None) :
		path = '//ScopePool[@name="%s"]'%(scope)
		scopeNodes = xpath.Evaluate(path, self.doc.documentElement)
		for scopeNode in scopeNodes :
			for node in nodesToMove :
				cloned = self.doc.importNode(node, True)
				if name != None :
					cloned.setAttribute("name", name)
				scopeNode.appendChild(cloned)
				return
		assert None, "Target scope not found"

	def PopulateScope(self, scope, population) :
		root = self.doc.documentElement
		newScope = self.doc.createElement("ScopePool")
		newScope.setAttribute("name",scope)
		newScope.setAttribute("size",str(population))
		self.doc.documentElement.appendChild(newScope)

	def AssureScopeWithPopulation(self, scope, population) :
		try :
			actualSize = self.GetScopeSize(scope)
		except Pool.Exception :
			self.PopulateScope(scope, population)
			return
		if actualSize==population : return
		raise Pool.Exception(
			"Requested size for scope '"+ scope +
			"' was " + str(population) +
			" but it is actually " + str(actualSize))
	def PresentAttributes(self) :
		ids=[]
		path = '//ScopePool/@name'
		for scopeNode in \
				xpath.Evaluate(path, self.doc.documentElement):
			path = '//ScopePool[@name="%s"]/AttributePool/@name'%scopeNode.value
			for attributeNode in \
				xpath.Evaluate(path, self.doc.documentElement):
				ids.append(str(scopeNode.value+"::"+attributeNode.value))
		return ids


	

