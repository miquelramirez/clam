#!/usr/bin/python

import os.path
import glob
import sys
sconstoolspath = os.path.split(sys.argv[0])[0] + "/../scons/sconstools"
sys.path.append( sconstoolspath )
from versionInfo import versionFromRemoteSvn

proxyoption = "--http-proxy 'http://proxy.upf.edu:8080/'"
proxyoption = ""
distributions = [
#	('debian', 'lenny',   "http://ftp.de.debian.org/debian/", ['main']),
#	('debian', 'sid',    "http://ftp.de.debian.org/debian/", ['main']),
#	('ubuntu', 'gutsy',   "http://es.archive.ubuntu.com/ubuntu/", ['main','universe']),
#	('ubuntu', 'hardy', "http://es.archive.ubuntu.com/ubuntu/", ['main','universe']),
#	('ubuntu', 'intrepid', "http://es.archive.ubuntu.com/ubuntu/", ['main','universe']),
#	('ubuntu', 'jaunty', "http://es.archive.ubuntu.com/ubuntu/", ['main','universe']),
	('ubuntu', 'karmic', "http://es.archive.ubuntu.com/ubuntu/", ['main','universe']),
]
repositoryBase = "http://clam-project.org/clam/trunk/"
repositories = [
	( 'CLAM',          'clam',               versionFromRemoteSvn('CLAM')[1] ),
#	( 'CLAM/plugins',  'clam-plugins',       versionFromRemoteSvn('CLAM')[1] ),
#	( 'NetworkEditor', 'clam-networkeditor', versionFromRemoteSvn('NetworkEditor')[1] ),
#	( 'SMSTools',      'clam-smstools',      versionFromRemoteSvn('SMSTools')[1] ),
#	( 'Annotator',     'clam-annotator',     versionFromRemoteSvn('Annotator')[1] ),
#	( 'chordata',      'clam-chordata',      versionFromRemoteSvn('chordata')[1] ),
]

hooks = {
'D70publishResults' : '''
#!/bin/bash
/usr/bin/apt-get update
''',

'C50checkAfterFailedBuild': '''
#!/bin/bash
# do here whatever you want to see after a failed build
# clam build directory is /tmp/buildd/%(srcpackage)s/
#cat /tmp/buildd/%(srcpackage)s/scons/libs/config.log
'''%{'srcpackage':repositories[0][1]+"-" + repositories[0][2] },
}

aptconfiguration = "APT::Get::AllowUnauthenticated 1;"
failedSteps = []

def run(command) :
	print "\033[32m:: ", command, " \033[0m"
	sys.stdout.flush()
	retcode = os.system(command)
	if retcode != 0 : failedSteps.append(command)
	return retcode
	for line in os.popen(command) :
		print line,
		sys.stdout.flush()
def norun(command) :
	print "\033[31mXX ", command, "\033[0m"
def phase(desc) :
	print "\033[33m== ", desc, "\033[0m"

phase( "Setting up the environment" )
run ("echo 'Remember: run this as root, and configure proxy settings (both in the script and ~/.subversion/servers)'")
run ("mkdir -p hooks")
run ("mkdir -p aptcache")
run ("mkdir -p apt.config/apt.conf.d")

aptconf = file("apt.config/apt.conf.d/allow-unauthenticated",'w')
aptconf.write(aptconfiguration)
aptconf.close()

for (name, content) in hooks.items() :
	phase( "Generating %s"%name )
	hookfile = file("hooks/"+name,'w')
	hookfile.write(content)
	hookfile.close()

run ("chmod a+x hooks/*")

phase( "Obtaining latest sources" )

print repositories
for package, srcpackage, version in repositories :
	module = repositoryBase + package
	modulePackaging = module + '/debian'
	srcdir = srcpackage + "-" + version
	tarball = srcpackage + "_" + version + ".orig.tar.gz"
	run( "svn export --force %s %s"%(module, srcdir) )
	run( "rm -rf  %s/debian"%(srcdir) )
	run( "tar cvfz %s %s"%(tarball, srcdir) )
	run( "svn export --force %s %s/debian"%(modulePackaging, srcdir) )
	run( "cd %s; DEBFULLNAME='CLAM Team' DEBEMAIL='developers@clam-project.org' dch -d 'Autobuilt package' --distribution='unstable' --force-distribution"%(srcdir) )
	run( "dpkg-source -b %s"%(srcdir))


