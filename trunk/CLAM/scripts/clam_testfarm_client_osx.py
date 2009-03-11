#! /opt/local/bin/python2.4

import os, sys, time
sys.path.append('%s/testfarm/src' % os.environ['HOME'])
from task import *
from project import Project
from client import Client
from runner import Runner
from commands import getoutput

def countLines( path ):
	print "loc for path:", path
	lines =  getoutput("find %s -name '*.?xx' -exec wc -l {} \;" % path.strip() ).split("\n")
	return reduce( lambda x,y: x+y , map( lambda x: int(x.split()[0]), lines) )

startTime = -1
def startTimer():
	global startTime
	startTime = time.time()
def ellapsedTime():
	global startTime
	return time.time() - startTime

HOME = os.environ['HOME']
os.environ['LD_LIBRARY_PATH']='%s/local/lib:/usr/local/lib' % HOME
os.environ['PKG_CONFIG_PATH']='/usr/local/'

def set_qtdir_to_qt4(x) :
	os.environ['QTDIR']=localDefinitions['qt4dir']
	if os.environ['QTDIR'] and sys.platform=='darwin':
		os.environ['PKG_CONFIG_PATH']=os.environ['QTDIR']+'lib/pkgconfig'
def set_qtdir_to_qt3(x) :
	os.environ['QTDIR']=localDefinitions['qt3dir']

sandbox = os.path.expanduser('~/')
localDefinitions = dict(
	name = 'mac-10.4.8-intel-macbook',
	description = '<img src="http://clam-project.org/images/apple_icon.png"/>',
	sandbox = sandbox,
	repositories = "clam clam/testdata",
	installPath = os.path.join(sandbox,"local"),
	qt3dir ='/opt/local/',
#	qt4dir ='/usr/local/Trolltech/Qt-4.2.2/',
	qt4dir ='/opt/local/',
	cppunit_prefix = os.path.join(sandbox,"local"),
	packageWildcard ='*.dmg',
	downloadPlatform ='mac',
	extraLibOptions = '',
	extraAppOptions = '',
)
repositories = localDefinitions['repositories'].split()

client = Client(localDefinitions['name'])
client.brief_description = localDefinitions['description']


clam = Task(
	project = Project("CLAM"), 
	client = client, 
	task_name="with svn update" 
	)
clam.set_check_for_new_commits( 
	checking_cmd='cd %(sandbox)s && svn status -u %(repositories)s | grep \'[*!]\''%localDefinitions,
	minutes_idle=15
)
clam.add_subtask( 'List of new commits', [
	'cd %(sandbox)s/'%localDefinitions,
	] + [
		# 'true' is needed in order testfarm not to catch the 'cd'
		{CMD: 'true ; cd %s; svn log -r BASE:HEAD; cd -'%repo, INFO: lambda x:x }
		for repo in repositories
	] + [
		{CMD: 'svn up --accept postpone %s'%repo, INFO: lambda x:x }
		for repo in repositories
	] )

clam.add_subtask('count lines of code', [
	{CMD:'echo %(sandbox)s/clam/CLAM'%localDefinitions, STATS: lambda x: {'clam_loc': countLines(x) } },
	{CMD:'echo %(sandbox)s/clam/SMSTools'%localDefinitions, STATS: lambda x: {'smstools_loc': countLines(x) } },
	{CMD:'echo %(sandbox)s/clam/NetworkEditor'%localDefinitions, STATS: lambda x: {'networkeditor_loc': countLines(x) } },
] )
clam.add_deployment( [
	'cd %(sandbox)s/clam/CLAM'%localDefinitions,
#	'rm -rf %(installPath)s/*'%localDefinitions,
	'scons configure prefix=%(installPath)s %(extraLibOptions)s'%localDefinitions,
	'scons',
	'scons install',
] )

clam.add_subtask('Unit Tests', [
	'cd %(sandbox)s/clam/CLAM'%localDefinitions,
	'cd test',
	'scons test_data_path=%(sandbox)s/clam/testdata clam_prefix=%(installPath)s'%localDefinitions, # TODO: test_data_path and release
	{INFO : lambda x:startTimer() }, 
	{CMD: 'scons run_unit_tests', INFO: lambda x:x, STATUS_OK: lambda x:True},
	{STATS : lambda x:{'exectime_unittests' : ellapsedTime()} },
] )
clam.add_subtask('Functional Tests', [
	'cd %(sandbox)s/clam/CLAM'%localDefinitions,
	'cd test',
	'scons test_data_path=%(sandbox)s/clam/testdata clam_prefix=%(installPath)s'%localDefinitions, # TODO: test_data_path and release
	{INFO : lambda x:startTimer() }, 
	{CMD:'scons run_functional_tests', INFO: lambda x:x, STATUS_OK: lambda x:True},
	{STATS : lambda x: {'exectime_functests' : ellapsedTime()} },
] )

