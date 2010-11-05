from diff_audio_files import diff_files
import os, sys, string
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

def expectedArchName(base, extension='.wav') :
	suffix_arch = archSuffix()
	return base+'_expected_' + suffix_arch + extension

def expectedName(base, extension) :
	"""Returns the expected wav name.
	If an architecture specific output already exists, it will use it.
	"""
	expected = expectedArchName(base, extension)
	if os.access(expected,os.R_OK): return expected

	return base+'_expected'+extension

def badResultName(base, extension = '.wav') :
	return base+'_result'+extension

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

			extension = os.path.splitext(output)[-1]
			base = prefix(datapath, case, output)
			badResult = badResultName(base, extension)
			if not os.access(badResult, os.R_OK) : continue
			print "Accepting", badResult

			if archSpecific :
				os.rename(badResult, expectedArchName(base, extension))
			else :
				os.rename(badResult, expectedName(base, extension))
	if remainingCases :
		print "Warning: No such test cases:", ", ".join("'%s'"%case for case in remainingCases)

def removeIfExists(filename) :
	try: os.remove(filename)
	except: pass

def passB2BTests(datapath, back2BackCases) :
	failedCases = []
	for case, command, outputs in back2BackCases :
		phase("Test: %s Command: '%s'"%(case,command))
		for output in outputs :
			removeIfExists(output)
		try :
			commandError = subprocess.call(command, shell=True)
			if commandError :
				failedCases.append((case, ["Command failed with return code %i:\n'%s'"%(commandError,command)]))
				continue
		except OSError, e :
			failedCases.append((case, ["Unable to run command: '%s'"%(command)]))
			continue
		failures = []
		for output in outputs :

			extension = os.path.splitext(output)[-1]
			base = prefix(datapath, case, output)
			expected = expectedName(base, extension)
			diffbase = diffBaseName(base)
			difference = diff_files(expected, output, diffbase)
			#diffbase = diffbase+'.wav'
			diffbase = diffbase + extension

			if not difference:
				print "\033[32m Passed\033[0m"
				removeIfExists(diffbase)
				removeIfExists(diffbase+'.png')
				removeIfExists(badResultName(base,extension))
			else:
				print "\033[31m Failed\033[0m"
				os.system('cp %s %s' % (output, badResultName(base,extension)) )
				failures.append("Output '%s': %s"%(base, difference))
			removeIfExists(output)
		if failures :
			failedCases.append((case, failures))

	print "Summary:"
	print '\033[32m%i passed cases\033[0m'%(len(back2BackCases)-len(failedCases))

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

def runBack2BackProgram(datapath, argv, back2BackCases, help=help) :

	"--help" not in sys.argv or die(help, 0)

	architectureSpecific = "--arch" in argv
	if architectureSpecific : argv.remove("--arch")

	os.access( datapath, os.X_OK ) or die(
		"Datapath at '%s' not available. "%datapath +
		"Check the back 2 back script on information on how to obtain it.")

	availableCases = [case for case, command, outputs in back2BackCases]

	if "--list" in argv :
		
		for case in availableCases :
			print case
		sys.exit()

	if "--accept" in argv :
		cases = argv[argv.index("--accept")+1:]
		cases or die("Option --accept needs a set of cases to accept.\nAvailable cases:\n"+"\n".join(["\t"+case for case, command, outputs in back2BackCases]))
		unsupportedCases = set(cases).difference(set(availableCases))
		if unsupportedCases:
			die("The following specified cases are not available:\n" + _caseList(unsupportedCases) + "Try with:\n" + _caseList(availableCases))
		accept(datapath, back2BackCases, architectureSpecific, cases)
		sys.exit()

	if "--acceptall" in argv :
		print "Warning: Accepting any faling case"
		accept(datapath, back2BackCases, architectureSpecific)
		sys.exit()

	passB2BTests(datapath, back2BackCases) or die("Tests not passed")


### End of generic stuff
