#!/usr/bin/python
import glob, re, sys
"""
Substitution examples:
	(r"'''(([^']|'[^']|''[^'])*)'''", r"<b>\1</b>"),
	(r"''(([^']|'[^'])*)''", r"<em>\1</em>"),
	(r"\[(\S*)\s(.+)\]", r"<a href='\1'>\2</a>"),
	(r"\[(\S*)\]", r"<a href='\1'>\1</a>"),
	(r"@cite:([-_a-zA-Z0-9]*)", r"<a href='dataflow.bib.html#\1'>[\1]</a>"), # TODO: hover box with bib info
	(r"`([^`]*)`", r"<img src=http://www.forkosh.dreamhost.com/mimetex.cgi?\1 />"),
	(r"{{{", r"<pre>"),
	(r"}}}", r"</pre>"),
	(r"^@toc\s*$", r"%(toc)s"),
"""

inlineSubstitutions = [
	(r"'referencia': 100", r"'referencia': 10"),
]
filenames = glob.glob("*.testfarmlog")


inlines = [ (re.compile(pattern), substitution) for pattern, substitution in inlineSubstitutions ]
def substituteInlines(line) :
	for compiledPattern, substitution in inlines :
		line = compiledPattern.sub(substitution, line)
	return line


for filename in filenames :
	print "processing",filename
	file = open(filename)
	modified = []
	for line in file.readlines() :
		modified += [ substituteInlines(line) ]
	file.close()
	file = open(filename,"w")
	file.writelines( modified )
	file.close()
		

