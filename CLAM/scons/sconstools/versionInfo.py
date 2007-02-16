import os
repositoryBase = "http://iua-share.upf.edu/svn/clam/trunk/"

# used by all SConstruct scripts. For naming the package and propagate into the program
def takeFromChangeLog(changelogFile, product='CLAM') :
	import re
	import datetime
	versionExtractor = re.compile(r'[0-9?]+-[0-9?]+-[0-9?]+ %s (?P<Major>[0-9]+)\.(?P<Minor>[0-9]+)\.(?P<Patch>[0-9]+)(?P<SVN>.*SVN[^0-9]*(?P<Revision>[0-9]+))?'%product)
	for line in file(changelogFile) :
		match = versionExtractor.match(line)
		if match is None: continue
		major, minor, patch, isSvn, revision = [ match.group(tag) for tag in ('Major', 'Minor', 'Patch', 'SVN', 'Revision') ]
		versionString = "%s.%s.%s"%(major,minor,patch)
		today = datetime.date.today()
		if not isSvn : return versionString, versionString # release!
		try :
			# try local svn info. we prefer this before CHANGES
			revision = svnRevision( changelogFile )
		except:
			pass
		return versionString, "%s~svn%05i"%(versionString, int(revision))

def svnRevision( whatToCheck=repositoryBase ):
	import re
	os.environ['LANG']='C'
	output = os.popen("svn info "+whatToCheck)
	revisionLocator = re.compile(r'^Revision:(?P<revision>.*)')
	for line in output :
		match = revisionLocator.match(line)
		if not match: continue
		return match.group('revision').strip()
	raise "No svn revision found for "+ whatToCheck

# Used by doDebian and doSourcePackages ... (non svn dir)
def packageVersionFromSvn( package ) :
	os.system("rm CHANGES*" )
	os.system("svn export "+ repositoryBase + package + "/CHANGES" )
	version, longVersion = takeFromChangeLog( "CHANGES", package )
	if version==longVersion: return version
	# CHANGES file revision may not be the repository last revision
	return "%s~svn%05i"%(version, int(svnRevision()))



def generateVersionSources(fileBase, namespace, versionString) :
	header = file(fileBase+".hxx", "w")
	header.write('namespace %s { const char * GetFullVersion(); }\n'%namespace)
	header.close()
	source = file(fileBase+".cxx", "w")
	source.write('namespace %s { const char * GetFullVersion() {return "%s";} }\n'%(namespace,versionString))
	source.close()


