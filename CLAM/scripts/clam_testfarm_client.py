#! /usr/bin/python

# to install as a cron script, use this line in crontab
# 0,5,10,15,20,25,30,35,40,45,50,55 * * * *    (cd /home/parumi/testfarm_sandboxes/clam/CLAM/scripts && svn up clam_testfarm_client.py && ./runOnce.py lock ./clam_testfarm_client.py) 2>&1 | cat > /tmp/err_testfarm_clam


import os, sys, time
sys.path.append('%s/testfarm/v2' % os.environ['HOME'])
from testfarm.v1.task import *
from testfarm.v1.project import Project
from testfarm.v1.client import Client
from testfarm.v1.runner import Runner
from testfarm.svnsandbox import SvnSandbox
from testfarm.utils import loadDictFile
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

slowTests = '--slow-tests' in sys.argv

config = dict(
	description= '<img src="http://clam-project.org/images/linux_icon.png"/> <img src="http://clam-project.org/images/ubuntu_icon.png"/>',
	repositories = "clam clam-test-data", # "acustica clam/padova-speech-sms",
	private_repositories = "",
	sandbox= os.path.expanduser('~/'),
	extraLibOptions = 'release=0',
	extraAppOptions = '',
	testfarm_page = 'http://clam-project.org/testfarm.html',
)
try :
	config.update(loadDictFile(os.path.expanduser('~/.config/testfarmrc')))
	config['name'] # ensure that name is defined
	config['description']
except :
	print >> sys.stderr, "ERROR: You should create ~/.config/testfarmrc with at least the name and description attributes of your client"
	raise

config['installPath'] = os.path.join(config['sandbox'],"local")
if slowTests : config['name']+="_slow"
repositories = config['repositories'].split()
private_repositories = config['private_repositories'].split()
os.environ['LD_LIBRARY_PATH']='%(installPath)s/lib:/usr/local/lib' %config
os.environ['PATH']=':'.join([
	'%(installPath)s/bin'% config,
	os.path.expanduser('~/bin'), # for soxsucks
	os.environ['PATH'],
	])
os.environ['CLAM_PLUGIN_PATH']='%(installPath)s/lib/clam' % config
os.environ['LADSPA_PATH']='%(installPath)s/lib/ladspa' % config
os.environ['PKG_CONFIG_PATH']='%(installPath)s/lib/pkgconfig' % config

client = Client(config['name'])
client.brief_description = config['description']

clam = Task(
	project = Project('CLAM','<a href="http://clam-project.org">clam web</a>' ),
	client = client,
	task_name='Full Tests' if '--slow-tests' in sys.argv else 'Quick Tests',
	)

for repository in repositories :
	clam.add_sandbox(SvnSandbox(os.path.join(config['sandbox'], repository)))

for repository in config['private_repositories'].split() :
	clam.add_sandbox(SvnSandbox(os.path.join(config['sandbox'], repository)))

clam.set_check_for_new_commits(
	checking_cmd = 'false'%config,
	minutes_idle = 1500 if slowTests else 15,
)

clam.add_subtask('count lines of code', [
	{CMD:'echo %(sandbox)s/clam/CLAM'%config, STATS: lambda x: {'clam_loc': countLines(x) } },
	{CMD:'echo %(sandbox)s/clam/SMSTools'%config, STATS: lambda x: {'smstools_loc': countLines(x) } },
	{CMD:'echo %(sandbox)s/clam/NetworkEditor'%config, STATS: lambda x: {'networkeditor_loc': countLines(x) } },
] )
clam.add_deployment( [
	'cd %(sandbox)s/clam/CLAM'%config,
	'rm -rf %(installPath)s/*'%config,
	'mkdir -p %(installPath)s'%config,
	'scons configure prefix=%(installPath)s xmlbackend=both %(extraLibOptions)s'%config,
	'scons',
	'scons install',
	'mkdir -p %(installPath)s/bin'%config,
] )

