#!/usr/bin/python

import sys

schema = sys.argv[1]
data = sys.argv[2:]

print "Schema:", schema
print "Data:", data

from xml.etree import ElementTree
schemaDoc = ElementTree.parse(file(schema))
for line in schemaDoc.findall("//Attribute"):
	if line.attrib['type'] != 'Segmentation' : continue
	print line.attrib['scope'], line.attrib['name']
	print dir(line)





