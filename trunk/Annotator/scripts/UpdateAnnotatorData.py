#!/usr/bin/python

import sys

schema = sys.argv[1]
pools = sys.argv[2:]

print "Schema:", schema
print "pools:", pools

from xml.etree import ElementTree
schemaDoc = ElementTree.parse(file(schema))

# classical way
segmentationAttributes = []
for line in schemaDoc.findall("//Attribute"):
	if line.attrib['type'] != 'Segmentation' : continue
	attribute = (line.attrib['scope'], line.attrib['name'])
	segmentationAttributes.append(attribute)

print segmentationAttributes

# comprehended way
segmentationAttributes = [ (line.attrib['scope'], line.attrib['name']) 
	for line in schemaDoc.findall("//Attribute")
	if line.attrib['type'] == 'Segmentation'
]

print segmentationAttributes
print dir(schemaDoc)
help(schemaDoc.write)
schemaDoc.write(sys.stdout,'utf8')

for pool in pools:
	print pool


