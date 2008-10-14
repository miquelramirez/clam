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

def accept(back2back_files, archSpecific=False, cases=[]) :
	remainingCases = cases[:]
	for case, command, files in back2back_files :
		if cases and case not in cases : continue
		if cases : remainingCases.remove(case)
		for base, result in files :
			badResult = badResultName(base)
			if not os.access(badResult, os.R_OK) : continue
			print "Accepting", badResult
			if archSpecific :
				os.rename(badResult, expectedArchName(base))
			else :
				os.rename(badResult, expectedName(base))
	if remainingCases :
		print "Warning: Inexistent test cases:", ", ".join("'%s'"%case for case in remainingCases)

def removeIfExists(filename) :
	try: os.remove(filename)
	except: pass

def passB2BTests(back2back_files) :
	failedCases = []
	for case, command, files in back2back_files :
		phase("Test: %s Command: '%s'"%(case,command))
		caseFailed = False
		msgs = []
		for base, result in files :
			removeIfExists(result)
		os.system(command)
		for base, result in files :
			expected = expectedName(base)
			diffbase = base+'_diff'
			equals = diff_files(expected, result, diffBaseName(base))
			if equals:
				print "\033[32m Equals\033[0m"
				removeIfExists(diffbase+'.wav')
				removeIfExists(diffbase+'.wav.png')
				removeIfExists(badResultName(base))
			else:
				caseFailed = True
				print "\033[31m Non equals\033[0m"
				os.system('cp %s %s' % (result, badResultName(base)) )
				msgs.append("Output %s mismatch"%base)
			removeIfExists(result)
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

def runBack2BackProgram(argv, back2back_files, help) :

	"--help" not in sys.argv or die(help, 0)

	architectureSpecific = "--arch" in argv
	if architectureSpecific : argv.remove("--arch")

	if "--accept" in argv :
		cases = argv[argv.index("--accept")+1:]
		cases or die("Option --accept needs a set of cases to accept. Try")
		accept(back2back_files, architectureSpecific, cases)
		sys.exit()

	if "--acceptall" in argv :
		print "Warning: Accepting any faling case"
		accept(back2back_files, architectureSpecific)
		sys.exit()

	sys.exit(0 if passB2BTests(back2back_files) else -1)


### End of generic stuff
