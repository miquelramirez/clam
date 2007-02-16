import os
repositoryBase = "http://iua-share.upf.edu/svn/clam/trunk/"

def takeFromChangeLog(changelogFile, product='CLAM') :
	import re
	import datetime
	versionExtractor = re.compile(r'[0-9?]+-[0-9?]+-[0-9?]+ %s (?P<Major>[0-9]+)\.(?P<Minor>[0-9]+)\.(?P<Patch>[0-9]+)(?P<SVN>.*SVN.*)?'%product)
	for line in file(changelogFile) :
		match = versionExtractor.match(line)
		if match is None: continue
		major, minor, patch, isSvn = [ match.group(tag) for tag in ('Major', 'Minor', 'Patch', 'SVN') ]
		versionString = "%s.%s.%s"%(major,minor,patch)
		today = datetime.date.today()
		if not isSvn : return versionString, versionString
		return versionString, "%s~svn%05i"%(versionString, int(svnRevision()))

def svnRevision():
	import re
	os.environ['LANG']='C'
	output = os.popen("svn info "+repositoryBase)
	revisionLocator = re.compile(r'^Revision:(?P<revision>.*)')
	for line in output :
		match = revisionLocator.match(line)
		if not match: continue
		return match.group('revision').strip()
	raise "No revision found"

def packageVersionFromSvn( package ) :
	os.system("rm CHANGES*" )
	os.system("svn export "+ repositoryBase + package + "/CHANGES" )
	version, longVersion = takeFromChangeLog( "CHANGES", package )
	return version



def generateVersionSources(fileBase, namespace, versionString) :
	header = file(fileBase+".hxx", "w")
	header.write('namespace %s { const char * GetFullVersion(); }\n'%namespace)
	header.close()
	source = file(fileBase+".cxx", "w")
	source.write('namespace %s { const char * GetFullVersion() {return "%s";} }\n'%(namespace,versionString))
	source.close()


