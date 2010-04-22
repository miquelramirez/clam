#!/usr/bin/python

from diff_audio_files import diff_files
import os, sys, string,  glob
import cStringIO
import subprocess
import getopt

RunFromLocalDir=0
RunFromCurrentDir=1
RunFromBaseDir=2

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

def passCheckClamnetworks(datapath, clamnetworks, mode) :
	failedCases = []
	myDirectory=os.getcwd() # save current directory
	
	if(mode==RunFromBaseDir):
		os.chdir(datapath);	#change to the  path
	
	for case in clamnetworks :
		
		if mode==RunFromLocalDir :
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
To run the tests call this script with directory.
	./check_clam_networks_recursively.py directory

To know which are the available cases:
	./check_clam_networks_recursively.py [options]... directory

To know which are the available cases usiong blacklist:
	./check_clam_networks_recursively.py [options]... directory [-k subdirectory_excluded1 subdirectory_excluded2...]

By default the network will be run from the current directory. But you can change the run dir with following options:
	-b	--basedir	to run the networks found recursively from the given directory
	-c	--localdir	to run the networks found recursively from the directory of each network
Other options:
	-h	--help		
	-l	--list		list network into de directory
	

"""

def _caseList(cases) :
	return "".join(["\t"+case+"\n" for case in cases])

def runCheckClamnetworksProgram(datapath, clamnetworks, mode) :

	passCheckClamnetworks(datapath, clamnetworks, mode) or die("Tests not passed")

def recursiveDirs(root) :
	return filter( 
		(lambda a : a.rfind( ".svn")==-1 ), 
		[ a[0] for a in os.walk(root)]  )

######### Main #########

def main():
    # parse command line options

	showList=0;
	args =  sys.argv[1:]
	subdirectories_excluded=list()
	try:
		optlist1, args1 = getopt.getopt(args, "bchl",  ["basedir", "localdir", "help", "list"])
	except getopt.error, msg:
		print "[1] for help use --help"
		sys.exit(2)

	# process options
	mode = RunFromCurrentDir 
	for o, a in optlist1:
		if o in ("-c", "--localdir"):	# Run directory network = network directory 
			mode = RunFromLocalDir		
		if o in ("-b", "--basedir"):	# Run directory network = path 
			mode = RunFromBaseDir
		if o in ("-h", "--help"):
			print help
			sys.exit(0)
		if o in ("-l", "--list"):
			showList=1;

	if len(args1) ==0:
		print help
		exit(0)

	data_path= args1[0]
#	print "Path: ", data_path
	
	if len(args1) >1:
		try:
			optlist2, args2 = getopt.getopt(args1[1:], "k",  ["blacklist"])
		except getopt.error, msg:
			print "[2] for help use --help"
			sys.exit(2)
		
		for o, a in optlist2:
			if o in ("-k", "--blacklist"):
				for dir in args2:
					subdirectories_excluded.insert(0, os.path.join(data_path, dir))
			
#	print "Excluded: ", subdirectories_excluded

	os.access( data_path, os.X_OK ) or die(
		"Datapath at '%s' not available. "%data_path +
		"Check the check clam networks recursively script on information on how to obtain it.")

	clam_networks=set();

	print subdirectories_excluded

	for dir in recursiveDirs( data_path ):
		valid=1;
		for excluded in subdirectories_excluded:
			if dir.find(excluded) != -1 or excluded.find(dir) != -1:
				valid=0;
		if(valid==1):
			for net in glob.glob( os.path.join(dir, "*.clamnetwork")):
				clam_networks.add(net) ;

	if(showList==1):
		for case in clam_networks :
			print case
	else:
		runCheckClamnetworksProgram(data_path, clam_networks,  mode)

if __name__ == "__main__":
    main()




