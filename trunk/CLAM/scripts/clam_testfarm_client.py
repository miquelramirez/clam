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
os.environ['CLAM_TEST_DATA']='%s/clam/testdata' % HOME

def erase_QTDIR(x) :
	os.environ['QTDIR']=""

client = Client("linux_ubuntu_feisty")
client.brief_description = '<img src="http://clam.iua.upf.es/images/linux_icon.png"/> <img src="http://clam.iua.upf.es/images/ubuntu_icon.png"/>'
	

clam = Task(
	project = Project("CLAM"), 
	client = client, 
	task_name="with svn update" 
	)
clam.set_check_for_new_commits( 
		checking_cmd="cd $HOME/clam && svn status -u | grep \*",
		minutes_idle=15
)

clam.add_subtask( "List of new commits", [
	"cd $HOME/clam",
	{CMD:"svn log -r BASE:HEAD", INFO: lambda x:x },
	{CMD: "svn up", INFO: lambda x:x },
	] )

clam.add_subtask("count lines of code", [
	{CMD:"echo $HOME/clam/CLAM", STATS: lambda x: {"clam_loc": countLines(x) } },
	{CMD:"echo $HOME/clam/SMSTools", STATS: lambda x: {"smstools_loc": countLines(x) } },
	{CMD:"echo $HOME/clam/NetworkEditor", STATS: lambda x: {"networkeditor_loc": countLines(x) } }
] )
clam.add_deployment( [
#	{CMD: "echo setting QTDIR to qt3 path ", INFO: erase_QTDIR},
	"cd $HOME/clam/CLAM",
	"rm -rf $HOME/local/*",
	"cd $HOME/clam/CLAM/",
	"scons configure prefix=$HOME/local",
	"scons",
	"scons install",
] )
clam.add_subtask("Unit Tests", [
#	{CMD: "echo setting QTDIR to qt3 path ", INFO: erase_QTDIR},
	"cd $HOME/clam/CLAM",
	"cd test",
	"scons test_data_path=$HOME/clam/testdata  clam_prefix=$HOME/local", # TODO: test_data_path and release
	"cd UnitTests",
	{INFO : lambda x:startTimer() }, 
	{CMD: "./UnitTests", INFO: lambda x:x},
	{STATS : lambda x:{'exectime_unittests' : ellapsedTime()} },
] )
clam.add_subtask("Functional Tests", [
#	{CMD: "echo setting QTDIR to qt3 path ", INFO: erase_QTDIR},
	"cd $HOME/clam/CLAM",
	"cd test",
	"scons test_data_path=$HOME/clam/testdata  clam_prefix=$HOME/local", # TODO: test_data_path and release
	"cd FunctionalTests",
	{INFO : lambda x:startTimer() }, 
	{CMD:"./FunctionalTests", INFO: lambda x:x},
	{STATS : lambda x: {'exectime_functests' : ellapsedTime()} },
] )
clam.add_subtask("CLAM Examples", [
#	{CMD: "echo setting QTDIR to qt3 path ", INFO: erase_QTDIR},
	"cd $HOME/clam/CLAM/examples",
	"scons clam_prefix=$HOME/local",
] )
clam.add_subtask("SMSTools installation", [
#	{CMD: "echo setting QTDIR to qt3 path ", INFO: erase_QTDIR},
	"cd $HOME/clam/SMSTools",
	"scons prefix=$HOME/local clam_prefix=$HOME/local",
	"scons install",
	"$HOME/clam/CLAM/scons/sconstools/changeExampleDataPath.py $HOME/local/share/smstools ",
] )


clam.add_subtask('vmqt4 compilation and examples', [
	"cd $HOME/clam/Annotator/vmqt",
	'scons  prefix=$HOME/local clam_prefix=$HOME/local release=1 double=1',
	'scons examples',
] )
clam.add_subtask("Annotator installation", [
	"cd $HOME/clam/Annotator",
	"scons clam_vmqt4_path=vmqt  prefix=$HOME/local clam_prefix=$HOME/local",
	"scons install",
] )

clam.add_subtask("NetworkEditor installation", [
#	{CMD: "echo setting QTDIR to qt3 path ", INFO: erase_QTDIR},
	"cd $HOME/clam/NetworkEditor",
	"scons prefix=$HOME/local clam_prefix=$HOME/local",
	"$HOME/clam/CLAM/scons/sconstools/changeExampleDataPath.py $HOME/local/share/smstools ",
] )

Runner( clam, 
	continuous = True,
	remote_server_url = 'http://efpc072.upf.es/testfarm_server'
#	local_base_dir='/tmp'
)

