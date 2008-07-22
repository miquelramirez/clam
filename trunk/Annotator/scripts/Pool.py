# Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
#                         UNIVERSITAT POMPEU FABRA
#
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import sys
# Kludge: Ubuntu prefers the standard xml module which doesn't contain xpath
sys.path.append('/usr/lib/python2.5/site-packages/oldxml')
import xml.xpath

from xml.dom.ext.reader import Sax2
import xml.dom.ext 
import cStringIO

class Pool :
	class Exception :
		def __init__(self, message) :
			self.what = message
		def __str__(self) :
			return self.what

	def __init__(self, file=None) :
		if file is None:
			file = cStringIO.StringIO("<DescriptorsPool/>")
		self.doc = Sax2.Reader().fromStream(file);

	def Dump(self, file=sys.stdout):
		xml.dom.ext.PrettyPrint(self.doc, file)
		file.flush()

	def GetScopeSize(self, scope) :
		path = '//ScopePool[@name="%s"]/@size'%(scope)
		nodes = xml.xpath.Evaluate(path, self.doc.documentElement)
		for node in nodes :
			return int(node.nodeValue)
		raise Pool.Exception("Scope '" + scope + "' not found")

	def SelectAttributes(self, scope, name) :
		path = '//ScopePool[@name="%s"]/AttributePool[@name="%s"]'%(scope,name)
		return xml.xpath.Evaluate(path, self.doc.documentElement)

	def SelectScope(self, scope) :
		path = '//ScopePool[@name="%s"]'%(scope)
		return xml.xpath.Evaluate(path, self.doc.documentElement)

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
		nodesToMove = source.SelectAttributes(scope,name)
		if len(nodesToMove)!=1 :
			raise Pool.Exception("Attribute '"+scope+"::"+name+"' not found")
		sourceSize = source.GetScopeSize(scope)
		self.AssureScopeWithPopulation(targetScope, sourceSize)
		self._InsertNode(nodesToMove, targetScope, targetName)

	def _InsertNode(self, nodesToMove, scope, name = None) :
		path = '//ScopePool[@name="%s"]'%(scope)
		scopeNodes = xml.xpath.Evaluate(path, self.doc.documentElement)
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


