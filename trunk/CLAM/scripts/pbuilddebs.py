import os.path
import glob
import sys

proxyoption = "--http-proxy 'http://proxy.upf.edu:8080/'"
#proxyoption = ""
distributions = [
#	('ubuntu', 'feisty', "http://ad.archive.ubuntu.com/ubuntu/", ['main','universe']),
#	('ubuntu', 'edgy',   "http://ad.archive.ubuntu.com/ubuntu/", ['main','universe']),
#	('debian', 'etch',   "http://ftp.de.debian.org/debian/", ['main']),
	('debian', 'sid',    "http://ftp.de.debian.org/debian/", ['main']),
]
repositoryBase = "http://iua-share.upf.edu/svn/clam/trunk/"
repositories = [
	( repositoryBase+'CLAM',          'clam',               '0.92.0'),
	( repositoryBase+'Annotator',     'clam-annotator',     '0.3.3'),
	( repositoryBase+'NetworkEditor', 'clam-networkeditor', '0.4.0'),
	( repositoryBase+'SMSTools',      'clam-smstools',      '0.4.2'),
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
'''%{'srcpackage':repositories[0][1]+"-"+repositories[0][2]},
}

aptconfiguration = "APT::Get::AllowUnauthenticated 1;"

def run(command) :
	print ":: ", command
	for line in os.popen(command) :
		print line,
		sys.stdout.flush()
def norun(command) :
	print "XX ", command

print "== Setting up the environment"

run ("mkdir -p hooks")
run ("mkdir -p aptcache")
run ("mkdir -p apt.config/apt.conf.d")

aptconf = file("apt.config/apt.conf.d/allow-unauthenticated",'w')
aptconf.write(aptconfiguration)
aptconf.close()

for (name, content) in hooks.items() :
	print "Generating %s"%name
	hookfile = file("hooks/"+name,'w')
	hookfile.write(content)
	hookfile.close()

run ("chmod a+x hooks/*")

print "== Obtaining latest sources"

for (module, srcpackage, version) in repositories :
	srcdir = srcpackage + "-" + version
	if os.path.isdir(srcdir) :
		run( "svn up %s"%(srcdir) )
	else :
		run( "svn co %s %s"%(module, srcdir) )
	run ( "dpkg-source -b %s"%(srcdir))


for (maindistro, distribution, mirror, components) in distributions :
	print "== Preparing chroot for '%s'"%distribution
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

	print "== Building packages for '%s'"%distribution
	for (foo, srcpackage, version) in repositories :
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

	print "== Uploading packages for '%s'"%distribution
	norun("slogin clamadm@www.iua.upf.edu mkdir -p download/linux-%(maindistro)s-%(distro)s")
	norun("scp %(resultdir)s/* clamadm@www.iua.upf.edu:download/linux-%(maindistro)s-%(distro)s/"%{
		'resultdir' : resultdir,
		'maindistro' : maindistro,
		'distro': distribution,
		})
	norun("slogin clamadm@www.iua.upf.edu scripts/regenerateDownloadDirsIndex.py")
	
"""
cd /var/cache/pbuilder/result/
/usr/bin/dpkg-scanpackages . /dev/null > Packages
gzip -c Packages > Packages.gz
scp /var/cache/pbuilder/result/* clamadm@www.iua.upf.edu:download/linux-debian-sid/
slogin clamadm@www.iua.upf.edu scripts/regenerateDownloadDirsIndex.py
"""