for (maindistro, distribution, mirror, components) in distributions :
	phase( "Preparing chroot for '%s'"%distribution )
	resultdir = '/var/cache/pbuilder/result'
	resultdir = os.path.abspath('./result-%(maindistro)s-%(distro)s' % {
		'maindistro' : maindistro,
		'distro': distribution,
	})
	backportMirror = "'|deb http://es.archive.ubuntu.com/ubuntu/ %(distro)s-backports main restricted universe multiverse' "%{'distro':distribution} if maindistro == 'ubuntu' else ""
	# Assure that the first time a Package file exists
	run ("mkdir -p %(resultdir)s"%{'resultdir' : resultdir } )
	run ("touch %(resultdir)s/Packages"%{'resultdir': resultdir} )

	command = "create"
	if os.access("%s.tgz"%distribution, os.F_OK) :
		command = "update"

	run( ("pbuilder %(command)s " +
		" --othermirror 'deb file:%(resultdir)s ./'" +
		backportMirror +
		" --components '%(components)s' " +
		" --buildplace . " +
		" --basetgz ./%(distro)s.tgz " +
		" --distribution %(distro)s " +
		" --aptconfdir apt.config " +
		" --override-config " +
		" --aptcache ./aptcache " +
		" --bindmounts %(resultdir)s " +
		" --mirror %(mirror)s " +
		" %(proxyoption)s " +
		"") % {
			'components': " ".join(components),
			'resultdir': resultdir,
			'command': command,
			'mirror': mirror,
			'distro': distribution,
			'proxyoption': proxyoption,
		})

	phase( "Building packages for '%s'"%distribution )
	for (_, srcpackage, version) in repositories :
		dscbase = srcpackage+"_"+version
		dscfiles = glob.glob(dscbase + "*.dsc")
		if not dscfiles: raise "No dsc file found for %s"%dscbase
		dscfile = dscfiles[-1]
		ret = run( ("pbuilder build "+
			" --buildplace . " +
			" --buildresult %(resultdir)s " +
			" --basetgz ./%(distro)s.tgz " +
			" --aptcache ./aptcache " +
			" --bindmounts %(resultdir)s " +
			" --hookdir hooks/ " +
			" %(proxyoption)s " +
			" %(dscfile)s " +
			"") % {
				'resultdir': resultdir,
				'distro': distribution,
				'dscfile': dscfile,
				'proxyoption': proxyoption,
		})
		if ret<0 and srcpackage == "clam" :
			print >> sys.stderr, "\033[31mCLAM LIBS COULD NOT BE COMPILED in", maindistro, distribution, "\033[0m"
			break
		# We need update the package for each package for interdependencies
		run("cd %(resultdir)s; /usr/bin/dpkg-scanpackages . /dev/null > Packages; cd -" %{
			'resultdir': resultdir,
		})
		run("gzip -c %s/Packages > %s/Packages.gz" %( resultdir, resultdir ) )

	if '--noupload' in sys.argv : continue
	phase( "Uploading packages for '%s'"%distribution )
	targetWebDir = "download/linux-%(maindistro)s-%(distro)s/"%{
		'resultdir' : resultdir,
		'maindistro' : maindistro,
		'distro': distribution,
	}

	if "~svn" in repositories[0][2]:
		targetWebDir += "svnsnapshots/"
	run("slogin clamadm@clam-project.org mkdir -p clam-project.org/%s" % targetWebDir )
	run("scp %s/* clamadm@clam-project.org:clam-project.org/%s " % ( resultdir, targetWebDir) )
	run("slogin clamadm@clam-project.org clam-project.org/scripts/regenerateDownloadDirsIndex.py")

if '--noupload' not in sys.argv :
	run("slogin clamadm@clam-project.org clam-project.org/scripts/regenerateDownloadDirsIndex.py")


if failedSteps :
	print "Those steps have failed:"
	for step in failedSteps :
		print step

