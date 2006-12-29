#!/usr/bin/python

import os.path
import glob
import sys
sconstoolspath = os.path.split(sys.argv[0])[0] + "/../scons/sconstools"
sys.path.append( sconstoolspath )
from versionInfo import packageVersionFromSvn

proxyoption = "--http-proxy 'http://proxy.upf.edu:8080/'"
proxyoption = ""
distributions = [
	('ubuntu', 'feisty', "http://es.archive.ubuntu.com/ubuntu/", ['main','universe']),
#	('ubuntu', 'edgy',   "http://es.archive.ubuntu.com/ubuntu/", ['main','universe']),
#	('debian', 'etch',   "http://ftp.de.debian.org/debian/", ['main']),
#	('debian', 'sid',    "http://ftp.de.debian.org/debian/", ['main']),
]
repositoryBase = "http://iua-share.upf.edu/svn/clam/trunk/"
repositories = [
	( 'CLAM',          'clam',               '0.96.1'),
	( 'Annotator',     'clam-annotator',     '0.3.4'),
	( 'NetworkEditor', 'clam-networkeditor', '0.4.1'),
	( 'SMSTools',      'clam-smstools',      '0.4.3'),
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
'''%{'srcpackage':repositories[0][1]+"-" + packageVersionFromSvn("CLAM")},
}

aptconfiguration = "APT::Get::AllowUnauthenticated 1;"

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

phase( "Setting up the environment" )
norun("echo Remember: run this as root, and configure proxy settings (both in the script and ~/.subversion/servers)")
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

for (package, srcpackage, _) in repositories :
	module = repositoryBase + package
	version = packageVersionFromSvn(package)
	srcdir = srcpackage + "-" + version
	run( "svn export --force %s %s"%(module, srcdir) )
#	if os.path.isdir(srcdir) :
#		run( "svn up %s"%(srcdir) )
#	else :
#		run( "svn co %s %s"%(module, srcdir) )
	run ( "dpkg-source -b %s"%(srcdir))


for (maindistro, distribution, mirror, components) in distributions :
	phase( "Preparing chroot for '%s'"%distribution )
	resultdir = '/var/cache/pbuilder/result'
	resultdir = os.path.abspath('./result-%(maindistro)s-%(distro)s' % {
		'maindistro' : maindistro,
		'distro': distribution,
	})
	# Assure that the first time a Package file exists
	run ("mkdir -p %(resultdir)s"%{'resultdir' : resultdir } )
	run ("touch %(resultdir)s/Packages"%{'resultdir': resultdir} )

	command = "create"
	if os.access("%s.tgz"%distribution, os.F_OK) :
		command = "update"

	run( ("COMPONENTS='%(components)s' pbuilder %(command)s " +
		" --othermirror 'deb file:%(resultdir)s ./' " +
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
		if not dscfiles:
			dscfile = dscbase+"-666.dsc"
		else :
			dscfile = dscfiles[-1]
		run( ("pbuilder build "+
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
		# We need update the package for each package for interdependencies
		run("cd %(resultdir)s; /usr/bin/dpkg-scanpackages . /dev/null > Packages; cd -" %{
			'resultdir': resultdir,
		})
		run("gzip -c %s/Packages > %s/Packages.gz" %( resultdir, resultdir ) )

	phase( "Uploading packages for '%s'"%distribution )
	targetWebDir = "download/linux-%(maindistro)s-%(distro)s/"%{
		'resultdir' : resultdir,
		'maindistro' : maindistro,
		'distro': distribution,
	}

	run("slogin clamadm@www.iua.upf.edu mkdir -p %s" % targetWebDir )
	run("scp %s/* clamadm@www.iua.upf.edu:%s " % ( resultdir, targetWebDir) )
	run("slogin clamadm@www.iua.upf.edu scripts/regenerateDownloadDirsIndex.py")






