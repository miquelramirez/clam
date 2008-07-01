#!/usr/bin/python

import sys

schema = sys.argv[1]
pools = sys.argv[2:]

print "Schema:", schema
print "pools:", pools

from xml.etree import ElementTree
schemaDoc = ElementTree.parse(file(schema))

segmentationAttributes = [ (line.attrib['scope'], line.attrib['name'], (line.find('SegmentationPolicy')).text)
	for line in schemaDoc.findall("//Attribute")
	if line.attrib['type'] == 'Segmentation'
]

print segmentationAttributes
schemaDoc.write(sys.stdout,'utf8')

for pool in pools:
	poolDoc = ElementTree.parse(file(pool))
	# locate the segmentation attributes
	for scope, attribute, policy in segmentationAttributes:
		for line in  poolDoc.findall("//ScopePool"):
			if line.attrib['name'] != scope : continue
			for line2 in line.findall("AttributePool"):
				if line2.attrib['name'] != attribute : continue
				#insert the new attrib "Segmentations"
				segmentations=line2.makeelement(policy, {})
				#set maxlength, which is a fake number by now
				segmentations.attrib['maxlength']='100000'
				#mov the content into the deeper level
				segmentations.text = line2.text
				line2.text= None
				segmentations.set("size", line2.attrib.pop("size"))
				line2.append(segmentations)
	poolDoc.write(sys.stdout, 'utf8')

