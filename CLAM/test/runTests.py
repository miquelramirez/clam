#! /usr/bin/python

#----------------------------------------------------------------------
# begin configuration

enableSendMail = False

# update level: 0-Keep, 1-Update, 2-CleanCheckout
# when the sandbox is not present always clean checkout
updateLevelForCLAM = 1
updateLevelForExamples = 0
updateLevelForTestData = 0

# When false keeps already compiled objects
doCleanMake = False
# When false does not run autoconf and configure unless a new checkout
doAutoconf = False
configureOptions = '--without-portmidi  --without-portaudio'
# Non-test are runned those seconds and then killed
executionTime = 15 

configurations = ['release'] #['debug', 'release'] 

# Mail report settings
publicAddress = 'clam-devel@iua.upf.es' # To use only when fails
privateAddress = 'parumi@iua.upf.es' # To know the test has been runned
subject = 'nightly tests report'


#this will be used only when it's not set in the environment
CVSROOT = ':ext:parumi@iua-share.upf.es:/mnt/cvsroot'



import commands
import os
import string
import sys

doAutoconf = doAutoconf or updateLevelForCLAM == 2

CLAM_SANDBOXES = os.path.abspath( os.path.dirname(sys.argv[0])+'/../..' ) + '/'
print 'CLAM_SANDBOXES=',CLAM_SANDBOXES

MODULE_TAG = '' #'development-branch'
if MODULE_TAG :
	SANDBOX_NAME = 'clean-'+MODULE_TAG
else :
	SANDBOX_NAME = 'clean-CLAM'

BUILDPATH = CLAM_SANDBOXES + '%s/build/' % (SANDBOX_NAME)
SALTO_DATA_FOLDER = CLAM_SANDBOXES + 'SaltoDataFolder/'

unitTestsPath = BUILDPATH+'Tests/UnitTests/'
functionalTestsPath = BUILDPATH+'Tests/FunctionalTests/'
spvTestsPath = BUILDPATH + 'Tests/SupervisedTests/'
nonPortedTestsPath = BUILDPATH + 'Tests/NonPortedTests/'
simpleExamplesPath = BUILDPATH + 'Examples/Simple/'

sandboxes = [ # Module, Sandbox, Tag, Update level
	( 'CLAM', SANDBOX_NAME, MODULE_TAG, updateLevelForCLAM),
	( 'CLAM_DescriptorsGUI', 'CLAM_DescriptorsGUI', '', updateLevelForExamples ),
	( 'CLAM_NetworkEditor', 'CLAM_NetworkEditor', '', updateLevelForExamples ),
	( 'CLAM_Rappid', 'CLAM_Rappid', '', updateLevelForExamples ),
	( 'CLAM_SDIFDisplay', 'CLAM_SDIFDisplay', '', updateLevelForExamples ),
	( 'CLAM_SpectralDelay', 'CLAM_SpectralDelay', '', updateLevelForExamples ),
	( 'CLAM_SMSTools', 'CLAM_SMSTools', '', updateLevelForExamples ),
	( 'CLAM_Salto', 'CLAM_Salto', '', updateLevelForExamples ),
	( 'CLAM_Voice2MIDI', 'CLAM_Voice2MIDI', '', updateLevelForExamples ),
	( 'CLAM-TestData', 'CLAM-TestData', '', updateLevelForTestData )
]
# update level: 0-Keep, 1-Update, 2-CleanCheckout
# when the sandbox is not present always clean checkout

automaticTests = [
	( 'UnitTests', unitTestsPath ),
	( 'FunctionalTests', functionalTestsPath ),
	( 'SMSToolsTests', CLAM_SANDBOXES+'CLAM_SMSTools/build/FunctionalTests/' )
]

