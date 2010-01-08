#!/usr/bin/python
import glob,os,re
#listOfWavs=glob.glob("sine*.wav")
listOfWavs=[]

for number in range(1,65):
	listOfWavs.append("sine%i.wav"%number)
#	os.system("sox -M %s %s" % (("sine.wav "*number),listOfWavs[-1]))
	
errorLevel=os.system("(cd .. ; scons && scons install)")
if errorLevel != 0:
	import sys
	sys.exit()
for file in listOfWavs:
	os.system("cp test_main.clamnetwork tmp.clamnetwork")
	numberOfChannels=re.search("[0-9]+",file).group(0)
	line='clamrefactor.py -x -c \'setConfig  SndfilePlayer SourceFile "%s"\' -c \'setConfig SndfilePlayer SavedNumberOfChannels %s\' tmp.clamnetwork' % (file,numberOfChannels) 
	print line
	errorLevel=os.system(line)
	if errorLevel!=0:
		import sys
		sys.exit()
	line='LADSPA_PATH=/dev/null OfflinePlayer tmp.clamnetwork sine1.wav -o salida%s.wav'%numberOfChannels
	print line
	errorLevel=os.system(line)
	if errorLevel!=0:
		import sys
		sys.exit()

	
