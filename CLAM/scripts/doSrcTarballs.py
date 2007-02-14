#! /usr/bin/python
import os
import sys
sys.path.append("../scons/sconstools/")
import versionInfo 

def run(command) :
	print "\033[32m:: ", command, "\033[0m"
	return os.system(command)
	for line in os.popen(command) :
		print line,
		sys.stdout.flush()
def norun(command) :
	print "\033[31mXX ", command, "\033[0m"
def phase(desc) :
	print "\033[33m== ", desc, "\033[0m"

def svnRevision():
	w, r = os.popen2( "LANG='' svn info http://iua-share.upf.edu/svn/clam/trunk | awk '/^Revision:/ { print $2}'")
	lastRevision = r.readline().strip()
	w.close()
 	r.close()
	return lastRevision

def packageVersionFromSvn( package ) :
	repositoryBase = "http://iua-share.upf.edu/svn/clam/trunk/"
	os.system("rm CHANGES*" )
	os.system("wget "+ repositoryBase + package + "/CHANGES" )
	version, longVersion = versionInfo.takeFromChangeLog( "CHANGES", package )
	return version

versions = {
 "CLAM": "---",
 "NetworkEditor": "---",
 "Annotator" : "---",
 "SMSTools" : "---",
 "Voice2MIDI": "---",
}
for package, _ in versions.iteritems():
	versions[package] = versionInfo.packageVersionFromSvn(package)

print versions
print "svnRevision: ", versionInfo.svnRevision()

for package, version in versions.iteritems() : 
	if package == "CLAM" :
		#sandbox = "CLAM-%s-svn%s" % (version, svnRevision() )
		sandbox = "CLAM-%s" % (version)
		run("svn export http://iua-share.upf.edu/svn/clam/trunk/CLAM %s"% sandbox)
	else:
		dict =  { 
		"package" : package,
		"ver" : version,
		"clamver" : versions["CLAM"]
		}
		#sandbox = "%(package)s-%(ver)s_using_CLAM-%(clamver)s" % dict
		sandbox = "%(package)s-%(ver)s" % dict
		run("svn export http://iua-share.upf.edu/svn/clam/trunk/%s %s" % (package, sandbox))
	tarball = "%s.tar.gz" % sandbox
	run("echo '%s' > %s/SVN_REVISION" % (svnRevision(), sandbox) )
	run("tar czf %s %s " % (tarball, sandbox) )
	run("rm -rf %s/" % sandbox)
	norun("scp %s clamadm@www.iua.upf.edu:download/src/ " % tarball )

phase("regenerating web download dirs")
norun("slogin clamadm@www.iua.upf.edu scripts/regenerateDownloadDirsIndex.py")

print "Remove created tarballs in local? [y/n]"
if raw_input().strip() in ['y', 'Y', 'yes']:
	run("rm *.tar.gz")

 

