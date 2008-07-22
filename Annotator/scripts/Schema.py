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
		return xml.xpath.Evaluate(path, self.doc.documentElement)

	def InsertAttribute(self, source, scope, name, targetScope, targetName=None) :
		nodesToMove = source.SelectAttribute(scope,name)
		if len(nodesToMove)!=1 :
			raise Schema.Exception("Attribute '"+scope+"::"+name+"' not found")
		nodeToMove  = nodesToMove[0]
		newAttribute = self.doc.importNode(nodeToMove,True)
		newAttribute.setAttribute("scope", targetScope)
		if targetName != None : 
			newAttribute.setAttribute("name", targetName)
		xml.xpath.Evaluate("//Attributes", self.doc)[0].appendChild(newAttribute)

	def RemoveAttribute(self, scope, name) :
		nodesToRemove = self.SelectAttribute(scope,name)
		if len(nodesToRemove)!=1 :
			raise Schema.Exception("Attribute '"+scope+"::"+name+"' not found")
		xml.xpath.Evaluate("//Attributes", self.doc)[0].removeChild(nodesToRemove[0])



