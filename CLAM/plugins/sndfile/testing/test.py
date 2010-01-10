#!/usr/bin/python
import glob,os,re,sys

def run(command) :
	print "\033[32m%s\033[0m"%command
	result = os.system(command)
	if result == 0 : return
	print "\033[31mExecution failed\033[0m"
	sys.exit(-1)

errorLevel=os.system("(cd .. ; scons && scons install)")
if errorLevel != 0:
	sys.exit()

for n in range(1,18) :
	file = "sine%02i.wav"%n
	if not os.access(file, os.R_OK) :
		if n==1 :
			run("cp sine.wav sine01.wav")
		else :
			run("sox -M %s %s" % ("sine.wav "*n,file))
	run("cp test_main.clamnetwork tmp.clamnetwork")
	run('clamrefactor.py -x -c \'setConfig  SndfilePlayer SourceFile "%s"\' -c \'setConfig SndfilePlayer SavedNumberOfChannels %s\' tmp.clamnetwork' % (file,n) )
	run('LADSPA_PATH=/dev/null OfflinePlayer tmp.clamnetwork sine.wav -o salida%s.wav'%n)

	
