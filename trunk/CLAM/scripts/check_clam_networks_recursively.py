#!/usr/bin/python

from diff_audio_files import diff_files
import os, sys, string,  glob
import cStringIO
import subprocess

def run(command) :
	print '\033[32m:: ', command, '\033[0m'
	errorCode = os.system(command)
	if errorCode :
		print "\n\nThe following command failed:"
		print '\033[31m', command, '\033[0m'
		sys.exit()
	return not errorCode

def norun(command) :
	print '\033[31mXX ', command, '\033[0m'

def phase(msg) :
	print '\033[33m== ', msg, '\033[0m'

def die(message, errorcode=-1) :
	print >> sys.stderr, message
	sys.exit(errorcode)


def archSuffix() :
	return string.strip(os.popen('uname -m').read())

def expectedArchName(base) :
	suffix_arch = archSuffix()
	return base+'_expected_'+suffix_arch+'.wav'

def expectedName(base, suffix_arch = archSuffix() ) :
	"""Returns the expected wav name.
	If an architecture specific output already exists, it will use it.
	"""
	expected = expectedArchName(base)
	if os.access(expected,os.R_OK): return expected
	return base+'_expected.wav'

def badResultName(base) :
	return base+'_result.wav'

def diffBaseName(base) :
	return base+'_diff'

def prefix(datapath, case, output) :
	outputBasename = os.path.splitext(os.path.basename(output))[0]
	return os.path.join(datapath, case + '_' + outputBasename )

def accept(datapath, back2BackCases, archSpecific=False, cases=[]) :
	remainingCases = cases[:]
	for case, command, outputs in back2BackCases :
		if cases and case not in cases : continue
		if cases : remainingCases.remove(case)
		for output in outputs :
			base = prefix(datapath, case, output)
			badResult = badResultName(base)
			if not os.access(badResult, os.R_OK) : continue
			print "Accepting", badResult
			if archSpecific :
				os.rename(badResult, expectedArchName(base))
			else :
				os.rename(badResult, expectedName(base))
	if remainingCases :
		print "Warning: No such test cases:", ", ".join("'%s'"%case for case in remainingCases)

def removeIfExists(filename) :
	try: os.remove(filename)
	except: pass

def passCheckClamnetworks(datapath, clamnetworks) :
	failedCases = []
	myDirectory=os.getcwd() # save current directory
	
	for case in clamnetworks :
		
		newDirectory=os.path.dirname(case);
		os.chdir(newDirectory);	#change to the  network directory 
		
		command="CheckClamNetwork %s"%(case)
		phase("Test: %s"%(case))
		output = cStringIO.StringIO()

		try :
			process = subprocess.Popen(command, shell=True, 
														stdout=subprocess.PIPE, 
														stderr=subprocess.STDOUT, 
														close_fds = True)
			
			stdout_text = process.communicate()[0];
			output = process.returncode;
			
			if output:
				print stdout_text
				failedCases.append((case, ["Command: %s"%(command)]))
				continue

		except OSError, e :
			failedCases.append((case, ["Unable to run command: '%s'"%(command)]))
			continue
	os.chdir(myDirectory);
	
	print "Summary:"
	print '\033[32m%i passed cases\033[0m'%(len(clamnetworks)-len(failedCases))

	if not failedCases : return True

	print '\033[31m%i failed cases!\033[0m'%len(failedCases)
	for case, msgs in failedCases :
		for msg in msgs :
			print " %s"%msg
	return False

help ="""
To run the tests call this script without parameters.
	./check_clam_networks_recursively.py directory

To know which are the available cases:
	./check_clam_networks_recursively.py directory --list

To know which are the available cases:
	./check_clam_networks_recursively.py directory subdirectory_excluded1 subdirectory_excluded2 ...

To know which are the available cases:
	./check_clam_networks_recursively.py directory subdirectory_excluded1 subdirectory_excluded2 ... --list
"""

def _caseList(cases) :
	return "".join(["\t"+case+"\n" for case in cases])

def runCheckClamnetworksProgram(datapath, argv, clamnetworks) :

	availableCases = clamnetworks

	if "--list" in argv :
		for case in availableCases :
			print case
		sys.exit()

	passCheckClamnetworks(datapath, clamnetworks) or die("Tests not passed")

def recursiveDirs(root) :
	return filter( 
		(lambda a : a.rfind( ".svn")==-1 ), 
		[ a[0] for a in os.walk(root)]  )

######### Main #########

if len(sys.argv) ==1:
	print help
	exit()

data_path= sys.argv[1]
subdirectories_excluded=[]

if len(sys.argv) > 2:
	for i in range(2, len(sys.argv)):
		temp = sys.argv[i]
		if  temp != "--list":
			subdirectories_excluded.insert(0, os.path.join(data_path, temp))

os.access( data_path, os.X_OK ) or die(
    "Datapath at '%s' not available. "%data_path +
    "Check the check clam networks recursively script on information on how to obtain it.")

clam_networks=set();

for dir in recursiveDirs( data_path ):
	if dir not in subdirectories_excluded:
		for net in glob.glob( os.path.join(dir, "*.clamnetwork")):
			clam_networks.add(net) ;

#print clam_networks
#print len(clam_networks)

runCheckClamnetworksProgram(data_path, sys.argv, clam_networks)

