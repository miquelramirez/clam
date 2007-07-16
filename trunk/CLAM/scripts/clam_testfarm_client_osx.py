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

localDefinitions = {
	'name': 'mac-10.4.8-ppc-desktop',
	'description': '<img src="http://clam.iua.upf.es/images/apple_icon.png"/>',
	'sandbox': '$HOME/clam',
	'installPath': '$HOME/local',
	'qt3dir':'',
	'qt4dir':'/usr/local/Trolltech/Qt-4.2.2/',
}

client = Client(localDefinitions['name'])
client.brief_description = localDefinitions['description']
	

clam = Task(
	project = Project("CLAM"), 
	client = client, 
	task_name="with svn update" 
	)
clam.set_check_for_new_commits( 
		checking_cmd="cd %(sandbox)s && svn status -u | grep \*"%localDefinitions,
		minutes_idle=15
)

clam.add_subtask( "List of new commits", [
	"cd %(sandbox)s"%localDefinitions,
	{CMD:"svn log -r BASE:HEAD", INFO: lambda x:x },
	{CMD: "svn up", INFO: lambda x:x },
	] )

clam.add_subtask("count lines of code", [
	{CMD:"echo %(sandbox)s/CLAM"%localDefinitions, STATS: lambda x: {"clam_loc": countLines(x) } },
	{CMD:"echo %(sandbox)s/SMSTools"%localDefinitions, STATS: lambda x: {"smstools_loc": countLines(x) } },
	{CMD:"echo %(sandbox)s/NetworkEditor"%localDefinitions, STATS: lambda x: {"networkeditor_loc": countLines(x) } },
] )
clam.add_deployment( [
	"cd %(sandbox)s/CLAM"%localDefinitions,
	"rm -rf %(installPath)s/*"%localDefinitions,
	"cd %(sandbox)s/CLAM/"%localDefinitions,
	"scons configure prefix=%(installPath)s"%localDefinitions,
	"scons",
	"scons install",
] )

clam.add_subtask("Unit Tests", [
	"cd %(sandbox)s/CLAM"%localDefinitions,
	"cd test",
	"scons test_data_path=%(sandbox)s/testdata clam_prefix=%(installPath)s"%localDefinitions, # TODO: test_data_path and release
	"cd UnitTests",
	{INFO : lambda x:startTimer() }, 
	{CMD: "./UnitTests", INFO: lambda x:x, STATUS_OK: lambda x:True},
	{STATS : lambda x:{'exectime_unittests' : ellapsedTime()} },
] )
clam.add_subtask("Functional Tests", [
	"cd %(sandbox)s/CLAM"%localDefinitions,
	"cd test",
	"scons test_data_path=%(sandbox)s/testdata clam_prefix=%(installPath)s"%localDefinitions, # TODO: test_data_path and release
	"cd FunctionalTests",
	{INFO : lambda x:startTimer() }, 
	{CMD:"./FunctionalTests", INFO: lambda x:x, STATUS_OK: lambda x:True},
	{STATS : lambda x: {'exectime_functests' : ellapsedTime()} },
] )
clam.add_subtask("CLAM Examples", [
	{CMD: "echo set QTDIR to qt4", INFO: set_qtdir_to_qt4},
	"cd %(sandbox)s/CLAM/examples"%localDefinitions,
	"scons clam_prefix=%(installPath)s"%localDefinitions,
] )

clam.add_subtask("SMSTools packaging", [
	{CMD: "echo setting QTDIR to qt3 path ", INFO: set_qtdir_to_qt3},
	"cd %(sandbox)s/SMSTools"%localDefinitions,
	"scons prefix=%(installPath)s clam_prefix=%(installPath)s"%localDefinitions,
	"%(sandbox)s/CLAM/scons/sconstools/changeExampleDataPath.py %(installPath)s/share/smstools "%localDefinitions,
	"rm -f *.dmg",
	"scons package",
	"ls *svn1* > /dev/null || scp *.dmg clamadm@www.iua.upf.edu:download/mac/svnsnapshots/",
	'ls *svn1* > /dev/null || slogin clamadm@www.iua.upf.edu scripts/regenerateDownloadDirsIndex.py',
] )


clam.add_subtask('vmqt4 compilation and examples', [
	{CMD: "echo setting QTDIR to qt4 path ", INFO: set_qtdir_to_qt4},
	"cd %(sandbox)s/Annotator/vmqt"%localDefinitions,
	'scons prefix=%(installPath)s clam_prefix=%(installPath)s release=1 double=1'%localDefinitions,
	'scons examples',
] )
clam.add_subtask("Annotator packaging", [
	{CMD: "echo setting QTDIR to qt4 path ", INFO: set_qtdir_to_qt4},
	"cd %(sandbox)s/Annotator"%localDefinitions,
	"scons clam_vmqt4_path=vmqt prefix=%(installPath)s clam_prefix=%(installPath)s"%localDefinitions,
	"rm -f *.dmg",
	"scons package",
	"ls *svn1* > /dev/null || scp *.dmg clamadm@www.iua.upf.edu:download/mac/svnsnapshots/",
	'ls *svn1* > /dev/null || slogin clamadm@www.iua.upf.edu scripts/regenerateDownloadDirsIndex.py',
] )

clam.add_subtask("NetworkEditor packaging", [
	{CMD: "echo setting QTDIR to qt4 path ", INFO: set_qtdir_to_qt4},
	"cd %(sandbox)s/NetworkEditor"%localDefinitions,
	"scons prefix=%(installPath)s clam_prefix=%(installPath)s"%localDefinitions,
	"%(sandbox)s/CLAM/scons/sconstools/changeExampleDataPath.py %(installPath)s/share/smstools "%localDefinitions,
	"rm -f *.dmg",
	"scons package",
	"ls *svn1* > /dev/null || scp *.dmg clamadm@www.iua.upf.edu:download/mac/svnsnapshots/",
	'ls *svn1* > /dev/null || slogin clamadm@www.iua.upf.edu scripts/regenerateDownloadDirsIndex.py',
] )

Runner( clam, 
	continuous = True,
	remote_server_url = 'http://efpc072.upf.es/testfarm_server'
#	local_base_dir='/tmp'
)