externalApplications = [
#	( 'SpectralDelay', CLAM_SANDBOXES+'CLAM_SpectralDelay/build/'),
#	( 'SpectralDelay-Offline', CLAM_SANDBOXES+'CLAM_SpectralDelay/build/Offline/'),
#	( 'SpectralDelay-MultiBandProxyTest', CLAM_SANDBOXES+'CLAM_SpectralDelay/build/MultiBandProxyTest/'),
#	( 'SpectralDelay-DelayPoolTest', CLAM_SANDBOXES+'CLAM_SpectralDelay/build/DelayPoolTest/'),
	( 'NetworkEditor', CLAM_SANDBOXES+'CLAM_NetworkEditor/build/' ),
	( 'Voice2MIDI', CLAM_SANDBOXES+'CLAM_Voice2MIDI/build/' ),
	( 'SMSTools', CLAM_SANDBOXES+'CLAM_SMSTools/build/Tools/' ),
	( 'SMSBatch', CLAM_SANDBOXES+'CLAM_SMSTools/build/Batch/' ),
	( 'SMSConsole', CLAM_SANDBOXES+'CLAM_SMSTools/build/Console/' ),
#	( 'Rappid', CLAM_SANDBOXES+'CLAM_Rappid/build/' ),
	( 'SDIFDisplay', CLAM_SANDBOXES+'CLAM_SDIFDisplay/build/' )
#	( 'DescriptorsGUI', CLAM_SANDBOXES+'DescriptorsGUI/build/' ),
#	( 'Salto', CLAM_SANDBOXES+'CLAM_Salto/build/' ),
]

supervisedTests = [
	('SpectralPeaksPresentationTest', spvTestsPath+'SpectralPeaksPresentation/' ), 
	('SpectrumPresentationTest', spvTestsPath+'SpectrumPresentation/' ),
	('AudioPresentationTest', spvTestsPath+'AudioPresentation/' ),
	('FundFreqPresentationTest', spvTestsPath+'FundFreqPresentation/' ),
#	('AudioIOTest', spvTestsPath+'AudioIO/' ), TODO: fix. now needs root access and consumes all cpu
	('MIDIIOTest', spvTestsPath+'MIDIIO/' ),
	('Fl_EnvelopeTest', spvTestsPath+'Fl_Envelope/' ),
	('Test_Multiplot', spvTestsPath+'Plotsv2/Test_Multiplot/' ),
	('Test_SinglePlot', spvTestsPath+'Plotsv2/Test_SinglePlot/' ),
	('Test_SpecificPlots', spvTestsPath+'Plotsv2/Test_SpecificPlots/' )
]


notPortedTests = [
	( 'ArrayTest', nonPortedTestsPath+'Array/'),
        ( 'Array2Test', nonPortedTestsPath+'Array2/'),
        ( 'AssertTest', nonPortedTestsPath+'Assert/'),
        ( 'BPFTest', nonPortedTestsPath+'BPF/'),
#        ( 'EnvelopeExtractorTest', nonPortedTestsPath+'EnvelopeExtractor/'),
        ( 'ErrorTest', nonPortedTestsPath+'Error/'),
#        ( 'FDFilterGenTest', nonPortedTestsPath+'FDFilterGen/'),
        ( 'ListTest', nonPortedTestsPath+'List/'),
#        ( 'SegmentTest', nonPortedTestsPath+'Segment/'),
        ( 'Signalv1Test', nonPortedTestsPath+'Signalv1/'),
        ( 'SpectrumTest', nonPortedTestsPath+'Spectrum/'),
        ( 'SpectrumAdderTest', nonPortedTestsPath+'SpectrumAdder/'),
        ( 'SpectrumAdder2Test', nonPortedTestsPath+'SpectrumAdder2/'),
        ( 'SpectrumProductTest', nonPortedTestsPath+'SpectrumProduct/'),
        ( 'TabFunctTest', nonPortedTestsPath+'TabFunct/'),
        ( 'ThreadingTest', nonPortedTestsPath+'Threading/'),
        ( 'WaveGeneratorTest', nonPortedTestsPath+'WaveGenerator/'),
        ( 'WindowingTest', nonPortedTestsPath+'Windowing/')
]

