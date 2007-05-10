#! /usr/bin/python
import os, sys, time
sys.path.append('%s/testfarm/src' % os.environ['HOME'])
from task import *
from project import Project
from client import Client
from runner import Runner
from commands import getoutput

def countLines( path ):
	print "loc foro path:", path
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
	os.environ['QTDIR']='/usr/local/Trolltech/Qt-4.2.2/'
	os.environ['PKG_CONFIG_PATH']=os.environ['QTDIR']+'lib/pkgconfig'
def unset_qtdir(x) :
	os.environ['QTDIR']=""

client = Client("mac-10.4.8-ppc-desktop")
client.brief_description = '<img src="http://clam.iua.upf.es/images/apple_icon.png"/>'
	

clam = Task(
	project = Project("CLAM"), 
	client = client, 
	task_name="with svn update" 
	)
clam.set_check_for_new_commits( 
		checking_cmd="cd $HOME && svn status -u clam | grep \*",
		minutes_idle=5
)

clam.add_subtask( "List of new commits", [
	"cd $HOME/clam",
	{CMD:"svn log -r BASE:HEAD", INFO: lambda x:x },
	{CMD: "svn up", INFO: lambda x:x },
	] )

clam.add_subtask("count lines of code", [
	{CMD:"echo $HOME/clam/CLAM", STATS: lambda x: {"clam_loc": countLines(x) } }
] )
clam.add_deployment( [
	{CMD: "unset QTDIR", INFO: unset_qtdir},
	"cd $HOME/clam/CLAM",
	"rm -rf $HOME/local/*",
	"cd $HOME/clam/CLAM/",
	"scons configure prefix=$HOME/local",
	"scons",
	"scons install",
] )

clam.add_subtask("Unit Tests (with scons)", [
	{CMD: "echo unset QTDIR ", INFO: unset_qtdir},
	"cd $HOME/clam/CLAM",
	"cd test",
	"scons test_data_path=$HOME/clam/testdata clam_sconstools=$HOME/clam/CLAM/scons/sconstools install_prefix=$HOME/local clam_prefix=$HOME/local", # TODO: test_data_path and release
	"cd UnitTests",
	{INFO : lambda x:startTimer() }, 
	{CMD: "./UnitTests", INFO: lambda x:x, STATUS_OK: lambda x:True},
	{STATS : lambda x:{'exectime_unittests' : ellapsedTime()} },
] )
clam.add_subtask("Functional Tests (with scons)", [
	{CMD: "echo unset QTDIR", INFO: unset_qtdir},
	"cd $HOME/clam/CLAM",
	"cd test",
	"scons test_data_path=$HOME/clam/testdata clam_sconstools=$HOME/clam/CLAM/scons/sconstools install_prefix=$HOME/local clam_prefix=$HOME/local", # TODO: test_data_path and release
	"cd FunctionalTests",
	{INFO : lambda x:startTimer() }, 
	{CMD:"./FunctionalTests", INFO: lambda x:x, STATUS_OK: lambda x:True},
	{STATS : lambda x: {'exectime_functests' : ellapsedTime()} },
] )
clam.add_subtask("CLAM Examples (with scons)", [
	{CMD: "echo set QTDIR to qt4", INFO: set_qtdir_to_qt4},
	"cd $HOME/clam/CLAM/examples",
	"scons clam_prefix=$HOME/local",
] )

clam.add_subtask("SMSTools packaging", [
	{CMD: "echo unset  QTDIR  ", INFO: unset_qtdir},
	"cd $HOME/clam/SMSTools",
	"scons clam_sconstools=$HOME/clam/CLAM/scons/sconstools install_prefix=$HOME/local clam_prefix=$HOME/local",
	"$HOME/clam/CLAM/scons/sconstools/changeExampleDataPath.py $HOME/local/share/smstools ",
	"rm -f *.dmg",
	"scons package",
	"scp *.dmg clamadm@www.iua.upf.edu:download/mac/svnsnapshots/"
] )


clam.add_subtask('vmqt4 compilation and examples', [
	{CMD: "echo setting QTDIR to qt4 path ", INFO: set_qtdir_to_qt4},
	"cd $HOME/clam/Annotator/vmqt",
	'scons clam_sconstools=$HOME/clam/CLAM/scons/sconstools install_prefix=$HOME/local clam_prefix=$HOME/local release=1 double=1',
	'scons examples',
] )
clam.add_subtask("Annotator packaging", [
	{CMD: "echo setting QTDIR to qt4 path ", INFO: set_qtdir_to_qt4},
	"cd $HOME/clam/Annotator",
	"scons clam_vmqt4_path=vmqt clam_sconstools=$HOME/clam/CLAM/scons/sconstools install_prefix=$HOME/local clam_prefix=$HOME/local",
	"rm -f *.dmg",
	"scons package",
	"scp *.dmg clamadm@www.iua.upf.edu:download/mac/svnsnapshots/"
] )

clam.add_subtask("NetworkEditor packaging", [
	{CMD: "echo setting QTDIR to qt4 path ", INFO: set_qtdir_to_qt4},
	"cd $HOME/clam/NetworkEditor",
	"scons install_prefix=$HOME/local clam_prefix=$HOME/local",
	"$HOME/clam/CLAM/scons/sconstools/changeExampleDataPath.py $HOME/local/share/smstools ",
	"rm -f *.dmg",
	"scons package",
	"scp *.dmg clamadm@www.iua.upf.edu:download/mac/svnsnapshots/"
] )

Runner( clam, 
	continuous = True,
	remote_server_url = 'http://efpc072.upf.es/testfarm_server'
#	local_base_dir='/tmp'
)

