#!/usr/bin/python

import sys
import re

def Lab2Plot(labfile, output, attributeName, filter, childScope, labelAttribute):
	print >> output, '<?xml version="1.0" encoding="UTF-8" standalone="no" ?>'
	print >> output, '<DescriptorsPool>'
	print >> output, '\t<ScopePool name="Song" size="1">'
	tokens = list()
	labelFilter = re.compile(filter)
	for line in labfile:
		segment = line.split(" ",3)
		if len(segment)!=3:
			print >> sys.stderr, "Found a line with %s tokens, 3 expected"%len(segment)
			continue
		segment[2]=segment[2].rstrip().lstrip()
		if not labelFilter.match(segment[2]): continue
		tokens.append(segment)

	sampleRate=44100

	print >> output, '\t\t<AttributePool name="%s" size="%s">'%(attributeName, len(tokens)) ,
	for segment in tokens :
		print >> output, float(segment[0])*sampleRate, float(segment[1])*sampleRate,
	print >> output, '</AttributePool>'
	print >> output, '\t</ScopePool>'

	if childScope!=None:
		print >> output, '\t<ScopePool name="%s" size="%s">' % (childScope, len(tokens))
		print >> output, '\t\t<AttributePool name="%s">' % (labelAttribute)

		for segment in tokens :
			print >> output, '\t\t\t<Enumerated>%s</Enumerated>' % ( segment[2] ) 
		print >> output, '\t\t</AttributePool>'
		print >> output, '\t</ScopePool>'
	print >> output, '</DescriptorsPool>'
	return dict.fromkeys([ segment[2] for segment in tokens ]).keys() # unique labels


if __name__ == "__main__":
	# Default parameters
	filter = '.*'
	childScope = None
	labelAttribute = None

	if len(sys.argv) == 3:
		(labfilename,attributeName) = sys.argv[1:3]
	elif len(sys.argv) == 4:
		(labfilename,attributeName,filter) = sys.argv[1:4]
	elif len(sys.argv) == 5:
		(labfilename,attributeName,childScope,labelAttribute) = sys.argv[1:5]
	elif len(sys.argv) == 6:
		(labfilename,attributeName,filter,childScope,labelAttribute) = sys.argv[1:6]
	else:
		print >> sys.stderr, "Lab2Pools v1.0: Wavesurfer Lab file to CLAM Annotator pools converter"
		print >> sys.stderr
		print >> sys.stderr, "Usage: " + sys.argv[0] + " <labFile> <attributeName> [<labelFilter>] [<childScope> <labelAttribute>]"
		print >> sys.stderr
		print >> sys.stderr, " <labFile>        An annotation file from wave surfer"
		print >> sys.stderr, " <attributeName>  Attribute name for the segmentation"
		print >> sys.stderr, " <labelFilter>    A regular expresion to filter segments by label (default: all segments)"
		print >> sys.stderr, " <childScope>     If defined the child scope that should contain the label attribute (by default it is discarded)"
		print >> sys.stderr, " <labelAttribute> The attribute that holds the label"
		sys.exit()

	print Lab2Plot( 
			file(labfilename), 
			file(labfilename+".pool","w"), 
			attributeName, 
			filter, 
			childScope, labelAttribute)