simpleExamples = [
        ( 'AudioApplicationExample', simpleExamplesPath+'AudioApplication/'),
        ( 'AudioFileReading', simpleExamplesPath+'AudioFileReading/'),
        ( 'AudioFileWriting', simpleExamplesPath+'AudioFileWriting/'),
        ( 'ConfiguratorExample', simpleExamplesPath+'Configurator/'),
	( 'ControlArraysExample', simpleExamplesPath+'ControlArrays/'),
        ( 'ControlsExample', simpleExamplesPath+'Controls/'),
        ( 'DescriptorsComputationExample', simpleExamplesPath+'DescriptorsComputation/'),
        ( 'FDFilterExample', simpleExamplesPath+'FDFilter/'),
        ( 'FFTExample', simpleExamplesPath+'FFT/'),
        ( 'FileInfo', simpleExamplesPath+'FileInfo/'),
        ( 'FilePlayback', simpleExamplesPath+'FilePlayback/'),
        ( 'LPCExample', simpleExamplesPath+'LPC/'),
        ( 'MIDISynthesizer', simpleExamplesPath+'../MIDISynthesizer/'),
        ( 'midi2xml', simpleExamplesPath+'MIDI2XML/'),
        ( 'MIDIIO', simpleExamplesPath+'MIDIIO/'),
        ( 'MIDIOut', simpleExamplesPath+'MIDIOut/'),
        ( 'MultiPlot', simpleExamplesPath+'MultiPlot/'),
        ( 'NetworkPersistence', simpleExamplesPath+'NetworkPersistence/'),
        ( 'NetworkUsage', simpleExamplesPath+'NetworkUsage/'),
        ( 'POCompositeExample', simpleExamplesPath+'POComposite/'),
        ( 'PersistenceThroughDTs', simpleExamplesPath+'PersistenceThroughDTs/'),
        ( 'PortsAndControlsUsage', simpleExamplesPath+'PortsAndControlUsage/'),
        ( 'ProcessingLifeCycle', simpleExamplesPath+'ProcessingLifeCycle/'),
	( 'DPAudio', simpleExamplesPath+'QtPlots/DirectPlotsExamples/DPAudioExample/'),
	( 'DPFundamental', simpleExamplesPath+'QtPlots/DirectPlotsExamples/DPFundamentalExample/'),
	( 'DPSpectrumAndPeaks', simpleExamplesPath+'QtPlots/DirectPlotsExamples/DPSpectrumAndPeaksExample/'),
	( 'DPAudioStereo', simpleExamplesPath+'QtPlots/DirectPlotsExamples/DPAudioStereoExample/'),
	( 'ListPlot', simpleExamplesPath+'QtPlots/ListPlotExample/'),
	( 'AudioPlot', simpleExamplesPath+'QtPlots/QtPlotsExamples/AudioPlotExample/'),
	( 'SpectrumPlot', simpleExamplesPath+'QtPlots/QtPlotsExamples/SpectrumPlotExample/'),
        ( 'SDIF_And_Segment', simpleExamplesPath+'SDIF_And_Segment/'),
        ( 'SimplePortsUsage', simpleExamplesPath+'SimplePortsUsage/'),
        ( 'SinglePlot', simpleExamplesPath+'SinglePlot/'),
        ( 'SpectrumExample', simpleExamplesPath+'Spectrum/'),
        ( 'ThreadedProcessing', simpleExamplesPath+'ThreadedProcessing/')
]


testsToRun = []
# insert sub-lists to the main list: 
#    this makes debugging easier
testsToRun[-1:-1] = automaticTests 
testsToRun[-1:-1] = externalApplications 
testsToRun[-1:-1] = simpleExamples
testsToRun[-1:-1] = supervisedTests
testsToRun[-1:-1] = notPortedTests

sender = '"automatic tests script" <parumi@iua.upf.es>'

# end configuration
#--------------------------------------------------------------------

# global vars. ugly, yes.
foundCompilationErrors = False 
foundTestsFailures = False
foundExecutionErrors = False


