#! /usr/bin/python

import sys, time, math
from OSC import Message

host = ""

def deprecatedOscExample() : # by Xavier Oliver
	if not (len(sys.argv)==1 or len(sys.argv)==2) :
		print "usage: sendControlsOSC [host]\n"	
		sys.exit(1)

	if len(sys.argv)==2 :
		host = sys.argv[1]
		print "using host:", host
	else:
		host = ""
		print "using local host"

	procname="-"
	print ">Processing name?"
	procname=sys.stdin.readline()

	print ">Control name?"
	ctrlname=sys.stdin.readline()

	while True:
		print ">Control value?"
		value=float(sys.stdin.readline())

		dest = "/"+procname[:-1]+"/"+ctrlname[:-1]
		if host :
			Message(dest, [value]).sendto(host, 7000)
		else :
			Message(dest, [value]).sendlocal(7000)

def send(path, datalist) :
	floatlist = [float(element) for element in datalist]
	if host :
		Message(path, floatlist).sendto(host, 7000)
	else :
		Message(path, floatlist).sendlocal(7000)


def exampleInteractive() :
	while True:
		value1=float(sys.stdin.readline())
		value2=float(sys.stdin.readline())
		data = [value1, value2 ]
		send("/ebowSynthesizer", data)
def exampleWalkInCircles() :
	delta = 0.
	increment = math.pi/20 # in radians
	timestep = 0.2 # seconds
	while True:
		value1 = math.sin(delta)
		value2 = math.cos(delta)
		data = [value1, value2 ]
		print data
		send("/azimut", data)
		delta += increment
		if delta > 2*math.pi :
			delta -= 2*math.pi
		time.sleep(timestep)

def exampleRotatingAngleDegrees() :
	delta = 0.
	increment = 360/100 # in degrees
	timestep = 0.1 # seconds
	while True:
		print delta
		send("/azimut", [delta, 0])
		delta += increment
		if delta >= 360 :
			delta -= 360
		time.sleep(timestep)
			
		

if __name__ == "__main__":
#	exampleWalkInCircles()
	exampleRotatingAngleDegrees()

