import os, re
#reminder if we want to use the date: datetime.date.today()

# private methods:

def _svnRevisionOf( whatToCheck ):
	os.environ['LANG']='C'
	output = os.popen("svn info "+whatToCheck)
	revisionLocator = re.compile(r'^Revision:(?P<revision>.*)')
	for line in output :
		match = revisionLocator.match(line)
		if not match: continue
		return match.group('revision').strip()
	raise "No svn revision found for "+ whatToCheck

def _parseChangesFile( changesFile, product='CLAM' ) :
	versionExtractor = re.compile(r'[0-9?]+-[0-9?]+-[0-9?]+ %s (?P<Major>[0-9]+)\.(?P<Minor>[0-9]+)\.(?P<Patch>[0-9]+)(?P<SVN>.*SVN[^0-9]*(?P<Revision>[0-9]+))?'%product)
	for line in file(changesFile) :
		match = versionExtractor.match(line)
		if match is None: continue
		major, minor, patch, revision = [ match.group(tag) for tag in ('Major', 'Minor', 'Patch', 'Revision') ]
		version = "%s.%s.%s"%(major,minor,patch)
		return version, revision
	assert "not found a valid CHANGES file: "+changesFile

def _svnVersion(version, revision) :
	return "%s~svn%05i"%(version, int(revision))


# public methods:

remoteRepository = 'http://clam-project.org/clam/trunk/'

def remoteSvnRevision() :
	return _svnRevisionOf( remoteRepository )

def versionFromLocalInfo( product='CLAM', changesFile="CHANGES" ):
	"If available take the revision in (local) svn info, else use CHANGES"
	version, revision = _parseChangesFile( changesFile, product ) 
	if not revision : # it's a release. forget about svnVersion
		return version, version 
	try : 
		revision = _svnRevisionOf(".")
	except:	
		pass
	return version, _svnVersion(version, revision)

def versionFromRemoteSvn( product="CLAM" ) :
	os.system("rm CHANGES*" )
	os.system("svn export "+ remoteRepository + product + "/CHANGES" )
	version, revision = _parseChangesFile( "CHANGES", product ) 
	if not revision : return version, version
	revision = _svnRevisionOf( remoteRepository )
	return version, _svnVersion(version, revision)
	
def generateVersionSources(fileBase, namespace, versionString, fullVersionString=None) :
	if not fullVersionString:  fullVersionString = versionString
	header = file(fileBase+".hxx", "w")
	header.write('namespace %s { const char * GetFullVersion(); }\n'%namespace)
	header.write('namespace %s { const char * GetVersion(); }\n'%namespace)
	header.close()
	source = file(fileBase+".cxx", "w")
	source.write('namespace %s { const char * GetFullVersion() {return "%s";} }\n'%(namespace,fullVersionString))
	source.write('namespace %s { const char * GetVersion() {return "%s";} }\n'%(namespace,versionString))
	source.close()

