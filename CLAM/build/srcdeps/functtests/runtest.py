#!/usr/bin/python

logfile = 'timesLog.txt'

import os, sys, commands, time, shutil

settingsToTest = ['MIDIOut','FilePlayback', 'SMSTools']
settingsToTest = ['MIDIOut'] #TODOremove

if sys.platform in ['cygwin', 'win32'] :
	isWindows = True
	diffcmd = 'windiff'
else :
	isWindows = False
	print 'in linux'
	diffcmd = 'xxdiff'
	
def timeToExecute(cmd) :
	_,_,beforeUserTime,beforeSysTime,_ = os.times()
	os.system(cmd )
	_,_,afterUserTime,afterSysTime,_= os.times()
	return afterUserTime - beforeUserTime, afterSysTime - beforeSysTime

def appendTimesLog(testname, usrtime, systime) :
	date = time.strftime('%d/%m/%y\t(%A)')
	line = '%s\t%s\t%s\t%s\n' % (testname, usrtime, systime, date)
	file(logfile, 'a').write( line )

def RenameResultingDsp(test) :
	assert os.path.exists( test+'.dsp')
	target = test + '_result.dsp'
	shutil.copy(test+'.dsp', target)
	
def RenameResultingVcProj(test) :
	assert os.path.exists( test+'.vcproj')
	target = test + '_result.vcproj'
	shutil.copy(test+'.vcproj', target)

def compare( twoFiles ) :
	if commands.getoutput('diff -q %s %s' % twoFiles) :
		os.system( diffcmd+' %s %s' % twoFiles)
	else :
		print 'test passed: %s == %s ' % twoFiles
def createIfNotExists(filename)	:
	if not os.path.exists(filename) :
		print filename, "didn't exist: created void"
		file(filename,'w')
	
def empty(filename) :
	f = file(filename,'w')
	f.close()
	
for test in settingsToTest :
	config = test + '.cfg'
	if not os.path.exists(config) :
		print 'missing %s' % config
		continue
	expectedVars = test + '_expected.vars'
	expectedDsp = test+ '_expected.dsp'
	expectedVcproj = test + '_expected.vcproj'
	
	resultVars = test + '_result.vars'
	resultDsp = test + '_result.dsp'
	resultVcproj = test + '_result.vcproj'
	
	empty(resultVars)
	empty(resultDsp)
	empty(resultVcproj)
	
	createIfNotExists(expectedVars)
	createIfNotExists(expectedDsp)
	createIfNotExists(expectedVcproj)
		
	print 'testing', test
	usrtime, systime = timeToExecute('../srcdeps %s > %s ' % (config, resultVars))
	RenameResultingDsp(test)
	RenameResultingVcProj(test)
		
	print "user time: %s\nsys time: %s" % (usrtime, systime)
	appendTimesLog(test, usrtime, systime)

	varsToCompare = expectedVars, resultVars
	dspToCompare = expectedDsp, resultDsp
	vcprojToCompare = expectedVcproj, resultVcproj
	
	compare(varsToCompare)
	compare(dspToCompare)
	compare(vcprojToCompare)

