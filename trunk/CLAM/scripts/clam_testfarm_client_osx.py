#! /usr/bin/python
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

def set_qtdir_to_qt4(x) :
	os.environ['QTDIR']=localDefinitions['qt4dir']
	if os.environ['QTDIR'] and sys.platform=='darwin':
		os.environ['PKG_CONFIG_PATH']=os.environ['QTDIR']+'lib/pkgconfig'
def set_qtdir_to_qt3(x) :
	os.environ['QTDIR']=localDefinitions['qt3dir']

sandbox = os.path.expanduser('~/'),
localDefinitions = {
	'name': 'mac-10.4.8-intel-macbook',
	'description': '<img src="http://clam.iua.upf.es/images/apple_icon.png"/>',
	'sandbox': sandbox,
	'clamsrcroot': sandbox+"clam",
	'installPath': sandbox+"local",
	'qt3dir':'',
	'qt4dir':'/usr/local/Trolltech/Qt-4.2.2/',
	'packageWildcard':'*.dmg',
	'downloadPlatform':'mac',
	'extraLibOptions': '',
	'extraAppOptions': '',
}

client = Client(localDefinitions['name'])
client.brief_description = localDefinitions['description']
	

clam = Task(
	project = Project("CLAM"), 
	client = client, 
	task_name="with svn update" 
	)
clam.set_check_for_new_commits( 
		checking_cmd="cd %(clamsrcroot)s && svn status -u | grep \*"%localDefinitions,
		minutes_idle=15
)

clam.add_subtask( "List of new commits", [
	"cd %(clamsrcroot)s"%localDefinitions,
	{CMD:"svn log -r BASE:HEAD", INFO: lambda x:x },
	{CMD: "svn up", INFO: lambda x:x },
	] )

clam.add_subtask("count lines of code", [
	{CMD:"echo %(clamsrcroot)s/CLAM"%localDefinitions, STATS: lambda x: {"clam_loc": countLines(x) } },
	{CMD:"echo %(clamsrcroot)s/SMSTools"%localDefinitions, STATS: lambda x: {"smstools_loc": countLines(x) } },
	{CMD:"echo %(clamsrcroot)s/NetworkEditor"%localDefinitions, STATS: lambda x: {"networkeditor_loc": countLines(x) } },
] )
clam.add_deployment( [
	"cd %(clamsrcroot)s/CLAM"%localDefinitions,
	"rm -rf %(installPath)s/*"%localDefinitions,
	"cd %(clamsrcroot)s/CLAM/"%localDefinitions,
	"scons configure prefix=%(installPath)s %(extraLibOptions)s"%localDefinitions,
	"scons",
	"scons install",
] )

clam.add_subtask("Unit Tests", [
	"cd %(clamsrcroot)s/CLAM"%localDefinitions,
	"cd test",
	"scons test_data_path=%(clamsrcroot)s/testdata clam_prefix=%(installPath)s"%localDefinitions, # TODO: test_data_path and release
	"cd UnitTests",
	{INFO : lambda x:startTimer() }, 
	{CMD: "./UnitTests", INFO: lambda x:x, STATUS_OK: lambda x:True},
	{STATS : lambda x:{'exectime_unittests' : ellapsedTime()} },
] )
clam.add_subtask("Functional Tests", [
	"cd %(clamsrcroot)s/CLAM"%localDefinitions,
	"cd test",
	"scons test_data_path=%(clamsrcroot)s/testdata clam_prefix=%(installPath)s"%localDefinitions, # TODO: test_data_path and release
	"cd FunctionalTests",
	{INFO : lambda x:startTimer() }, 
	{CMD:"./FunctionalTests", INFO: lambda x:x, STATUS_OK: lambda x:True},
	{STATS : lambda x: {'exectime_functests' : ellapsedTime()} },
] )
clam.add_subtask("CLAM Examples", [
	{CMD: "echo set QTDIR to qt4", INFO: set_qtdir_to_qt4},
	"cd %(clamsrcroot)s/CLAM/examples"%localDefinitions,
	"scons clam_prefix=%(installPath)s"%localDefinitions,
] )
clam.add_subtask("CLAM Plugins compilation", [
	"cd %(clamsrcroot)s/CLAM/examples/PluginExample"%localDefinitions,
	"scons clam_prefix=%(installPath)s"%localDefinitions,
] )

clam.add_subtask("SMSTools packaging", [
	{CMD: "echo setting QTDIR to qt3 path ", INFO: set_qtdir_to_qt3},
	"cd %(clamsrcroot)s/SMSTools"%localDefinitions,
	"scons prefix=%(installPath)s clam_prefix=%(installPath)s"%localDefinitions,
	"%(clamsrcroot)s/CLAM/scons/sconstools/changeExampleDataPath.py %(installPath)s/share/smstools "%localDefinitions,
	"rm -f %(packageWildcard)s"%localDefinitions,
	"scons package",
	"ls *svn1* > /dev/null || scp %(packageWildcard)s clamadm@www.iua.upf.edu:download/%(downloadPlatform)s/svnsnapshots/"%localDefinitions,
	'ls *svn1* > /dev/null || slogin clamadm@www.iua.upf.edu scripts/regenerateDownloadDirsIndex.py',
] )
clam.add_subtask('vmqt4 compilation and examples', [
	{CMD: "echo setting QTDIR to qt4 path ", INFO: set_qtdir_to_qt4},
	"cd %(clamsrcroot)s/Annotator/vmqt"%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s release=1 double=1 %(extraAppOptions)s'%localDefinitions,
	'scons examples',
] )
clam.add_subtask("Annotator packaging", [
	{CMD: "echo setting QTDIR to qt4 path ", INFO: set_qtdir_to_qt4},
	"cd %(clamsrcroot)s/Annotator"%localDefinitions,
	"scons prefix=%(installPath)s clam_prefix=%(installPath)s %(extraAppOptions)s "%localDefinitions,
	"rm -f %(packageWildcard)s"%localDefinitions,
	"scons package",
	"ls *svn1* > /dev/null || scp %(packageWildcard)s clamadm@www.iua.upf.edu:download/%(downloadPlatform)s/svnsnapshots/"%localDefinitions,
	'ls *svn1* > /dev/null || slogin clamadm@www.iua.upf.edu scripts/regenerateDownloadDirsIndex.py',
] )

clam.add_subtask("NetworkEditor packaging", [
	{CMD: "echo setting QTDIR to qt4 path ", INFO: set_qtdir_to_qt4},
	"cd %(clamsrcroot)s/NetworkEditor"%localDefinitions,
	"scons prefix=%(installPath)s clam_prefix=%(installPath)s  %(extraAppOptions)s "%localDefinitions,
	"%(clamsrcroot)s/CLAM/scons/sconstools/changeExampleDataPath.py %(installPath)s/share/smstools "%localDefinitions,
	"rm -f %(packageWildcard)s"%localDefinitions,
	"scons package",
	"ls *svn1* > /dev/null || scp %(packageWildcard)s clamadm@www.iua.upf.edu:download/%(downloadPlatform)s/svnsnapshots/"%localDefinitions,
	'ls *svn1* > /dev/null || slogin clamadm@www.iua.upf.edu scripts/regenerateDownloadDirsIndex.py',
] )

Runner( clam, 
	continuous = True,
	remote_server_url = 'http://ocata48123.upf.es/testfarm_server',
#	local_base_dir='/tmp',
)

