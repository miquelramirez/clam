#! /usr/bin/python
# -*- coding: iso-8859-15 -*-

#TODO: 
# - filter out ling srcdeps output in compilation errors
# - tests able to be marked as not-mandatory, so if failed just gives warning
#----------------------------------------------------------------------
# begin configuration


	
enableSendMail = True
enablePlaceCvsTags = True

quickTestForScriptDebuging = False  # important: to be enabled for debuging puroposes only

# update level: 0-Keep, 1-Update, 2-CleanCheckout
# when the sandbox is not present always clean checkout
updateLevelForCLAM = 2
updateLevelForExamples = 2
updateLevelForTestData = 1 

# When false keeps already compiled objects
doCleanMake = True
# When false does not run autoconf and configure unless a new checkout
doAutoconf = True
configureOptions = '--without-portmidi  --without-portaudio'
# Non-test are runned those seconds and then killed
executionTime = 15

#configurations = ['debug', 'release'] 
configurations = ['release'] 

# Mail report settings
publicAddress = 'clam-devel@iua.upf.es' # To use only when some test fails
privateAddress = 'parumi@iua.upf.es' # To know the test has been runned
subject = 'nightly tests report'
maxLinesPerMail = 200

#this will be used only when it's not set in the environment
#CVSROOT = ':ext:parumi@iua-share.upf.es:/mnt/cvsroot'

import commands
import os
import string
import sys

doAutoconf = doAutoconf or updateLevelForCLAM == 2

CLAM_SANDBOXES = os.path.dirname( os.path.abspath( sys.argv[0])+'/../..' ) + '/'
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

# update level: 0-Keep, 1-Update, 2-CleanCheckout
# when the sandbox is not present always clean checkout

if quickTestForScriptDebuging : 
	enableSendMail = False
	enablePlaceCvsTags = False
	updateLevelForCLAM = 1 
	updateLevelForExamples = 0
	updateLevelForTestData = 0 
	doCleanMake = False
	doAutoconf = False
	
sandboxes = [ # Module, Sandbox name, Tag, Update level
	( 'CLAM', SANDBOX_NAME, MODULE_TAG, updateLevelForCLAM),
	( 'CLAM_DescriptorsGUI', 'clean-CLAM_DescriptorsGUI', '', updateLevelForExamples ),
	( 'CLAM_NetworkEditor', 'clean-CLAM_NetworkEditor', '', updateLevelForExamples ),
	( 'CLAM_Rappid', 'clean-CLAM_Rappid', '', updateLevelForExamples ),
	( 'CLAM_SDIFDisplay', 'clean-CLAM_SDIFDisplay', '', updateLevelForExamples ),
	( 'CLAM_SpectralDelay', 'clean-CLAM_SpectralDelay', '', updateLevelForExamples ),
	( 'CLAM_SMSTools', 'clean-CLAM_SMSTools', '', updateLevelForExamples ),
	( 'CLAM_Annotator', 'clean-CLAM_Annotator', '', updateLevelForExamples ),
	( 'CLAM_Salto', 'clean-CLAM_Salto', '', updateLevelForExamples ),
	( 'CLAM_Voice2MIDI', 'clean-CLAM_Voice2MIDI', '', updateLevelForExamples ),
	( 'CLAM-TestData', 'CLAM-TestData', '', updateLevelForTestData )
]
def baseDirOf(keyname) :
	for modulename, sandboxname, _, _  in sandboxes :
		if modulename.find(keyname) >= 0 : 
			return CLAM_SANDBOXES+ sandboxname
	assert False, "in baseDirOf() wrong module keyname"


automaticTests = [
	( 'UnitTests', unitTestsPath ),
	( 'FunctionalTests', functionalTestsPath ),
	( 'SMSToolsTests', baseDirOf('SMSTools')+'/build/FunctionalTests/' )
]

