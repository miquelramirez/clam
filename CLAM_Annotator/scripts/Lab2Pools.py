#!/usr/bin/python

import sys
import re

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


labfile=file(labfilename)

print '<?xml version="1.0" encoding="UTF-8" standalone="no" ?>'
print '<DescriptorsPool>'
print '<ScopePool name="Song" size="1">'
tokens = list()
labelFilter = re.compile(filter)
for line in labfile:
	segment = line.split(" ",3)
	if len(segment)!=3:
		print >> sys.stderr, "Found a line with %s tokens, 3 expected"%len(segment)
		continue
	if not labelFilter.match(segment[2]): continue
	tokens.append(segment)

sampleRate=44100

print '<AttributePool name="%s" size="%s">'%(attributeName, len(tokens)) ,
for segment in tokens :
	print float(segment[0])*sampleRate, float(segment[1])*sampleRate,
print '</AttributePool>'
print '</ScopePool>'

if childScope!=None:
	print '<ScopePool name="%s size="%s">\n' % (childScope, len(tokens))
	print '<AttributePool name="%s">' % (labelAttribute)

	for segment in tokens :
		print '\t<Enumerated>%s</Enumerated>' % ( segment[2].rstrip().lstrip() ) 
	print '</AttributePool>'
	print '</ScopePool>'

print '</DescriptorsPool>'


