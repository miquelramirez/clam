#! /usr/bin/python

# to install as a cron script, use this line in crontab
# 0,5,10,15,20,25,30,35,40,45,50,55 * * * *    (cd /home/parumi/testfarm_sandboxes/clam/CLAM/scripts && svn up clam_testfarm_client.py && ./runOnce.py lock ./clam_testfarm_client.py) 2>&1 | cat > /tmp/err_testfarm_clam


import os, sys, time
sys.path.append('%s/testfarm/v2' % os.environ['HOME'])
from testfarm.v1.task import *
from testfarm.v1.project import Project
from testfarm.v1.client import Client
from testfarm.v1.runner import Runner
from testfarm.utils import loadDictFile
from testfarm.v1.mailreporter import MailReporter
from testfarm.v1.ircreporter import IrcReporter
from testfarm.v1.SvnSandbox import SvnSandbox
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

localDefinitions = dict(
	description= '<img src="http://clam-project.org/images/linux_icon.png"/> <img src="http://clam-project.org/images/ubuntu_icon.png"/>',
	repositories = "clam clam-test-data", # "acustica clam/padova-speech-sms",
	private_repositories = "",
	sandbox= os.path.expanduser('~/'),
	extraLibOptions = 'release=0',
	extraAppOptions = '',
)
try :
	localDefinitions.update(loadDictFile(os.path.expanduser('~/.config/testfarmrc')))
	localDefinitions['name'] # ensure that name is defined
	localDefinitions['description']
except :
	print >> sys.stderr, "ERROR: You should create ~/.config/testfarmrc with at least the name and description attributes of your client"
	raise

localDefinitions['installPath'] = os.path.join(localDefinitions['sandbox'],"local")
if slowTests : localDefinitions['name']+="_slow"
repositories = localDefinitions['repositories'].split()
private_repositories = localDefinitions['private_repositories'].split()
os.environ['LD_LIBRARY_PATH']='%(installPath)s/lib:/usr/local/lib' %localDefinitions
os.environ['PATH']=':'.join([
	'%(installPath)s/bin'% localDefinitions,
	os.path.expanduser('~/bin'), # for soxsucks
	os.environ['PATH'],
	])
os.environ['CLAM_PLUGIN_PATH']='%(installPath)s/lib/clam' % localDefinitions
os.environ['LADSPA_PATH']='%(installPath)s/lib/ladspa' % localDefinitions
os.environ['PKG_CONFIG_PATH']='%(installPath)s/lib/pkgconfig' % localDefinitions

client = Client(localDefinitions['name'])
client.brief_description = localDefinitions['description']

clam = Task(
	project = Project('CLAM','<a href="http://clam-project.org">clam web</a>' ),
	client = client,
	task_name='Full Tests' if '--slow-tests' in sys.argv else 'Quick Tests',
	)

for repository in repositories :
	clam.add_sandbox(SvnSandbox(os.path.join(localDefinitions['sandbox'], repository)))

for repository in localDefinitions['private_repositories'].split() :
	clam.add_sandbox(SvnSandbox(os.path.join(localDefinitions['sandbox'], repository)))

clam.set_check_for_new_commits(
	checking_cmd = 'false'%localDefinitions,
	minutes_idle = 1500 if slowTests else 15,
)

clam.add_subtask('count lines of code', [
	{CMD:'echo %(sandbox)s/clam/CLAM'%localDefinitions, STATS: lambda x: {'clam_loc': countLines(x) } },
	{CMD:'echo %(sandbox)s/clam/SMSTools'%localDefinitions, STATS: lambda x: {'smstools_loc': countLines(x) } },
	{CMD:'echo %(sandbox)s/clam/NetworkEditor'%localDefinitions, STATS: lambda x: {'networkeditor_loc': countLines(x) } },
] )
clam.add_deployment( [
	'cd %(sandbox)s/clam/CLAM'%localDefinitions,
	'rm -rf %(installPath)s/*'%localDefinitions,
	'mkdir -p %(installPath)s'%localDefinitions,
	'scons configure prefix=%(installPath)s xmlbackend=both %(extraLibOptions)s'%localDefinitions,
	'scons',
	'scons install',
	'mkdir -p %(installPath)s/bin'%localDefinitions,
] )

