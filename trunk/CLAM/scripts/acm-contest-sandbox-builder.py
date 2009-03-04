#!/usr/bin/python


import os, sys


def main(development = True):
	sourceModules = [
		('src/CLAM', 'CLAM'),
		('src/MusicAnnotator', 'CLAM_Annotator'),
		('src/NetworkEditor', 'CLAM_NetworkEditor'),
		('src/SMSTools', 'CLAM_SMSTools'),
	] 
	
	binaryPlatforms = {
		'': [
		],
		'doc': [
			'http://clam-project.org/doc/icmc05-clam.pdf'
		
		],
		'installers/windows' : [
			'http://clam-project.org/download/win/CLAM-SMSTools-0.4.1-CVS-20060601-1_setup.exe',
			'http://clam-project.org/download/win/CLAM-NetworkEditor-0.3.1-CVS-20060601-1_setup.exe',
			'http://clam-project.org/download/win/CLAM-Annotator-0.3.2-CVS-20060601-1_setup.exe'
		],
#		'installers/macosx/tiger' : [
#			'http://clam-project.org/download/mac/external-libraries-for-CLAM-0.90.dmg',
#			'http://clam-project.org/download/mac/CLAM-libs-0.90.0.dmg',
#			'http://clam-project.org/download/mac/CLAM_QtSMSTools-0.4.dmg',
#			'http://clam-project.org/download/mac/CLAM_NetworkEditor-0.3.dmg',
#			'http://clam-project.org/download/mac/CLAM_Annotator-0.2.dmg',
#		],
		'installers/linux/fedora5' : [
			'http://clam-project.org/download/linux-fc5/CLAM-0.91.0-3.i386.rpm',
			'http://clam-project.org/download/linux-fc5/CLAM-devel-0.91.0-3.i386.rpm',
			'http://clam-project.org/download/linux-fc5/CLAM-applications-0.91.0-3.i386.rpm',
			'http://rpm.livna.org/fedora/5/i386/libmad-0.15.1-2.b.lvn5.i386.rpm',
			'http://rpm.livna.org/fedora/5/i386/libmad-devel-0.15.1-2.b.lvn5.i386.rpm',
			'http://ccrma.stanford.edu/planetccrma/mirror/fedora/linux/planetccrma/5/i386/jack-audio-connection-kit-devel-0.101.0-0.2.cvs.rhfc5.ccrma.i386.rpm',
			'http://ccrma.stanford.edu/planetccrma/mirror/fedora/linux/planetccrma/5/i386/jack-audio-connection-kit-0.101.0-0.2.cvs.rhfc5.ccrma.i386.rpm',
			'ftp://ftp.pbone.net/mirror/apt.kde-redhat.org/kde-redhat/fedora/5/i386/RPMS.stable/qt4-devel-4.1.3-3.fc5.i386.rpm',
			'ftp://ftp.pbone.net/mirror/apt.kde-redhat.org/kde-redhat/fedora/5/i386/RPMS.stable/qt4-4.1.3-3.fc5.i386.rpm',
			'ftp://ftp.pbone.net/mirror/apt.kde-redhat.org/kde-redhat/fedora/5/i386/RPMS.stable/qt4-config-4.1.3-3.fc5.i386.rpm',
		],
	}

	blackList = [
		'src/CLAM/build',
		'src/NetworkEditor/build',
		'src/SMSTools/scons/',
		'src/SMSTools/build/',
		'src/SMSTools/draft/'
		'src/MusicAnnotator/build',
		'src/MusicAnnotator/buildTest',
	]

	txtdocs = [
			('INSTALL.txt', 'http://iua-share.upf.es/wikis/clam_draft/INSTALL.txt?action=raw'),
			('README.txt','http://iua-share.upf.es/wikis/clam_draft/README.txt?action=raw'),
	]

	docs = [
		('doc/MusicAnnotator', 'Music_Annotator_tutorial'),
		('doc/SMSTools', 'SMSTools_tutorial'),
		('doc/NetworkEditor', 'Network_Editor_tutorial'),
		]

	for file,url in txtdocs :
		os.system("wget -q --user-agent='' -O'%s' '%s'"%(file,url))

	for base,page in docs :
	#	base = 'doc/MusicAnnotator'
	#	page = 'Music_Annotator_tutorial'
		print base, page
		os.system("wget -q -c -H -k -nd -P'%s' -p 'http://clam-project.org/wiki/%s?printable=yes'"%(base,page))
		os.rename(base+'/index.php?title=%s&printable=yes'%page, base+'/index.html')

	for sourceModule in sourceModules :
		if development :
			os.system('cvs -q co -P -d "%s" "%s"'%sourceModule)
		else :
			os.system('cvs export -Dnow -d "%s" "%s"'%sourceModule)

	for location, binaries in binaryPlatforms.items() :
		os.system('wget -c -P "%s" %s'%(location, " ".join(["'"+url+"'" for url in binaries]))) 

	#windows external libraries
	os.system('wget -c -P src/ "http://clam-project.org/download/win/windows-external-libs.tar.gz"')
	os.system('cd src && tar xvzf windows-external-libs.tar.gz' )
	os.system('cd src && rm windows-external-libs.tar.gz')
		

	for root, dirs, files in os.walk('.') :
		if '.not-public' not in files: continue
		print 'Prunning %s because not public'%root
		os.system('rm -rf "%s"'%root)

	for prunable in blackList :
		print "Removing '%s'..."%prunable
		os.system('rm -rf "%s"'%prunable)


print 'usage: ./buildSandbox.py tarball [version]'

if len(sys.argv) ==  1:
	main()
else:
	if len(sys.argv) == 3:
		version = '-'+sys.argv[2]
	else:
		version = ''
	
	tarball_base = 'CLAM-acm-contest%s' % version
	os.system('cvs export -Dnow -d %s CLAM-acm-contest' % tarball_base)
	os.chdir( tarball_base )
	main( development = False)
	
	if sys.argv[1] == 'tarball':
		os.system('rm buildSandbox.py')
		os.system('cd .. && tar cvzf %s.tar.gz %s' % (tarball_base, tarball_base) )
