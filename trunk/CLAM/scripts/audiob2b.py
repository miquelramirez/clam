from diff_audio_files import diff_files
import os, sys, string

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
	print message
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

def accept(datapath, back2back_files, archSpecific=False, cases=[]) :
	remainingCases = cases[:]
	for case, command, outputs in back2back_files :
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

def passB2BTests(datapath, back2back_files) :
	failedCases = []
	for case, command, outputs in back2back_files :
		phase("Test: %s Command: '%s'"%(case,command))
		caseFailed = False
		msgs = []
		for output in outputs :
			removeIfExists(output)
		os.system(command)
		for output in outputs :
			base = prefix(datapath, case, output)
			expected = expectedName(base)
			diffbase = diffBaseName(base)
			equals = diff_files(expected, output, diffbase)
			if equals:
				print "\033[32m Equals\033[0m"
				removeIfExists(diffbase+'.wav')
				removeIfExists(diffbase+'.wav.png')
				removeIfExists(badResultName(base))
			else:
				caseFailed = True
				print "\033[31m Non equals\033[0m"
				os.system('cp %s %s' % (output, badResultName(base)) )
				msgs.append("Output %s mismatch"%base)
			removeIfExists(output)
		if caseFailed :
			failedCases.append((case, msgs))

	print "Summary:"
	print '\033[32m%i passed cases\033[0m'%(len(back2back_files)-len(failedCases))

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

To accept any failing cases (USE IT WITH CARE) call:
	./back2back --acceptall

To accept some results but just for a given architecture,
due to floating point missmatches, use:
	./back2back --arch --accept case1 case2
"""

def runBack2BackProgram(datapath, argv, back2back_files, help=help) :

	"--help" not in sys.argv or die(help, 0)

	architectureSpecific = "--arch" in argv
	if architectureSpecific : argv.remove("--arch")

	os.access( datapath, os.X_OK ) or die("Datapath at '%s' not available"%datapath)

	if "--accept" in argv :
		cases = argv[argv.index("--accept")+1:]
		cases or die("Option --accept needs a set of cases to accept. Try")
		accept(datapath, back2back_files, architectureSpecific, cases)
		sys.exit()

	if "--acceptall" in argv :
		print "Warning: Accepting any faling case"
		accept(datapath, back2back_files, architectureSpecific)
		sys.exit()

	passB2BTests(datapath, back2back_files) or die("Tests not passed")


### End of generic stuff