clam.add_subtask('Unit Tests', [
	'cd %(sandbox)s/clam/CLAM/test'%localDefinitions,
	'scons test_data_path=%(sandbox)s/clam-test-data clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions, # TODO: test_data_path and release
	'cd UnitTests',
	{INFO : lambda x:startTimer() },
	'./UnitTests',
	{STATS : lambda x:{'exectime_unittests' : ellapsedTime()} },
] )
clam.add_subtask('Functional Tests', [
	'cd %(sandbox)s/clam/CLAM/test'%localDefinitions,
	'scons test_data_path=%(sandbox)s/clam-test-data clam_prefix=%(installPath)s'%localDefinitions, # TODO: test_data_path and release
	'cd FunctionalTests',
	{INFO : lambda x:startTimer() },
	'./FunctionalTests',
	{STATS : lambda x: {'exectime_functests' : ellapsedTime()} },
] )
clam.add_subtask('CLAM Examples', [
	'cd %(sandbox)s/clam/CLAM/examples'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s'%localDefinitions,
] )

clam.add_subtask('CLAM Plugins', [
	'cd %(sandbox)s/clam/CLAM/plugins/spacialization/spectral'%localDefinitions,
	'scons prefix=%(installPath)s'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/spacialization'%localDefinitions,
	'scons clam_prefix=%(installPath)s %(extraAppOptions)s raytracing=disabled'%localDefinitions,
	'scons install',

	'cd %(sandbox)s/clam/CLAM/plugins/spacialization/ladspa/'%localDefinitions,
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

clam.add_subtask('ipyclam compilation and test', [
	'cd %(sandbox)s/clam/ipyclam'%localDefinitions,
	'scons'%localDefinitions,
	'./runTests.py',
] )

"""
clam.add_subtask('pyCLAM build', [
	'cd %(sandbox)s/clam/pyclam'%localDefinitions,
	 './generate_bindings.py && scons',
] )
clam.add_subtask('pyCLAM Unit Tests', [
	'cd %(sandbox)s/clam/pyclam'%localDefinitions,
	'cd test',
	{INFO : lambda x:startTimer() },
	{CMD: './UnitTestsSuite.py'},
	{STATS : lambda x:{'exectime_pyclamtests' : ellapsedTime()} },
])

clam.add_subtask('Padova Speech SMS (external repository)', [
	'cd %(sandbox)s/clam/padova-speech-sms/'%localDefinitions,
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
			'cd %(sandbox)s/clam/CLAM/scripts'%localDefinitions,
			{CMD: './check_clam_networks_recursively.py -b %(sandbox)s/'%localDefinitions + d },
		] )

forceRun = len(sys.argv)>1
print "force Run: ", forceRun

extra_listeners = []

if 'mail_report' in localDefinitions :
	mail_report_args = localDefinitions['mail_report'].__dict__
	del mail_report_args["__module__"]
	del mail_report_args["__doc__"]
	extra_listeners.append(
		MailReporter(
			testfarm_page="http://clam-project.org/testfarm.html",
			**mail_report_args))

if 'irc_report' in localDefinitions :
	irc_report_args = localDefinitions['irc_report'].__dict__
	del irc_report_args["__module__"]
	del irc_report_args["__doc__"]
	extra_listeners.append(
		IrcReporter(
			testfarm_page="http://clam-project.org/testfarm.html",
			**irc_report_args))

Runner( clam, 
	continuous = False,
	first_run_always = forceRun,
	remote_server_url = 'http://localhost/testfarm/server/testfarmservice',
#	local_base_dir='~/logs',
	extra_listeners = extra_listeners,
)

