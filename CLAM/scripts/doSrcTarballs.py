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
 "chordata": "---",
 "pyclam": "---",
}
for package, _ in versions.iteritems():
	versions[package] = versionInfo.versionFromRemoteSvn(package)[1]

print versions
print "remoteSvnRevision: ", versionInfo.remoteSvnRevision()

for package, version in versions.iteritems() : 
	if package == "CLAM" :
		sandbox = "CLAM-%s" % (version)
		run("svn export http://clam-project.org/clam/trunk/CLAM %s"% sandbox)
	else:
		dict =  { 
		"package" : package,
		"ver" : version,
		"clamver" : versions["CLAM"]
		}
		#sandbox = "%(package)s-%(ver)s_using_CLAM-%(clamver)s" % dict
		sandbox = "%(package)s-%(ver)s" % dict
		run("svn export http://clam-project.org/clam/trunk/%s %s" % (package, sandbox))
	tarball = "%s.tar.gz" % sandbox
	run("rm -rf %s/debian" % (sandbox) )
	run("echo '%s' > %s/SVN_REVISION" % (versionInfo.remoteSvnRevision(), sandbox) )
	run("tar czf %s %s " % (tarball, sandbox) )
	run("rm -rf %s/" % sandbox)

print "Upload all tarballs found in local (*.tar.gz) ? [y/n]"
if raw_input().strip() in ['y', 'Y', 'yes']:

	if "~svn" in versions["CLAM"]: destDir = "src/svnsnapshots/"
	else : destDir = "src/"
	run("scp *.tar.gz clamadm@clam-project.org:clam-project.org/download/"+destDir )
	phase("regenerating web download dirs")
	run("slogin clamadm@clam-project.org clam-project.org/scripts/regenerateDownloadDirsIndex.py")

print "Remove created tarballs in local (rm *.tar.gz) ? [y/n]"
if raw_input().strip() in ['y', 'Y', 'yes']:
	run("rm *.tar.gz")

 

