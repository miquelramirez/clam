#! /usr/bin/python
import os
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

versions = {
 "CLAM": "0.96.1",
 "NetworkEditor": "0.4.1",
 "Annotator" : "0.3.2",
 "SMSTools" : "0.4.3",
 "Voice2MIDI": "0.3.1",
}
svnRevision = "9546"



for app, version in versions.iteritems() : 
	if app == "CLAM" :
		#sandbox = "CLAM-%s-svn%s" % (version, svnRevision)
		sandbox = "CLAM-%s" % (version)
		run("svn export http://iua-share.upf.edu/svn/clam/trunk/CLAM %s"% sandbox)
	else:
		dict =  { 
		"app" : app,
		"ver" : version,
		"clamver" : versions["CLAM"]
		}
		#sandbox = "%(app)s-%(ver)s_using_CLAM-%(clamver)s" % dict
		sandbox = "%(app)s-%(ver)s" % dict
		run("svn export http://iua-share.upf.edu/svn/clam/trunk/%s %s" % (app, sandbox))
	tarball = "%s.tar.gz" % sandbox
	run("echo '%s' > %s/SVN_REVISION" % (svnRevision, sandbox) )
	run("tar czf %s %s " % (tarball, sandbox) )
	run("rm -rf %s/" % sandbox)
	run("scp %s clamadm@www.iua.upf.edu:download/src/ " % tarball )

phase("regenerating web download dirs")
run("slogin clamadm@www.iua.upf.edu scripts/regenerateDownloadDirsIndex.py")

print "Remove created tarballs in local? [y/n]"
if raw_input().strip() in ['y', 'Y', 'yes']:
	run("rm *.tar.gz")

 