clam.add_subtask('CLAM Examples', [
	{CMD: 'echo set QTDIR to qt4', INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/clam/CLAM/examples'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
] )
clam.add_subtask('CLAM Plugins', [
	{CMD: "echo set QTDIR to qt4", INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/clam/CLAM/plugins/spacialization'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/continuousExcitationSynthesizer'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/GuitarEffects/'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/osc'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/speech'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',
] )
clam.add_subtask('Back-to-back network tests', [
	'cd %(sandbox)s/clam/CLAM/plugins/spacialization'%localDefinitions,
	'./back2back.py',
] )
"""
clam.add_subtask('SMSTools packaging', [
	{CMD: 'echo setting QTDIR to qt3 path ', INFO: set_qtdir_to_qt3},
	'cd %(sandbox)s/clam/SMSTools'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s'%localDefinitions,
	'%(sandbox)s/clam/CLAM/scons/sconstools/changeExampleDataPath.py %(installPath)s/share/smstools '%localDefinitions,
	'rm -f %(packageWildcard)s'%localDefinitions,
	'scons package',
	'ls *svn1* > /dev/null || scp %(packageWildcard)s clamadm@clam-project.org:clam-project.org/download/%(downloadPlatform)s/svnsnapshots/'%localDefinitions,
	'ls *svn1* > /dev/null || slogin clamadm@clam-project.org clam-project.org/scripts/regenerateDownloadDirsIndex.py',
] )
"""
clam.add_subtask('vmqt4 compilation and examples', [
	{CMD: 'echo setting QTDIR to qt4 path ', INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/clam/Annotator/vmqt'%localDefinitions,
	'scons clam_prefix=%(installPath)s release=1 %(extraAppOptions)s'%localDefinitions,
	'scons examples',
] )
clam.add_subtask('Annotator packaging', [
	{CMD: 'echo setting QTDIR to qt4 path ', INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/clam/Annotator'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s %(extraAppOptions)s '%localDefinitions,
	'rm -f %(packageWildcard)s'%localDefinitions,
	'scons package',
#	'ls *svn1* > /dev/null || scp %(packageWildcard)s clamadm@clam-project.org:clam-project.org/download/%(downloadPlatform)s/svnsnapshots/'%localDefinitions,
#	'ls *svn1* > /dev/null || slogin clamadm@clam-project.org clam-project.org/scripts/regenerateDownloadDirsIndex.py',
] )

clam.add_subtask('NetworkEditor packaging', [
	{CMD: 'echo setting QTDIR to qt4 path ', INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/clam/NetworkEditor'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s  %(extraAppOptions)s '%localDefinitions,
	'%(sandbox)s/clam/CLAM/scons/sconstools/changeExampleDataPath.py %(installPath)s/share/smstools '%localDefinitions,
	'rm -f %(packageWildcard)s'%localDefinitions,
	'scons package',
#	'ls *svn1* > /dev/null || scp %(packageWildcard)s clamadm@clam-project.org:clam-project.org/download/%(downloadPlatform)s/svnsnapshots/'%localDefinitions,
#	'ls *svn1* > /dev/null || slogin clamadm@clam-project.org clam-project.org/scripts/regenerateDownloadDirsIndex.py',
] )

clam.add_subtask('Chordata packaging', [
	{CMD: 'echo setting QTDIR to qt4 path ', INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/clam/chordata'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s  %(extraAppOptions)s '%localDefinitions,
	'scons package',
	'ls *svn1* > /dev/null || scp %(packageWildcard)s clamadm@clam-project.org:clam-project.org/download/%(downloadPlatform)s/svnsnapshots/'%localDefinitions,
	'ls *svn1* > /dev/null || slogin clamadm@clam-project.org clam-project.org/scripts/regenerateDownloadDirsIndex.py',
] )

clam.add_subtask('Voice2MIDI packaging', [
	{CMD: 'echo setting QTDIR to qt4 path ', INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/clam/Voice2MIDI'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s  %(extraAppOptions)s '%localDefinitions,
	'%(sandbox)s/clam/CLAM/scons/sconstools/changeExampleDataPath.py %(installPath)s/share/smstools '%localDefinitions,
	'rm -f %(packageWildcard)s'%localDefinitions,
#	'scons package',
#	'ls *svn1* > /dev/null ; scp %(packageWildcard)s clamadm@clam-project.org:clam-project.org/download/%(downloadPlatform)s/svnsnapshots/'%localDefinitions,
#	'ls *svn1* > /dev/null ; slogin clamadm@clam-project.org clam-project.org/scripts/regenerateDownloadDirsIndex.py',
] )

forceRun = len(sys.argv)>1
print "force Run: ", forceRun

Runner( clam, 
	continuous = False,
	first_run_always = forceRun,
	remote_server_url = 'http://84.88.76.190/testfarm_server',
#	local_base_dir='/tmp',
)

