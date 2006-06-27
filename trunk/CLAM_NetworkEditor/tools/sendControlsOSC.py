#! /usr/bin/python

import sys
from OSC import Message

if __name__ == "__main__":
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