externalApplications = [
#	( 'SpectralDelay', CLAM_SANDBOXES+'CLAM_SpectralDelay/build/'),
#	( 'SpectralDelay-Offline', CLAM_SANDBOXES+'CLAM_SpectralDelay/build/Offline/'),
#	( 'SpectralDelay-MultiBandProxyTest', CLAM_SANDBOXES+'CLAM_SpectralDelay/build/MultiBandProxyTest/'),
#	( 'SpectralDelay-DelayPoolTest', CLAM_SANDBOXES+'CLAM_SpectralDelay/build/DelayPoolTest/'),
	( 'NetworkEditor', baseDirOf('NetworkEditor')+'/build/' ),
	( 'Voice2MIDI', baseDirOf('CLAM_Voice2MIDI')+'/build/' ),
	( 'SMSTools', baseDirOf('SMSTools')+'/build/Tools/' ),
	( 'Annotator', baseDirOf('Annotator')+'/build/' ),
	( 'SMSBatch', baseDirOf('SMSTools')+'/build/Batch/' ),
	( 'SMSConsole', baseDirOf('SMSTools')+'/build/Console/' ),
	( 'SDIFDisplay', baseDirOf('SDIFDisplay')+'/build/' )
#	( 'Rappid', baseDirOf('Rappid')+'/build/' ),
#	( 'DescriptorsGUI', baseDirOf('DescriptorsGUI')+'/build/' ),
#	( 'Salto', baseDirOf('Salto')+'/build/' ),
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
#        ( 'TabFunctTest', nonPortedTestsPath+'TabFunct/'),
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
        ( 'ThreadedProcessing', simpleExamplesPath+'ThreadedProcessing/'),
	( 'BPFEditor', simpleExamplesPath+'QtPlots/BPFEditorExample/'),
	( 'SinTracks', simpleExamplesPath+'QtPlots/QtPlotsExamples/SinTracksPlotExample/'),
	( 'Spectrogram', simpleExamplesPath+'QtPlots/QtPlotsExamples/SpectrogramExample/')
]


testsToRun = []
# insert sub-lists to the main list: 
#    this makes debugging easier
testsToRun[-1:-1] = automaticTests 
testsToRun[-1:-1] = externalApplications 
testsToRun[-1:-1] = simpleExamples
testsToRun[-1:-1] = supervisedTests
testsToRun[-1:-1] = notPortedTests

if quickTestForScriptDebuging :
	testsToRun = [( 'UnitTests', unitTestsPath ),( 'MIDISynthesizer', simpleExamplesPath+'../MIDISynthesizer/') ]
	
sender = '"automatic tests script" <parumi@iua.upf.es>'

# end configuration
#--------------------------------------------------------------------

from testResult import *

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
	

def parseCompilationWarnings(compilationOut) : #TODO depracate
	nwarnings = compilationOut.count('warning') + compilationOut.count('av�s')
	warnings = ''
	if nwarnings > 0 :
		warnings = ' - found %d warnings !'% (nwarnings)
		# print warnings
	return warnings

def getNWarnings(compilationOut):
	return compilationOut.count('warning') + compilationOut.count('av�s')
	

def parseTestsFailures( testsOut ) :
	state='TESTS_INFO'
	summary = 'tests results: '
	details = ''
	for line in testsOut.split('\n') :
		if line.find('Run:  ') == 0 :
			summary += line
		
		if state == 'TESTS_INFO' :
			if line.find('!!!FAILURES!!!') >=0 :
				# print 'found !!!FAILURES!!!'
				details = line + '\n'
				summary +=  'Failures! '
				state = 'FAILURES'
			if line.find('OK (') == 0 :
				# print 'found OK'
				details = ''
				summary += line
				state = 'OK'
		elif state == 'FAILURES' :
			details +=  line + '\n'
		elif state == 'OK':
			pass
		else :
			assert False, 'error parsing tests failures'
	if state == 'TESTS_INFO' :
		summary += 'could not terminate!'
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
	return isUnitTest(path) or isFunctionalTest(path)
def isUnitTest(path):
	return path.find('UnitTests/')>=0  
def isFunctionalTest(path):
	return path.find('FunctionalTests/')>=0  

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
	stat, output = getStatusOutput(cmd ) #+' 2>&1')
	if not stat : 
		sendError(output)
		sys.exit(1)
#----------------------------------------------------------------

def formatSummary(name, configuration, result) :
	nameConfig = '%s (%s) ' % (name, configuration)
	secondRow = 35
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
		return 'found invalid test (see message above)', '', TestResult()
	
	os.chdir(path)
	# compilation phase
	summaryText = ['']
	testResult = TestResult()
	testResult.name = name
	
	details = ''
	for configuration in configurations :
		# compilation phase
		if doCleanMake or len(configurations)>1:
			getStatusOutput('make clean')
		makecmd = 'make depend 2> /dev/null && make CONFIG=%s' % (configuration)
		ok, output = getStatusOutput( makecmd )
		foundCompilationErrors = foundCompilationErrors or not ok
		if not ok :
			compilationSumary = 'COMPILATION ERRORS' #TODO depracate
		else :
			compilationSumary = 'compilation OK'
		
		testResult.compilationOk(configuration, ok)
#		testResult.nWarnings(configuration, getNWarnings(output) )
		
		compilationSumary += parseCompilationWarnings( output )
		summaryText.append(formatSummary(name, configuration, compilationSumary) )
		detailsFormat = '\n\n%s\n-----------------------------\n%s\n'
		if not ok :
