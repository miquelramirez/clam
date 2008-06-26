#!/usr/bin/python

import sys

schema = sys.argv[1]
data = sys.argv[2:]

print "Schema:", schema
print "Data:", data

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