def sendmail(fromaddr, toaddrs, subject, body) :
	import smtplib

	# Add the From: and To: headers at the start!
	msg = "From: %s\r\nTo: %s\r\nSubject: %s\r\n" % (fromaddr, toaddrs, subject) + body
	if not enableSendMail :
		return
	
	server = smtplib.SMTP('iua-mail.upf.es')
	server.set_debuglevel(1)
	server.sendmail(fromaddr, toaddrs, msg)
	server.quit()

def checkPaths() :
	global testsToRun
	out = ""
	if not os.access(CLAM_SANDBOXES, os.F_OK) :
		err = "Sorry can't access CLAM_SANDBOXES path : " + CLAM_SANDBOXES
		sendError(err)
		sys.exit(1)
	dirsExpected = setfilesExpected = ''
	i=0
	for name, path in testsToRun :
		if not os.access(path, os.F_OK) :
			dirsExpected += '\t- %s\n'%path
			testsToRun[i] = ('','')
		else :
			setfile = path+'settings.cfg'
			if not os.access(setfile, os.F_OK) :
				setfilesExpected +='\t- %s\n'%setfile
				testsToRun[i] = ('','')
		i += 1
	if dirsExpected!='' or setfilesExpected!='' :
		msg = 'dirs not found:\n\n%s\n files not found\n%s\n'
		out = msg % (dirsExpected,setfilesExpected)
		# print out
	return out
	

def parseCompilationWarnings(compilationOut) :
	nwarnings = compilationOut.count('warning')
	warnings = ''
	if nwarnings > 0 :
		warnings = ' - found %d warnings !'% (nwarnings)
		# print warnings
	return warnings

def parseTestsFailures( testsOut ) :
	state='TESTS_INFO'
	summary = 'tests results: '
	details = ''
	for line in testsOut.split('\n') :
		if state == 'TESTS_INFO' :
			if line.find('!!!FAILURES!!!') >=0 :
				# print 'found !!!FAILURES!!!'
				details = line + '\n'
				summary +=  line
				state = 'FAILURES'
			if line.find('OK (') == 0 :
				# print 'found OK'
				details = ''
				summary += line
		elif state == 'FAILURES' :
			details +=  line + '\n'
		else :
			assert(false)
	# print 'in parseTestsFailures :', summary, ' details: ',details
	return summary, details

def parseExecutionErrors( executionOut ) :
	# print 'executed non test\n', executionOut
	if executionOut.find('# ASSERTION FAILED #')>=0 :
		return 'execution: assertion failed!', executionOut
	if executionOut.find('aborted')>=0:
		return 'execution: aborted!', executionOut
	#TODO violacio de segment
	if executionOut.find('# WARNING #')>=0 :
		return 'execution OK - althought warning-assert(s)', executionOut
	return 'execution OK', ''


def isTest(path) :
	return path.find('UnitTests/')>=0 or path.find('FunctionalTests/')>=0 

#----------------------------------------------------------------
def getStatusOutput(cmd) :	
	"returns wheather cmd exits correctly and the output"
	print 'executing ',cmd
	stat, output = commands.getstatusoutput(cmd)		
	if stat != 0 : 
		print output
	else :
		print ' OK'
	return (stat == 0), output

def executeMandatory(cmd) :
	stat, output = getStatusOutput(cmd)
	if not stat : 
		sendError(output)
		sys.exit(1)
#----------------------------------------------------------------

def formatSummary(name, configuration, result) :
	nameConfig = '%s (%s) ' % (name, configuration)
	secondRow = 50
	npoints = 0
	if len(nameConfig) < secondRow :
		npoints += secondRow - len(nameConfig)
	points = '.'
	for i in range(npoints): 
		points +='.'
	return nameConfig + points + result+'\n'

