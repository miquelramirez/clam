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

localDefinitions = dict(
	name= 'linux_ubuntu_hardy',
	description= '<img src="http://clam.iua.upf.es/images/linux_icon.png"/> <img src="http://clam.iua.upf.es/images/ubuntu_icon.png"/>',
	sandbox= '$HOME/testfarm_sandboxes',
	installPath= '$HOME/testfarm_sandboxes/local',
	qt3dir='',
	qt4dir='',
	repositories = "clam acustica data_acustica clam/testdata clam/padova-speech-sms"
)
repositories = localDefinitions['repositories'].split()

client = Client(localDefinitions['name'])
client.brief_description = localDefinitions['description']


clam = Task(
		project = Project('CLAM','<a href="http://clam.iua.upf.edu">clam web</a>' ), 
	client = client, 
	task_name='svn up|DEBUG' 
	)
clam.set_check_for_new_commits( 
		checking_cmd='cd %(sandbox)s && svn status -u %(repositories)s | grep \*'%localDefinitions,
		minutes_idle=15
)
clam.add_subtask( 'List of new commits', [
	'cd %(sandbox)s/'%localDefinitions,
	] + [
		{CMD: 'echo "== Log for repository %(repo)s ==" ; svn log -r BASE:HEAD %(repo)s'%{'repo': repo}, INFO: lambda x:x }
		for repo in repositories
	] + [
		{CMD: 'svn up %(repo)s'%{'repo': repo}, INFO: lambda x:x }
		for repo in repositories
	] )

clam.add_subtask('count lines of code', [
	{CMD:'echo %(sandbox)s/clam/CLAM'%localDefinitions, STATS: lambda x: {'clam_loc': countLines(x) } },
	{CMD:'echo %(sandbox)s/clam/SMSTools'%localDefinitions, STATS: lambda x: {'smstools_loc': countLines(x) } },
	{CMD:'echo %(sandbox)s/clam/NetworkEditor'%localDefinitions, STATS: lambda x: {'networkeditor_loc': countLines(x) } },
] )
clam.add_deployment( [
	'cd %(sandbox)s/clam/CLAM'%localDefinitions,
	'rm -rf %(installPath)s/*'%localDefinitions,
	'cd %(sandbox)s/clam/CLAM/'%localDefinitions,
	'scons configure prefix=%(installPath)s release=0'%localDefinitions,
	'scons',
	'scons install',
] )

clam.add_subtask('Unit Tests', [
	'cd %(sandbox)s/clam/CLAM'%localDefinitions,
	'cd test',
	'scons test_data_path=%(sandbox)s/clam/testdata clam_prefix=%(installPath)s'%localDefinitions, # TODO: test_data_path and release
	'cd UnitTests',
	{INFO : lambda x:startTimer() }, 
	{CMD: './UnitTests'},
	{STATS : lambda x:{'exectime_unittests' : ellapsedTime()} },
] )
clam.add_subtask('Functional Tests', [
	'cd %(sandbox)s/clam/CLAM'%localDefinitions,
	'cd test',
	'scons test_data_path=%(sandbox)s/clam/testdata clam_prefix=%(installPath)s'%localDefinitions, # TODO: test_data_path and release
	'cd FunctionalTests',
	{INFO : lambda x:startTimer() }, 
	{CMD:'./FunctionalTests'},
	{STATS : lambda x: {'exectime_functests' : ellapsedTime()} },
] )
clam.add_subtask('CLAM Examples', [
	'cd %(sandbox)s/clam/CLAM/examples'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
] )

clam.add_subtask('SMSTools installation', [
	'cd %(sandbox)s/clam/SMSTools'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',
	'%(sandbox)s/clam/CLAM/scons/sconstools/changeExampleDataPath.py %(installPath)s/share/smstools '%localDefinitions,
] )


clam.add_subtask('vmqt4 compilation and examples', [
	'cd %(sandbox)s/clam/Annotator/vmqt'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s release=1 double=1'%localDefinitions,
	'scons examples',
] )
clam.add_subtask('Annotator installation', [
	'cd %(sandbox)s/clam/Annotator'%localDefinitions,
	'scons clam_vmqt4_path=vmqt prefix=%(installPath)s clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',
] )

clam.add_subtask('NetworkEditor installation', [
	'cd %(sandbox)s/clam/NetworkEditor'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',
] )



clam.add_subtask('BM-Audio update', [
	'cd %(sandbox)s/data_acustica'%localDefinitions,
	{CMD:'svn log -r BASE:HEAD', INFO: lambda x:x },
	'cd %(sandbox)s/acustica'%localDefinitions,
	{CMD:'svn log -r BASE:HEAD', INFO: lambda x:x },
	'cd %(sandbox)s/acustica/raytracing'%localDefinitions,
	'scons',
] )


clam.add_subtask("CLAM Plugins", [
	'cd %(sandbox)s/clam/CLAM/plugins/spacialization'%localDefinitions,
	'scons clam_prefix=%(installPath)s raytracing=0'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/spacialization'%localDefinitions,
	'scons clam_prefix=%(installPath)s raytracing=1'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/continuousExcitationSynthesizer'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/osc'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/speech'%localDefinitions,
	'scons clam_prefix=%(installPath)s'%localDefinitions,
	'scons install',
] )
clam.add_subtask('Back-to-back network tests', [
	'cd %(sandbox)s/clam/CLAM/plugins/spacialization'%localDefinitions,
	'./back2back.py',
] )
clam.add_subtask('BM-Audio tests back-to-back', [
	'cd %(sandbox)s/data_acustica/test_coreos'%localDefinitions,
	'./back2back.py',
	'cd %(sandbox)s/acustica/bformat2binaural'%localDefinitions,
	'./back2back.py',
] )

clam.add_subtask('Padova Speech SMS (external repository)', [
	'cd %(sandbox)s/clam/padova-speech-sms/'%localDefinitions,
	{CMD:'svn log -r BASE:HEAD', INFO: lambda x:x },
	{CMD: 'svn up', INFO: lambda x:x },
	'make',
] )

forceRun = len(sys.argv)>1
print "force Run: ", forceRun

Runner( clam, 
	continuous = False,
	first_run_always = forceRun,
	remote_server_url = 'http://localhost/testfarm_server'
#	local_base_dir='/tmp'
)

