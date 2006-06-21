def takeFromChangeLog(changelogFile) :
	import re
	import datetime
	versionExtractor = re.compile(r'[0-9?]+-[0-9?]+-[0-9?]+ CLAM (?P<Major>[0-9]+)\.(?P<Minor>[0-9]+)\.(?P<Patch>[0-9]+)(?P<CVS>.*CVS.*)?')
	for line in file(changelogFile) :
		match = versionExtractor.match(line)
		if match is None: continue
		major, minor, patch, isCvs = [ match.group(tag) for tag in ('Major', 'Minor', 'Patch', 'CVS') ]
		versionString = "%s.%s.%s"%(major,minor,patch)
		today = datetime.date.today()
		if not isCvs : return versionString, versionString
		return versionString, "%s-CVS-%04i%02i%02i"%(versionString, today.year, today.month, today.day)

def generateVersionSources(fileBase, namespace, versionString) :
	header = file(fileBase+".hxx", "w")
	header.write('namespace %s { const char * GetFullVersion(); }\n'%namespace)
	header.close()
	source = file(fileBase+".cxx", "w")
	source.write('namespace %s { const char * GetFullVersion() {return "%s";} }\n'%(namespace,versionString))
	source.close()