def compileAndRun(name, path) :
	global foundCompilationErrors, foundExecutionErrors, foundTestsFailures, configurations
	if name == '' :
		# print 'found removed test (invalid dir or settings file)'
		return '',''
	os.chdir(path)
	# compilation phase
	summary = ''
	details = ''
	for configuration in configurations :
		# compilation phase
		if doCleanMake :
			getStatusOutput('make clean')
		makecmd = 'make depend && make CONFIG=%s' % (configuration)
		ok, output = getStatusOutput( makecmd )
		foundCompilationErrors = foundCompilationErrors or not ok
		if not ok :
			compilationSumary = 'COMPILATION ERRORS'
		else :
			compilationSumary = 'compilation OK'
		compilationSumary += parseCompilationWarnings( output )
		summary += formatSummary(name, configuration, compilationSumary)
		detailsFormat = '\n\n%s\n-----------------------------\n%s\n'
		if not ok :
			details += detailsFormat % (name, output)
		if not ok : 
			continue
			
		# execution phase
		execcmd = './'+name
		if not os.access(execcmd, os.X_OK) :
			print 'file should exist: ', execcmd
			assert(False)
		if isTest(path) :
			# print 'isTest yes\nrunning tests'
			ok, output = getStatusOutput( execcmd )
			foundTestsFailues = foundTestsFailures or not ok
			runMessages, d = parseTestsFailures( output )
		else :
			# print 'isTest no\nexecuting application for a while'
			ok, output = runInBackgroundForAWhile(path, execcmd, executionTime)
			#TODO have into account that when segfault returns(?) 139
			runMessages, d = parseExecutionErrors( output )
			foundExecutionErrors =  foundExecutionErrors or not ok or runMessages.find('OK')==-1
		
		summary += formatSummary(name, configuration, runMessages)
		if d != '' :
			details += detailsFormat % (name, d)
	os.chdir(CLAM_SANDBOXES)

	return summary, details


mailTemplate = '''
(This message has been automatically generated)

Status of CLAM on tag: %s  And externals examples in main trunk)

-------  
SUMMARY
-------
%s

---------------------------------------
DETAILS (only if finds errors/failures)
---------------------------------------
%s

-----------------------------------------------------------
// Powered by Python //
'''

import time, string, signal
def runInBackgroundForAWhile(path, command, sleeptime=10) :
	os.chdir(path)
	out, err = '/tmp/removeme.out', '/tmp/removeme.err'
	#file(out, 'w')
	#file(err, 'w')
	fullcmd = '%s > %s 2> %s &' % (command, out, err)
	# print fullcmd
	# print 'result ',os.system( fullcmd )
	time.sleep( sleeptime )
	withoutSlash = command[command.find('/')+1 : ]

	status, dummy = commands.getstatusoutput('killall '+ withoutSlash)
	
	# print 'kill status ', status, dummy
	if os.access('/tmp/removeme.out', os.F_OK) :
		result = string.join( file('/tmp/removeme.out').readlines() )
		os.remove(out)
	else :
		result = "[No output received from '%s']"%command
	if os.access('/tmp/removeme.err', os.F_OK) :
		result += string.join( file('/tmp/removeme.err').readlines() )
		os.remove(err)
	else :
		result = "[No error messages received from '%s']"%command
	os.chdir(CLAM_SANDBOXES)
	return True, result #TODO status


def sendError(usermsg='') :
	import traceback
	tb = string.join( traceback.format_exception(sys.exc_info()[0], sys.exc_info()[1], sys.exc_info()[2]) )
	subject = 'unexpected error on runTests.py script'
	errormsg = usermsg + 'Python interpreter said:\n' + tb
	print errormsg
	if privateAddress != '' :
		sendmail( sender, privateAddress, subject, errormsg )

def checkoutSandbox (module, sandbox, tag) :
	tagOption = ''
	if tag!='' : tagOption = '-r '+tag
	os.chdir(CLAM_SANDBOXES)
	executeMandatory('cvs checkout %s -d %s %s' % (tagOption, sandbox, module) )




