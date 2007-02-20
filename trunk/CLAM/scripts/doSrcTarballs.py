#! /usr/bin/python
import os
import sys
sys.path.append("../scons/sconstools/")
import versionInfo 

def run(command) :
	print "\033[32m:: ", command, "\033[0m"
	return os.system(command)
def norun(command) :
	print "\033[31mXX ", command, "\033[0m"
def phase(desc) :
	print "\033[33m== ", desc, "\033[0m"


versions = {
 "CLAM": "---",
 "NetworkEditor": "---",
 "Annotator" : "---",
 "SMSTools" : "---",
 "Voice2MIDI": "---",
}
for package, _ in versions.iteritems():
	versions[package] = versionInfo.versionFromRemoteSvn(package)[0]

print versions
print "remoteSvnRevision: ", versionInfo.remoteSvnRevision()

for package, version in versions.iteritems() : 
	if package == "CLAM" :
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
	run("echo '%s' > %s/SVN_REVISION" % (versionInfo.remoteSvnRevision(), sandbox) )
	run("tar czf %s %s " % (tarball, sandbox) )
	run("rm -rf %s/" % sandbox)
	run("scp %s clamadm@www.iua.upf.edu:download/src/ " % tarball )

phase("regenerating web download dirs")
run("slogin clamadm@www.iua.upf.edu scripts/regenerateDownloadDirsIndex.py")

print "Remove created tarballs in local (rm *.tar.gz) ? [y/n]"
if raw_input().strip() in ['y', 'Y', 'yes']:
	run("rm *.tar.gz")

 

