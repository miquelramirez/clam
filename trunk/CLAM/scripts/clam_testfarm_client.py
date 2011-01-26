#! /usr/bin/python

# to install as a cron script, use this line in crontab
# 0,5,10,15,20,25,30,35,40,45,50,55 * * * *    (cd /home/parumi/testfarm_sandboxes/clam/CLAM/scripts && svn up clam_testfarm_client.py && ./runOnce.py lock ./clam_testfarm_client.py) 2>&1 | cat > /tmp/err_testfarm_clam


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

localDefinitions = dict(
	name= 'BM_lucid_32',
	description= '<img src="http://clam-project.org/images/linux_icon.png"/> <img src="http://clam-project.org/images/ubuntu_icon.png"/>',
#	repositories = "clam acustica data_acustica clam/testdata clam/padova-speech-sms",
	repositories = "clam acustica data_acustica clam/testdata",
	private_repositories = "acustica data_acustica",
	sandbox= os.path.expanduser('~/'),
	qt3dir='',
	qt4dir='',
	extraLibOptions = 'release=0',
	extraAppOptions = '',
)
localDefinitions['installPath'] = os.path.join(localDefinitions['sandbox'],"local")
repositories = localDefinitions['repositories'].split()
private_repositories = localDefinitions['private_repositories'].split()
os.environ['LD_LIBRARY_PATH']='%(installPath)s/lib:/usr/local/lib' %localDefinitions
os.environ['PATH']='%(installPath)s/bin:' % localDefinitions + os.environ['PATH']
os.environ['CLAM_PLUGIN_PATH']='%(installPath)s/lib/clam' % localDefinitions
os.environ['LADSPA_PATH']='%(installPath)s/lib/ladspa' % localDefinitions

client = Client(localDefinitions['name'])
client.brief_description = localDefinitions['description']


clam = Task(
	project = Project('CLAM','<a href="http://clam-project.org">clam web</a>' ), 
	client = client, 
	task_name='svn up|DEBUG' 
	)

clam.set_repositories_to_keep_state_of(repositories + localDefinitions['private_repositories'].split())

clam.set_check_for_new_commits( 
	checking_cmd='for a in %(repositories)s; do ( cd %(sandbox)s/$a && svn status -u); done | grep \'[*!]\''%localDefinitions,
	minutes_idle=15
)

clam.add_subtask( 'List of new commits', [
	'cd %(sandbox)s/'%localDefinitions,
	] + [
		# 'true' is needed in order testfarm not to catch the 'cd'
		{CMD: 'true ; cd %s; svn log -r BASE:HEAD; cd -'%repo, INFO: lambda x:x }
		for repo in repositories if repo not in private_repositories
	] + [
		{CMD: 'true ; cd %s; svn log -q -r BASE:HEAD; cd -'%repo, INFO: lambda x:x }
		for repo in private_repositories
	] + [
		{CMD: '(cd %s ; svn up --accept postpone )'%repo, INFO: lambda x:x }
		for repo in repositories if repo not in private_repositories
	] + [
		{CMD: '(cd %s ; svn up --accept postpone )'%repo, INFO: lambda x:'No available' }
		for repo in private_repositories
	] )
clam.add_subtask('count lines of code', [
	{CMD:'echo %(sandbox)s/clam/CLAM'%localDefinitions, STATS: lambda x: {'clam_loc': countLines(x) } },
	{CMD:'echo %(sandbox)s/clam/SMSTools'%localDefinitions, STATS: lambda x: {'smstools_loc': countLines(x) } },
	{CMD:'echo %(sandbox)s/clam/NetworkEditor'%localDefinitions, STATS: lambda x: {'networkeditor_loc': countLines(x) } },
] )
clam.add_deployment( [
	'cd %(sandbox)s/clam/CLAM'%localDefinitions,
	'rm -rf %(installPath)s/*'%localDefinitions,
	'scons configure prefix=%(installPath)s %(extraLibOptions)s'%localDefinitions,
	'scons',
	'scons install',
	'mkdir -p %(installPath)s/bin'%localDefinitions,
] )

clam.add_subtask('Unit Tests', [
	'cd %(sandbox)s/clam/CLAM/test'%localDefinitions,
	'scons test_data_path=%(sandbox)s/clam/testdata clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions, # TODO: test_data_path and release
	'cd UnitTests',
	{INFO : lambda x:startTimer() }, 
	{CMD: './UnitTests'},
	{STATS : lambda x:{'exectime_unittests' : ellapsedTime()} },
] )
clam.add_subtask('Functional Tests', [
	'cd %(sandbox)s/clam/CLAM/test'%localDefinitions,
	'scons test_data_path=%(sandbox)s/clam/testdata clam_prefix=%(installPath)s'%localDefinitions, # TODO: test_data_path and release
	'cd FunctionalTests',
	{INFO : lambda x:startTimer() }, 
	{CMD:'./FunctionalTests'},
	{STATS : lambda x: {'exectime_functests' : ellapsedTime()} },
] )
clam.add_subtask('CLAM Examples', [
	'cd %(sandbox)s/clam/CLAM/examples'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
] )

