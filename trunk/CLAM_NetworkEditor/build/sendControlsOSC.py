#! /usr/bin/python

import sys
from OSC import Message

if __name__ == "__main__":
	print "To quit write 'exit' as the Processing Name"
	procname="-"

	while True:
		print ">Processing name?"
		procname=sys.stdin.readline()

		if procname[:-1]=="exit":
			break

		print ">Control name?"
		ctrlname=sys.stdin.readline()
		print ">Control value?"
		value=float(sys.stdin.readline())

		Message("/"+procname[:-1]+"/"+ctrlname[:-1], [value]).sendlocal(7000)


