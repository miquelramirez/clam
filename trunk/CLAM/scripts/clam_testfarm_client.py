#! /usr/bin/python
import os, sys, time
sys.path.append('%s/testfarm/src' % os.environ['HOME'])
from task import *
from project import Project
from client import Client
from runner import Runner
from commands import getoutput

def countLines( path ):
	print 'loc for path:', path
	lines =  getoutput("find %s -name '*.?xx' -exec wc -l {} \;" % path.strip() ).split('\n')
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

def set_qtdir_to_qt4(x) :
	os.environ['QTDIR']=localDefinitions['qt4dir']
	if os.environ['QTDIR'] and sys.platform=='darwin':
		os.environ['PKG_CONFIG_PATH']=os.environ['QTDIR']+'lib/pkgconfig'
def set_qtdir_to_qt3(x) :
	os.environ['QTDIR']=localDefinitions['qt3dir']

localDefinitions = {
	'name': 'linux_ubuntu_feisty',
	'description': '<img src="http://clam.iua.upf.es/images/linux_icon.png"/> <img src="http://clam.iua.upf.es/images/ubuntu_icon.png"/>',
	'sandbox': '$HOME/clam',
	'installPath': '$HOME/local',
	'qt3dir':'',
	'qt4dir':'',
}

client = Client(localDefinitions['name'])
client.brief_description = localDefinitions['description']
	

clam = Task(
		project = Project('CLAM','<a href="http://clam.iua.upf.edu">clam web</a>' ), 
	client = client, 
	task_name='svn up|DEBUG' 
	)
clam.set_check_for_new_commits( 
		checking_cmd='cd %(sandbox)s && svn status -u | grep \*'%localDefinitions,
		minutes_idle=15
)

clam.add_subtask( 'List of new commits', [
	'cd %(sandbox)s'%localDefinitions,
	{CMD:'svn log -r BASE:HEAD', INFO: lambda x:x },
	{CMD: 'svn up', INFO: lambda x:x },
	] )

clam.add_subtask('count lines of code', [
	{CMD:'echo %(sandbox)s/CLAM'%localDefinitions, STATS: lambda x: {'clam_loc': countLines(x) } },
	{CMD:'echo %(sandbox)s/SMSTools'%localDefinitions, STATS: lambda x: {'smstools_loc': countLines(x) } },
	{CMD:'echo %(sandbox)s/NetworkEditor'%localDefinitions, STATS: lambda x: {'networkeditor_loc': countLines(x) } },
] )
clam.add_deployment( [
	'cd %(sandbox)s/CLAM'%localDefinitions,
	'rm -rf %(installPath)s/*'%localDefinitions,
	'cd %(sandbox)s/CLAM/'%localDefinitions,
	'scons configure prefix=%(installPath)s release=0'%localDefinitions,
	'scons',
	'scons install',
] )

clam.add_subtask('Unit Tests', [
	'cd %(sandbox)s/CLAM'%localDefinitions,
	'cd test',
	'scons test_data_path=%(sandbox)s/testdata clam_prefix=%(installPath)s'%localDefinitions, # TODO: test_data_path and release
	'cd UnitTests',
	{INFO : lambda x:startTimer() }, 
	{CMD: './UnitTests', INFO: lambda x:x},
	{STATS : lambda x:{'exectime_unittests' : ellapsedTime()} },
] )
clam.add_subtask('Functional Tests', [
	'cd %(sandbox)s/CLAM'%localDefinitions,
	'cd test',
	'scons test_data_path=%(sandbox)s/testdata clam_prefix=%(installPath)s'%localDefinitions, # TODO: test_data_path and release
	'cd FunctionalTests',
	{INFO : lambda x:startTimer() }, 
	{CMD:'./FunctionalTests', INFO: lambda x:x},
	{STATS : lambda x: {'exectime_functests' : ellapsedTime()} },
] )
clam.add_subtask('CLAM Examples', [
	{CMD: 'echo set QTDIR to qt4', INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/CLAM/examples'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
] )

clam.add_subtask('SMSTools installation', [
	{CMD: 'echo setting QTDIR to qt3 path ', INFO: set_qtdir_to_qt3},
	'cd %(sandbox)s/SMSTools'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',
	'%(sandbox)s/CLAM/scons/sconstools/changeExampleDataPath.py %(installPath)s/share/smstools '%localDefinitions,
] )


clam.add_subtask('vmqt4 compilation and examples', [
	{CMD: 'echo setting QTDIR to qt4 path ', INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/Annotator/vmqt'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s release=1 double=1'%localDefinitions,
	'scons examples',
] )
clam.add_subtask('Annotator installation', [
	{CMD: 'echo setting QTDIR to qt4 path ', INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/Annotator'%localDefinitions,
	'scons clam_vmqt4_path=vmqt prefix=%(installPath)s clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',
] )

clam.add_subtask('NetworkEditor installation', [
	{CMD: 'echo setting QTDIR to qt4 path ', INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/NetworkEditor'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',
	'%(sandbox)s/CLAM/scons/sconstools/changeExampleDataPath.py %(installPath)s/share/smstools '%localDefinitions,
] )
clam.add_subtask('Plugins compilation', [
	{CMD: 'echo setting QTDIR to qt4 path ', INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/CLAM/plugins/spacialization'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/CLAM/plugins/continuousExcitationSynthesizer'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/CLAM/plugins/oscSource'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/CLAM/plugins/speech'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',
] )
clam.add_subtask('Back-to-back network tests', [
	{CMD: 'echo setting QTDIR to qt4 path ', INFO: set_qtdir_to_qt4},
	'cd %(sandbox)s/CLAM/plugins/spacialization'%localDefinitions,
	'CLAM_PLUGIN_PATH=. ../../../NetworkEditor/OfflinePlayer simple-interpolation.clamnetwork metronom.wav simple-interpolation-result.wav'%localDefinitions,
	'diff simple-interpolation-expected.wav simple-interpolation-result.wav'
] )

clam.add_subtask('Padova Speech SMS (external repository)', [
	'cd %(sandbox)s/padova-speech-sms/src'%localDefinitions,
	{CMD:'svn log -r BASE:HEAD', INFO: lambda x:x },
	{CMD: 'svn up', INFO: lambda x:x },
	'make',
] )

Runner( clam, 
	continuous = True,
	remote_server_url = 'http://efpc072.upf.es/testfarm_server'
#	local_base_dir='/tmp'
)

