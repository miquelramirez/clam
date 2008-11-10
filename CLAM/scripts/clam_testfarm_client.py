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
PATH = os.environ['PATH']
os.environ['LD_LIBRARY_PATH']='%s/testfarm_sandboxes/local/lib:/usr/local/lib' % HOME
os.environ['PATH']='%s/testfarm_sandboxes/local/bin:%s' % (HOME, PATH)
os.environ['CLAM_PLUGIN_PATH']='%s/testfarm_sandboxes/local/lib/clam' % HOME

localDefinitions = {
	'name': 'linux_ubuntu_hardy',
	'description': '<img src="http://clam.iua.upf.es/images/linux_icon.png"/> <img src="http://clam.iua.upf.es/images/ubuntu_icon.png"/>',
	'sandbox': '$HOME/testfarm_sandboxes/clam',
	'installPath': '$HOME/testfarm_sandboxes/local',
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
	{CMD: './UnitTests'},
	{STATS : lambda x:{'exectime_unittests' : ellapsedTime()} },
] )
clam.add_subtask('Functional Tests', [
	'cd %(sandbox)s/CLAM'%localDefinitions,
	'cd test',
	'scons test_data_path=%(sandbox)s/testdata clam_prefix=%(installPath)s'%localDefinitions, # TODO: test_data_path and release
	'cd FunctionalTests',
	{INFO : lambda x:startTimer() }, 
	{CMD:'./FunctionalTests'},
	{STATS : lambda x: {'exectime_functests' : ellapsedTime()} },
] )
clam.add_subtask('CLAM Examples', [
	'cd %(sandbox)s/CLAM/examples'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
] )

clam.add_subtask('SMSTools installation', [
	'cd %(sandbox)s/SMSTools'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',
	'%(sandbox)s/CLAM/scons/sconstools/changeExampleDataPath.py %(installPath)s/share/smstools '%localDefinitions,
] )


clam.add_subtask('vmqt4 compilation and examples', [
	'cd %(sandbox)s/Annotator/vmqt'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s release=1 double=1'%localDefinitions,
	'scons examples',
] )
clam.add_subtask('Annotator installation', [
	'cd %(sandbox)s/Annotator'%localDefinitions,
	'scons clam_vmqt4_path=vmqt prefix=%(installPath)s clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',
] )

clam.add_subtask('NetworkEditor installation', [
	'cd %(sandbox)s/NetworkEditor'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',
] )



clam.add_subtask('BM-Audio update', [
	'cd ~/data_acustica',
	{CMD:'svn log -r BASE:HEAD', INFO: lambda x:x },
	'cd ~/acustica',
	{CMD:'svn log -r BASE:HEAD', INFO: lambda x:x },
	'cd ~/acustica/raytracing',
	'scons',
] )


clam.add_subtask("CLAM Plugins", [
	'cd %(sandbox)s/CLAM/plugins/spacialization'%localDefinitions,
	'scons clam_prefix=%(installPath)s raytracing=0'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/CLAM/plugins/spacialization'%localDefinitions,
	'scons clam_prefix=%(installPath)s raytracing=1'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/CLAM/plugins/continuousExcitationSynthesizer'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/CLAM/plugins/osc'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/CLAM/plugins/speech'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',
] )
clam.add_subtask('Back-to-back network tests', [
	'cd %(sandbox)s/CLAM/plugins/spacialization'%localDefinitions,
	'./back2back.py',
] )
clam.add_subtask('BM-Audio tests back-to-back', [
	'cd ~/data_acustica/test_coreos',
	'./back2back.py',
	'cd ~/acustica/bformat2binaural',
	'./back2back.py',
] )

clam.add_subtask('Padova Speech SMS (external repository)', [
	'cd %(sandbox)s/padova-speech-sms/'%localDefinitions,
	{CMD:'svn log -r BASE:HEAD', INFO: lambda x:x },
	{CMD: 'svn up', INFO: lambda x:x },
	'make',
] )

Runner( clam, 
	continuous = False,
	first_run_always = True,
	remote_server_url = 'http://localhost/testfarm_server'
#	local_base_dir='/tmp'
)