clam.add_subtask('Unit Tests', [
	'cd %(sandbox)s/clam/CLAM/test'%config,
	'scons test_data_path=%(sandbox)s/clam-test-data clam_prefix=%(installPath)s %(extraAppOptions)s'%config, # TODO: test_data_path and release
	'cd UnitTests',
	{INFO : lambda x:startTimer() },
	'./UnitTests',
	{STATS : lambda x:{'exectime_unittests' : ellapsedTime()} },
] )
clam.add_subtask('Functional Tests', [
	'cd %(sandbox)s/clam/CLAM/test'%config,
	'scons test_data_path=%(sandbox)s/clam-test-data clam_prefix=%(installPath)s'%config, # TODO: test_data_path and release
	'cd FunctionalTests',
	{INFO : lambda x:startTimer() },
	'./FunctionalTests',
	{STATS : lambda x: {'exectime_functests' : ellapsedTime()} },
] )
clam.add_subtask('CLAM Examples', [
	'cd %(sandbox)s/clam/CLAM/examples'%config,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%config,
] )

clam.add_subtask('CLAM Plugins', [
	'cd %(sandbox)s/clam/CLAM/plugins/spacialization/spectral'%config,
	'scons prefix=%(installPath)s'%config,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/spacialization'%config,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s raytracing=disabled'%config,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/spacialization/ladspa/'%config,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%config,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/continuousExcitationSynthesizer'%config,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%config,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/GuitarEffects/'%config,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%config,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/MIDI/'%config,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%config,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/sndfile'%config,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%config,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/osc'%config,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%config,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/speech'%config,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%config,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/Filters'%config,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%config,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/resampling'%config,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%config,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/expression'%config,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%config,
	'scons install',
] )

clam.add_subtask('NetworkEditor installation', [
	'cd %(sandbox)s/clam/NetworkEditor'%config,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s %(extraAppOptions)s'%config,
	'scons install',
] )

clam.add_subtask('Back-to-back network tests', [
	'cd %(sandbox)s/clam/CLAM/plugins/spacialization'%config,
	'./back2back.py',
] )

clam.add_subtask('SMSTools installation', [
	'cd %(sandbox)s/clam/SMSTools'%config,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s'%config,
	'scons install',
	'%(sandbox)s/clam/CLAM/scons/sconstools/changeExampleDataPath.py %(installPath)s/share/smstools '%config,
] )

clam.add_subtask('vmqt4 compilation and examples', [
	'cd %(sandbox)s/clam/Annotator/vmqt'%config,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s release=1 double=1'%config,
	'scons examples',
] )
clam.add_subtask('Annotator installation', [
	'cd %(sandbox)s/clam/Annotator'%config,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s %(extraAppOptions)s '%config,
	'scons install',
] )

clam.add_subtask('Chordata installation', [
	'cd %(sandbox)s/clam/chordata'%config,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s %(extraAppOptions)s '%config,
	'scons install',
] )

clam.add_subtask('Voice2MIDI installation', [
	'cd %(sandbox)s/clam/Voice2MIDI'%config,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s %(extraAppOptions)s '%config,
	'scons install',
] )

clam.add_subtask('ipyclam compilation and test', [
	'cd %(sandbox)s/clam/ipyclam'%config,
	'scons'%config,
	'./runTests.py',
] )

"""
clam.add_subtask('pyCLAM build', [
	'cd %(sandbox)s/clam/pyclam'%config,
	 './generate_bindings.py && scons',
] )
clam.add_subtask('pyCLAM Unit Tests', [
	'cd %(sandbox)s/clam/pyclam'%config,
	'cd test',
	{INFO : lambda x:startTimer() },
	{CMD: './UnitTestsSuite.py'},
	{STATS : lambda x:{'exectime_pyclamtests' : ellapsedTime()} },
])

clam.add_subtask('Padova Speech SMS (external repository)', [
	'cd %(sandbox)s/clam/padova-speech-sms/'%config,
	{CMD:'svn log -r BASE:HEAD', INFO: lambda x:x },
	{CMD: 'svn up', INFO: lambda x:x },
	'make',
] )
"""


if slowTests :
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
#		'clam/CLAM/plugins/spacialization',
		'clam/CLAM/plugins/speech',
		]
	for d in dirs :
		clam.add_subtask('Check Clam Networks Recursively in %s' % d, [
			'cd %(sandbox)s/clam/CLAM/scripts'%config,
			{CMD: './check_clam_networks_recursively.py -b %(sandbox)s/'%config + d },
		] )

forceRun = len(sys.argv)>1
print "force Run: ", forceRun

Runner( clam, 
	continuous = False,
	first_run_always = forceRun,
	remote_server_url = 'http://localhost/testfarm/server/testfarmservice',
#	local_base_dir='~/logs',
	config = config,
)

