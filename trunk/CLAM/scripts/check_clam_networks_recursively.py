#!/usr/bin/python

from diff_audio_files import diff_files
import os, sys, string,  glob
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
	for case in clamnetworks :
		command="CheckClamNetwork %s"%(case)
		phase("Test: %s Command: %s"%(case, command))
		try :
			process = subprocess.Popen(command, shell=True, 
														stdout=subprocess.PIPE, 
														stderr=subprocess.PIPE, 
														close_fds = True)
			
			stdout_text, stderr_text = process.communicate();
			output = process.returncode;
			
			if output:
				print >>sys.stderr, stderr_text
				print stdout_text
				failedCases.append((case, ["Command failed with return code %i:\n'%s'"%(output,command)]))
				continue

		except OSError, e :
			failedCases.append((case, ["Unable to run command: '%s'"%(command)]))
			continue

	print "Summary:"
	print '\033[32m%i passed cases\033[0m'%(len(clamnetworks)-len(failedCases))

	if not failedCases : return True

	print '\033[31m%i failed cases!\033[0m'%len(failedCases)
	for case, msgs in failedCases :
		print case, ":"
		for msg in msgs :
			print "\t%s"%msg
	return False

help ="""
To run the tests call this script without parameters.
	./back2back

Failed cases will generate *_result.wav and *_diff.wav
files for each missmatching output, containing the
obtained output and the difference with the expected one.

If some test fail but you want to accept the new results
just call:
	./back2back --accept case1 case2
where case1 and case2 are the cases to be accepted.

To know which are the available cases:
	./back2back --list

To accept any failing cases (USE IT WITH CARE) call:
	./back2back --acceptall

To accept some results but just for a given architecture,
due to floating point missmatches, use:
	./back2back --arch --accept case1 case2
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
	
data_path= sys.argv[1]

os.access( data_path, os.X_OK ) or die(
    "Datapath at '%s' not available. "%data_path +
    "Check the check clam networks recursively script on information on how to obtain it.")

clam_networks=set();

for dir in recursiveDirs( data_path ):
    for net in glob.glob( os.path.join(dir, "*.clamnetwork")):
        clam_networks.add(net) ;

#print clam_networks
#print len(clam_networks)

runCheckClamnetworksProgram(data_path, sys.argv, clam_networks)

