#! /usr/bin/python
# -*- coding: iso-8859-15 -*-
import sys, re

if len(sys.argv) != 2 :
	print 'filename needed'
	sys.exit(1)
	
print 'processing file:', sys.argv[1]

def contains( line, pattern ):
	return re.search( pattern, line )
	

substitutions = [
	('ASSERT_IGUALS', 'CPPUNIT_ASSERT_EQUAL'),
	('ASSERT_MISSATGE', 'ASSERT_MESSAGE'),
	('#include\s*"MiniCppUnit.hxx"', '#include <cppunit/extensions/HelperMacros.h>'),
	('public GrupDeTests<\s*\S+\s*>','public CppUnit::TestFixture' ),
	('\tCAS_DE_TEST', 'CPPUNIT_TEST' ),
	('GRUP_DE_TESTS','CPPUNIT_TEST_SUITE'),
	('FALLA', 'CPPUNIT_FAIL' )
	]

parsingState = 'DEFAULT'


def checkAsserts(aLine):
	assertPattern = 'ASSERT\((.*)' 
	matchAssert = re.search(assertPattern, aLine)	
	if matchAssert:
		cppunitAssertPattern = 'CPPUNIT_ASSERT('+matchAssert.group(1)
		aLine = re.sub( assertPattern, cppunitAssertPattern, aLine )
	return aLine
	
def processLine(aLine) :
	global parsingState

	if parsingState == 'DEFAULT' :
		if contains(aLine, 'class Test\S+') :
			matchGroup = re.search('class(.+):', aLine)
			classname = matchGroup.group(1)
			register = 'class%s;\nCPPUNIT_TEST_SUITE_REGISTRATION(%s);' % (classname, classname)
			aLine = 'namespace CLAMTest {\n\n%s\n\n%s' % (register, aLine)
		if contains(aLine, 'GRUP_DE_TESTS') :
			matchGroup = re.search('GRUP_DE_TESTS\(\s*(\S+)\s*\)', aLine )
			aLine = re.sub( 'GRUP_DE_TESTS\(\s*(\S+)\s*\)\n', 'CPPUNIT_TEST_SUITE( ', aLine)
			aLine += matchGroup.group(1) + ' );\n'
			parsingState = 'TESTS_DECL'
		if contains(aLine, 'int main'):
			parsingState = 'MAIN'
			aLine = ''
			
	if parsingState == 'TESTS_DECL' :
		if contains(aLine, '{') :
			aLine = '\n'
		if contains(aLine, '}') :
			aLine = '\n\tCPPUNIT_TEST_SUITE_END();\n'
			parsingState = 'DEFAULT'

	if parsingState == 'MAIN' :		
		if contains(aLine,'}') :
			parsingState = 'DEFAULT'
		aLine = ''
	
	changed = False
	for pattern, subst in substitutions :
		match = re.search( pattern, aLine)
		changed = changed or match
		if match:
			aLine = re.sub( pattern, subst, aLine )

	aLine = checkAsserts( aLine )
	
	return changed, aLine

newfile = file('__'+sys.argv[1], 'w')
originalFile = file(sys.argv[1]).readlines()
originalFile.append('\n} // namespace CLAMTest \n')

for line in originalFile :
	changed, processedLine = processLine(line)
	if changed : 
		print '+++',
	else :
		print '···',
	print processedLine,
	newfile.write(processedLine)
	

