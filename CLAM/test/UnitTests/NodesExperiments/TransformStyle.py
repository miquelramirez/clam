#! /usr/bin/python
# -*- coding: iso-8859-15 -*-
import sys, re, os

if len(sys.argv) != 2 :
	print 'filename needed'
	sys.exit(1)
	
print 'processing file:', sys.argv[1]

def contains( line, pattern ):
	return re.search( pattern, line )

def changeMethods(aLine) :
	changed = False
	method = '(\s+)([a-z])([a-zA-Z0-9]+)\((.*)\)'
	matchMethod = re.search( method, aLine )
	if matchMethod :
		changed = True
		newMethod = '%s%s%s(%s)' % (matchMethod.group(1), matchMethod.group(2).upper(),matchMethod.group(3), matchMethod.group(4) )
		aLine = re.sub( method, newMethod, aLine )

	call= '(\.|->)([a-z])([a-zA-Z0-9]+)\((.*)\)'
	matchCall = re.search( call, aLine )
	if matchCall:
		change = True
		print 'call'
		newCall = '%s%s%s(%s)' % (matchCall.group(1), matchCall.group(2).upper(), matchCall.group(3), matchCall.group(4))
		aLine = re.sub( call, newCall, aLine )
	
	return changed, aLine

def processLine(aLine) :
	changed = False
	changed, aLine = changeMethods(aLine)
	attribute = '([^a-zA-Z0-9])_([a-z])\S*'
	matchAttribute = re.search( attribute, aLine )
	
	include = contains( aLine, '_hxx' )
	if matchAttribute and not include:
		changed = True
		newAttribute = '%sm%s%s' % ( matchAttribute.group(1), matchAttribute.group(0)[2].upper(), matchAttribute.group(0)[3:] )
		aLine = re.sub( attribute, newAttribute, aLine )
	
		
	return changed, aLine



newfile = file('__'+sys.argv[1], 'w')
originalFile = file(sys.argv[1]).readlines()

for line in originalFile :
	changed, processedLine = processLine(line)
	if changed : 
		print '+++',
	else :
		print '···',
	print processedLine,
	newfile.write(processedLine)
newfile.close()
os.system('xxdiff %s __%s' % (sys.argv[1], sys.argv[1]))
	