def updateSandboxes() :
	global sandboxes
	for module, sandbox, tag, level in sandboxes :
		if not os.access(sandbox, os.F_OK) :
			print 'Cannot update sandbox %s (dont exist), doing checkout'%(sandbox)
			checkoutSandbox(module, sandbox, tag)
		elif level == 2 :
			print 'The sandbox %s already exists, deleting it'%(sandbox)
			getStatusOutput('rm -rf '+sandbox )
			checkoutSandbox(module, sandbox, tag)
		elif level == 1 :
			print 'The sandbox %s already exists, updating it'%(sandbox)
			os.chdir(sandbox)
			ok, output = getStatusOutput( 'cvs update -dP' )
			if output.find('\nC ')>=0 :
				print 'CVS CONFLICT !!', output
				# TODO: Inform about them
			os.chdir(CLAM_SANDBOXES)
		elif level == 0 :
			print 'The sandbox %s already exists, keeping it'%(sandbox)

def deployClamBuildSystem() :
	# BuildSrcDeps
	os.chdir(BUILDPATH+'srcdeps/')
	executeMandatory('make')

	# ConfigureClam
	os.chdir(BUILDPATH)
	if doAutoconf:
		executeMandatory('autoconf')
		executeMandatory('./configure %s'%configureOptions)

	# Setting the clam location
	global sandboxes
	os.chdir(CLAM_SANDBOXES)
	for module, sandbox, tag, level in sandboxes :
		clamlocationfile = sandbox + '/build/clam-location.cfg'
		if not os.access(clamlocationfile, os.F_OK) :
			continue
		executeMandatory('echo \'CLAM_PATH = %s%s\' > %s' %(CLAM_SANDBOXES, SANDBOX_NAME, clamlocationfile))


	
#-------------------------------------------------------------------------------------  
#  Aplication Logic
#
def runTests() :
	totalSummary = ['']
	totalDetails = ['']
	subj = [subject]
	if 'CVSROOT' not in os.environ :
		print 'warning: CVSROOT not found in environ'
		os.environ['CVSROOT'] = CVSROOT

	os.environ['CVS_RSH'] = getStatusOutput('which ssh')[1]
	os.chdir(CLAM_SANDBOXES)
	#sanity check
	if SANDBOX_NAME in ['devel','CLAM'] : 
		sendError( 'ups, trying to remove devel sandbox !!' )
		sys.exit(-1)

	updateSandboxes()
	totalSummary.append(checkPaths())
	deployClamBuildSystem()

	# LinkSaltoDataFolder
#	os.chdir(CLAM_SANDBOXES + SANDBOX_NAME+ '/build/Examples/Salto')
#	executeMandatory('ln -s ' + SALTO_DATA_FOLDER)

	# compile and run/tests entries
	for name, path in testsToRun :
		print '\n\nname\t\t %s \npath \t\t%s \n' % (name, path)
		summary, details  = compileAndRun(name, path)
		totalSummary.append(summary)
		totalDetails.append(details)
		print "".join(totalSummary)

	mailBody = mailTemplate  % ( MODULE_TAG, "".join(totalSummary), "".join(totalDetails) )
	if foundCompilationErrors : 
		subj.append(' - compilation err!')
	if foundTestsFailures :
		subj.append(' - tests failures!')
	if foundExecutionErrors :
		subj.append(' - execution errs!')

	if foundCompilationErrors or foundTestsFailures or foundExecutionErrors :
		sendReportTo = publicAddress
	else :
		sendReportTo = privateAddress

	if sendReportTo != '' :
		sendmail( sender, sendReportTo, "".join(subj), mailBody )
	else :
		print 'nowbody to send report'
		print 'subject: ', "".join(subj)
		print mailBody

	#write log
	results = file(CLAM_SANDBOXES + "RunTestResults.txt", "w")
	results.write("".join(subj))
	results.write("".join(totalSummary))
	results.write("".join(totalDetails))
	results.close()
#--------------------------------------------------------------
#
#  If called from command-line, parse arguments and take actions
#
if __name__ == '__main__':
	try :
		runTests()
	except KeyboardInterrupt :
		print 'interrupted by the user'
	except:
		sendError()