#			testResult.reportError(configuration, output)
			details += detailsFormat % (name, output)
			continue 
			
		# execution phase
		execcmd = './'+name
		assert os.access(execcmd, os.X_OK), 'file should exist: ' + execcmd
		
		if isTest(path) :
			ok, output = getStatusOutput( execcmd )
			ok = (output.find("\n\nOK (") > 0)
			foundTestsFailues = foundTestsFailures or not ok
			runMessages, d = parseTestsFailures( output )
			if isUnitTest(path) :
				failures = 0 
				if not ok : failures = 1 #TODO parse output to obtain failures
				testResult.unitTestsFailures(configuration, failures)
			
		else :
			ok, output = runInBackgroundForAWhile(path, execcmd, executionTime)
			#TODO have into account that when segfault returns(?) 139
			runMessages, d = parseExecutionErrors( output )
			foundExecutionErrors =  foundExecutionErrors or not ok or runMessages.find('OK')==-1
		
		summaryText.append( formatSummary(name, configuration, runMessages) )
		if d != '' :
			details += detailsFormat % (name, d)

	os.chdir(CLAM_SANDBOXES)
	return "".join(summaryText), details, testResult


mailTemplate = '''
(This message has been automatically generated)

%s

Status of CLAM and externals examples, in main trunk

	SUMMARY
	-------

%s

	DETAILS (only if finds errors/failures)
	---------------------------------------
%s

-----------------------------------------------------------
// Powered by Python //
'''
from guiltyCommits import placeTestsOkCandidateTags, placeTestsOkTags, chaseTheGuiltyCommits

import time, string, signal
def runInBackgroundForAWhile(path, command, sleeptime=10) :
	os.chdir(path)
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
	executeMandatory('make clean && make')

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
	totalSummary = [''] # TODO depracate
	totalDetails = [''] # TODO depracate
	resultSet = TestResultSet()
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

	if enablePlaceCvsTags :
		placeTestsOkCandidateTags("CLAM")
		placeTestsOkCandidateTags("CLAM_NetworkEditor")
		placeTestsOkCandidateTags("CLAM_SMSTools")

	updateSandboxes()
	totalSummary.append(checkPaths())
	deployClamBuildSystem()

	# LinkSaltoDataFolder
#	os.chdir(CLAM_SANDBOXES + SANDBOX_NAME+ '/build/Examples/Salto')
#	executeMandatory('ln -s ' + SALTO_DATA_FOLDER)

	# compile and run/tests entries
	for name, path in testsToRun :
		print '\n\nname\t\t %s \npath \t\t%s \n' % (name, path)
		aSummary, aDetail, aTestResult  = compileAndRun(name, path)
		totalSummary.append(aSummary)
		totalDetails.append(aDetail)
		resultSet.add( aTestResult )
		print "test name: %s \t compiles: %i " % (aTestResult.name, aTestResult.compiles() )
		print aSummary     # for console monitoring purposes
	
	requiredStabilityLevel = PassUnitTests # Compiles 
	clamIsBroken = resultSet.stabilityLevel() < requiredStabilityLevel
	print "\nGlobal test results with %i tests. Stability level: %i Required: %i " % ( resultSet.nTests(), resultSet.stabilityLevel(), requiredStabilityLevel )
	guiltyReport = "Chasing-guilty-commits is DISABLED"
	if enablePlaceCvsTags :
		if clamIsBroken :
			guiltyReport = chaseTheGuiltyCommits("CLAM")
			guiltyReport += chaseTheGuiltyCommits("CLAM_NetworkEditor")
			guiltyReport += chaseTheGuiltyCommits("CLAM_SMSTools")
		else :
			placeTestsOkTags("CLAM")
			placeTestsOkTags("CLAM_NetworkEditor")
			placeTestsOkTags("CLAM_SMSTools")
			guiltyReport = ''

	if len(totalDetails) > maxLinesPerMail :
		totalDetails = ["Too long output. So skiping the details section."]

	mailBody = mailTemplate  % ( guiltyReport, "".join(totalSummary), "".join(totalDetails) )


	if clamIsBroken :
		sendReportTo = publicAddress
		subj.append(" -- "+ resultSet.stabilityLevelString() )
	else :
		sendReportTo = privateAddress

	sendmail( sender, sendReportTo, "".join(subj), mailBody )

	#write log
	results = file(CLAM_SANDBOXES + "RunTestResults.txt", "w")
	results.write("".join(subj))
	results.write(mailBody)
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

