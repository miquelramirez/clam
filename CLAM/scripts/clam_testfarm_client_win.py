#! /usr/bin/python
#


# IMPORTANT
# vcvars32.bat must be run in windows before this script
# to get testfarm:
# $ svn co https://testfarm.svn.sourceforge.net/svnroot/testfarm/trunk testfarm 

import sys
sys.path.append('D:\\CLAM-sandboxes\\testfarm\\src') #XXX write your path to testfarm/src here.


from task import *
from project import Project
from client import Client
from runner import Runner
import os, time

start_time = -1
def start_timer(output):
	global start_time
	start_time = time.time()
def exectime_unittests(output):
	global start_time
	return {'exectime_unittests' : time.time() - start_time}
def exectime_functests(output):
	global start_time
	return {'exectime_functests' : time.time() - start_time}

def force_ok(text):
	return True

def filter_cvs_update( text ):
	dont_start_interr = lambda line : not line or not line[0]=='?'
	result = filter(dont_start_interr, text.split('\n') )	
	return '\n'.join(result)

def filter_svn_update( text ):
	return text

#update_command = { CMD: "cvs -q up -Pd", INFO: filter_cvs_update }
update_command = { CMD: "svn up --accept postpone", INFO: filter_svn_update }

def set_qtdir_to_qt4_fn(x) : 
	os.environ['QTDIR']='D:\\CLAM-sandboxes\\Qt\\4.2.1'
	return 'QTDIR set to ' + os.environ['QTDIR']
set_qtdir_to_qt4 = {CMD: "echo seting QTDIR to qt4 path ", INFO: set_qtdir_to_qt4_fn}

def set_qtdir_to_qt3_fn(x) : 
	os.environ['QTDIR']='D:\\CLAM-sandboxes\\Qt\\3.3.4'
	return 'QTDIR set to ' + os.environ['QTDIR']
set_qtdir_to_qt3 = {CMD: "echo seting QTDIR to qt3 path ", INFO: set_qtdir_to_qt3_fn}

os.environ['EXTERNALDLLDIR']='D:\\CLAM-sandboxes\\CLAM\\externals\dlls'
os.environ['CLAM_TEST_DATA']='D:\\CLAM-sandboxes\\CLAM\\test'

cd_clam = 'cd D:\\CLAM-sandboxes'
install_path = 'D:\\CLAM-sandboxes\\CLAM-bin'
sconstools_path = 'D:\\CLAM-sandboxes\\CLAM\\scons\\sconstools'

windows = Client("windows_xp-msvc")
windows.brief_description = '<img src="http://clam-project.org/images/windows_icon.png"/>'

clam = Task(
	project = Project("CLAM"), 
	client = windows, 
	task_name="with svn update" 
	)

clam.set_check_for_new_commits(
	checking_cmd = "cd D:\\CLAM-sandboxes\\ && svn status -u | grep \\*",
	minutes_idle=5
)

clam.add_deployment([
	cd_clam,
	{ CMD: "svn up --accept postpone", INFO: filter_svn_update },
	cd_clam,
	"rm -rf CLAM-bin\\*",
	'cd CLAM',
	set_qtdir_to_qt3,
	'scons configure'+
	' prefix=%s'%install_path +
	' sandbox_path=D:\\CLAM-sandboxes' +
#	' qt_includes=f:\\clam-external-libs\\qt\\include' +
#	' qt_libs=f:\\clam-external-libs\\qt\\lib' +
	' release=1 double=1 sandbox=1 checks=1 release_asserts=0 optimize_and_lose_precision=0 with_jack=0 with_fftw3=0 with_fftw=1 with_nr_fft=1 with_sndfile=1 with_oggvorbis=1 with_mad=1 with_id3=1 with_portaudio=1 with_portmidi=1',
	'scons',
	'scons install',
] )
'''
clam.add_subtask('unit tests', [
	cd_clam,
	'cd CLAM\\tests',
	set_qtdir_to_qt3,
	'scons unit_tests'+
	' clam_prefix=%s'%install_path +
	' cppunit_prefix=f:\\clam-external-libs\\cppunit' +
	' test_data_path=f:\\clam-sandboxes\\CLAM-TestData' +
	' release=1 double=1',
	'cd unit_tests',
	{CMD: 'UnitTests.exe', INFO: lambda x:x, STATUS_OK: force_ok },
	])

clam.add_subtask('functional test', [
	cd_clam,
	'cd CLAM\\tests',
	set_qtdir_to_qt3,
	'scons functional_tests' +
	' clam_prefix=%s'%install_path +
	' cppunit_prefix=f:\\clam-external-libs\\cppunit' +
	' test_data_path=f:\\clam-sandboxes\\CLAM-TestData' +
	' release=1 double=1',
	'cd functional_tests',
	{CMD: 'FunctionalTests.exe', INFO: lambda x:x, STATUS_OK: force_ok },
	])

clam.add_subtask('clam examples compilation', [
	cd_clam,
	'cd CLAM\\examples',
	set_qtdir_to_qt3,
	'scons' +
	' clam_prefix=%s'%install_path +
	' release=1 double=1',
	])

'''

clam.add_subtask('smstools compilation', [
	cd_clam,
	'cd SMSTools',
	set_qtdir_to_qt3,
	'scons' +
	' clam_prefix=%s'%install_path +
	' prefix=%s'%install_path +
	' release=1 double=1'
] )
'''
clam.add_subtask('smstools package', [
	cd_clam,
	'cd SMSTools',
	'scons package'
])
'''

clam.add_subtask('vmqt compilation and examples', [
	cd_clam,
	'cd Annotator\\vmqt',
	set_qtdir_to_qt4,
	'scons ' +
	' clam_prefix=%s'%install_path +
	' prefix=%s'%install_path +
	' release=1 double=1',
	'scons examples'
] )


clam.add_subtask('annotator compilation', [
	cd_clam,
	'cd Annotator',
	set_qtdir_to_qt4,
	'scons' +
	' clam_prefix=%s'%install_path +
	' prefix=%s'%install_path +
	' release=1 double=1'
] )


'''
clam.add_subtask('annotator install', [
	cd_clam,
	'cd Annotator',
	'scons install'
] )
clam.add_subtask('annotator package', [
	cd_clam,
	'cd Annotator\\SimacServicesClient',
	'buildExeFromPython.py',
	'cd ..',
	'scons package'
] )
'''

clam.add_subtask('network editor compilation', [
	cd_clam,
	'cd NetworkEditor',
	set_qtdir_to_qt4,
	'scons' +
	' clam_prefix=%s'%install_path +
	' prefix=%s'%install_path +
	' release=1 double=1'
] )

'''
clam.add_subtask('neteditor install', [
	cd_clam,
	'cd NetworkEditor',
	'scons install'
])
'''
clam.add_subtask('neteditor package', [
	cd_clam,
	'cd NetworkEditor',
	'rm -f *.exe',
	'scons package',
	'scp *.exe clamadm@clam-project.org:clam-project.org/download/win/svnsnapshots/',
	'slogin clamadm@clam-project.org clam-project.org/scripts/regenerateDownloadDirsIndex.py'

])



clam.add_subtask('VstPrototyper', [
	cd_clam,
	'cd NetworkEditor\\src\\\\vstprototyper',
	'scons' +
	' prefix=%s'%install_path +
	' clam_sconstools=%s'%sconstools_path +
	' vstsdk_path=f:\\clam-external-libs\\\\vstsdk2.3'
] )


Runner( clam, 
	continuous = True,
	remote_server_url = 'http://ocata48123.upf.es/testfarm_server'
#	local_base_dir='f:\\tmp'
)

