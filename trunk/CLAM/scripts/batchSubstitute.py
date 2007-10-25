#!/usr/bin/python
import glob, re, sys
"""
Substitution examples:
	(r"'''(([^']|'[^']|''[^'])*)'''", r"<b>\1</b>"),
	(r"''(([^']|'[^'])*)''", r"<em>\1</em>"),
	(r"\[(\S*)\s(.+)\]", r"<a href='\1'>\2</a>"),
	(r"\[(\S*)\]", r"<a href='\1'>\1</a>"),
	(r"`([^`]*)`", r"<img src=http://www.forkosh.dreamhost.com/mimetex.cgi?\1 />"),
	(r"{{{", r"<pre>"),
	(r"}}}", r"</pre>"),
	(r"^@toc\s*$", r"%(toc)s"),
"""

# --------------------------
# User configuration
# --------------------------

testingMode = True # True: do not write any file but print debugging output
                   # False: overwrite files

inlineSubstitutions = [
	(r"GrupDeTests", r"TestFixture"),
	(r"GRUP_DE_TESTS", r"TEST_FIXTURE"),
	(r"CAS_DE_TEST", r"TEST_CASE"),
	(r"ASSERT_IGUALS", r"ASSERT_EQUALS"),
	(r"REGISTRA_TEST", r"REGISTER_FIXTURE"),
	(r"FALLA", r"FAIL"),
	
	
]
filesToSubstitute = [
	"Sandboxes/ES1-*/src/TestsUnitaris/Test*.cxx",
]
# --------------------------
# End of user configuration
# --------------------------


filenames = []
for search in filesToSubstitute: filenames +=  glob.glob(search)

inlines = [ (re.compile(pattern), substitution) for pattern, substitution in inlineSubstitutions ]
def substituteInlines(line, linenumber, filename) :
	original = line
	for compiledPattern, substitution in inlines :
		line = compiledPattern.sub(substitution, line)
	if testingMode and line!=original :
		print "%s line %i:\nold: %snew: %s"%(filename, linenumber, original, line)
	return line


for filename in filenames :
	file = open(filename)
	modified = []
	for i, line in enumerate(file.readlines()) :
		modified += [ substituteInlines(line, i, filename) ]
	file.close()
	if testingMode: continue
	print "writing: "+filename
	file = open(filename,"w")
	file.writelines( modified )
	file.close()
		

