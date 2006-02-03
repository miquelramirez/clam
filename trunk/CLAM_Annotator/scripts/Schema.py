from xml import xpath
from xml.dom.ext.reader import Sax2
import xml.dom.ext 
import sys
from cStringIO import StringIO

class Schema :
	class Exception :
		def __init__(self, message) :
			self.what = message
		def __str__(self) :
			return self.what

	def __init__(self, file=None) :
		if file is None:
			file = StringIO("<DescriptionScheme><Attributes/></DescriptionScheme>")
		self.doc = Sax2.Reader().fromStream(file);

	def Dump(self, file=sys.stdout):
		xml.dom.ext.PrettyPrint(self.doc, file)
		file.flush()

	def SelectAttribute(self, scope, name) :
		path = '//Attribute[@scope="%s" and @name="%s"]'%(scope,name)
		return xpath.Evaluate(path, self.doc.documentElement)

	def InsertAttribute(self, source, scope, name, targetScope, targetName=None) :
		nodesToMove = source.SelectAttribute(scope,name)
		if len(nodesToMove)!=1 :
			raise Schema.Exception("Attribute '"+scope+"::"+name+"' not found")
		nodeToMove  = nodesToMove[0]
		newAttribute = self.doc.importNode(nodeToMove,True)
		newAttribute.setAttribute("scope", targetScope)
		if targetName != None : 
			newAttribute.setAttribute("name", targetName)
		xpath.Evaluate("//Attributes", self.doc)[0].appendChild(newAttribute)

	def RemoveAttribute(self, scope, name) :
		nodesToRemove = self.SelectAttribute(scope,name)
		if len(nodesToRemove)!=1 :
			raise Schema.Exception("Attribute '"+scope+"::"+name+"' not found")
		xpath.Evaluate("//Attributes", self.doc)[0].removeChild(nodesToRemove[0])



