#! /usr/bin/python

import sys
from OSC import Message

if __name__ == "__main__":
	procname="-"
	print ">Processing name?"
	procname=sys.stdin.readline()

	print ">Control name?"
	ctrlname=sys.stdin.readline()

	while True:
		print ">Control value?"
		value=float(sys.stdin.readline())

		dest = "/"+procname[:-1]+"/"+ctrlname[:-1]
		Message(dest, [value]).sendto("193.145.55.66", 7000)


