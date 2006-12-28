import os
repositoryBase = "http://iua-share.upf.edu/svn/clam/trunk/"

def takeFromChangeLog(changelogFile, product='CLAM') :
	import re
	import datetime
	versionExtractor = re.compile(r'[0-9?]+-[0-9?]+-[0-9?]+ %s (?P<Major>[0-9]+)\.(?P<Minor>[0-9]+)\.(?P<Patch>[0-9]+)(?P<CVS>.*CVS.*)?'%product)
	for line in file(changelogFile) :
		match = versionExtractor.match(line)
		if match is None: continue
		major, minor, patch, isCvs = [ match.group(tag) for tag in ('Major', 'Minor', 'Patch', 'CVS') ]
		versionString = "%s.%s.%s"%(major,minor,patch)
		today = datetime.date.today()
		if not isCvs : return versionString, versionString
		return versionString, "%s-CVS-%04i%02i%02i"%(versionString, today.year, today.month, today.day)

def svnRevision():
	w, r = os.popen2( "LANG='' svn info "+repositoryBase+" | awk '/^Revision:/ { print $2}'")
	lastRevision = r.readline().strip()
	w.close()
 	r.close()
	return lastRevision

def packageVersionFromSvn( package ) :
	os.system("rm CHANGES*" )
	os.system("wget "+ repositoryBase + package + "/CHANGES" )
	version, longVersion = takeFromChangeLog( "CHANGES", package )
	return version



def generateVersionSources(fileBase, namespace, versionString) :
	header = file(fileBase+".hxx", "w")
	header.write('namespace %s { const char * GetFullVersion(); }\n'%namespace)
	header.close()
	source = file(fileBase+".cxx", "w")
	source.write('namespace %s { const char * GetFullVersion() {return "%s";} }\n'%(namespace,versionString))
	source.close()