clam.add_subtask('BM-Audio Soxsucks', [
	'cd %(sandbox)s/acustica/src/soxsucks/'%localDefinitions,
	'scons',
	'ln -sf %(sandbox)s/acustica/src/soxsucks/soxsucks %(installPath)s/bin'%localDefinitions,
] )

clam.add_subtask('BM-Audio Raytracing', [
	'cd %(sandbox)s/acustica/src/raytracing'%localDefinitions,
	'scons',
	'ln -sf %(sandbox)s/acustica/src/raytracing/raytracing %(installPath)s/bin'%localDefinitions,
	'./back2back.py',
] )

clam.add_subtask('CLAM Plugins', [
	'cd %(sandbox)s/clam/CLAM/plugins/spacialization'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s raytracing=disabled'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/spacialization'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s raytracing=traditional'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/spacialization/ladspa/'%localDefinitions,
	'scons -c',
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/continuousExcitationSynthesizer'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/GuitarEffects/'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/MIDI/'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/sndfile'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/osc'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/speech'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',
	
	'cd %(sandbox)s/clam/CLAM/plugins/Filters'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',
	
	'cd %(sandbox)s/clam/CLAM/plugins/resampling'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',
	
	'cd %(sandbox)s/clam/CLAM/plugins/expression'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',
] )

clam.add_subtask('NetworkEditor installation', [
	'cd %(sandbox)s/clam/NetworkEditor'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
	'scons install',
] )

clam.add_subtask('Back-to-back network tests', [
	'cd %(sandbox)s/clam/CLAM/plugins/spacialization'%localDefinitions,
	'./back2back.py',
] )

clam.add_subtask('BM-Audio back-to-back tests', [
	'ln -sf %(sandbox)s/acustica/src/rendercoreo/rendercoreo %(installPath)s/bin'%localDefinitions,
	'cd %(sandbox)s/acustica/bformat2binaural'%localDefinitions,
	'./back2back.py',
	'cd %(sandbox)s/data_acustica/test_coreos'%localDefinitions,
	'./back2back.py',
	'cd %(sandbox)s/acustica/src/bm_clam_plugin'%localDefinitions,
	'scons clam_prefix=%(installPath)s' % localDefinitions,
	'scons install',
	'./back2back_decoders.py',
#	'./back2back_upmix.py',
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
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s %(extraAppOptions)s '%localDefinitions,
	'scons install',
] )

clam.add_subtask('Chordata installation', [
	'cd %(sandbox)s/clam/chordata'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s %(extraAppOptions)s '%localDefinitions,
	'scons install',
] )

clam.add_subtask('Voice2MIDI installation', [
	'cd %(sandbox)s/clam/Voice2MIDI'%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s %(extraAppOptions)s '%localDefinitions,
	'scons install',
] )


"""
clam.add_subtask('Padova Speech SMS (external repository)', [
	'cd %(sandbox)s/clam/padova-speech-sms/'%localDefinitions,
	{CMD:'svn log -r BASE:HEAD', INFO: lambda x:x },
	{CMD: 'svn up', INFO: lambda x:x },
	'make',
] )

clam.add_subtask('pyCLAM update', [
	'cd %(sandbox)s/clam/pyclam'%localDefinitions,
	{CMD:'svn log -r BASE:HEAD', INFO: lambda x:x },
	 './generate_bindings.py && scons',
] )
clam.add_subtask('pyCLAM Unit Tests', [
	'cd %(sandbox)s/clam/pyclam'%localDefinitions,
	'cd test',
	{INFO : lambda x:startTimer() },
	{CMD: './UnitTestsSuite.py'},
	{STATS : lambda x:{'exectime_unittests' : ellapsedTime()} },
])
"""

if '--slow-tests' in sys.argv :
	dirs = [
		'clam/NetworkEditor',
		'clam/CLAM/plugins/continuousExcitationSynthesizer',
		'clam/CLAM/plugins/GuitarEffects',
		'clam/CLAM/plugins/osc',
		'clam/CLAM/plugins/sndfile',
		'clam/CLAM/plugins/Filters',
		'clam/CLAM/plugins/MIDI',
		'clam/CLAM/plugins/resampling',
		'clam/CLAM/plugins/samplebysample',
		'clam/CLAM/plugins/spacialization',
		'clam/CLAM/plugins/speech',
		]
	for d in dirs :
		clam.add_subtask('Check Clam Networks Recursively in %s' % d, [
			'cd %(sandbox)s/clam/CLAM/scripts'%localDefinitions,
			{CMD: './check_clam_networks_recursively.py -b %(sandbox)s/'%localDefinitions + d },
		] )

forceRun = len(sys.argv)>1
print "force Run: ", forceRun

Runner( clam, 
	continuous = False,
	first_run_always = forceRun,
	remote_server_url = 'http://84.88.76.186/testfarm_server',
#	local_base_dir='/tmp',
)

