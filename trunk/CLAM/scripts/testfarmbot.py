#!/usr/bin/python

import sys
import socket
import string

# Basic idea was taken from http://www.oreilly.com/pub/h/1968

HOST="irc.freenode.net"
PORT=6667
NICK="testfarmbot"
IDENT="testfarmbot"
REALNAME="testfarmbot"
DFT_CHANNEL="#clam"
DFT_MSG="testing again"

def join_channel( channel, s ):
	s.send('JOIN '+channel+'\r\n')

def send_msg( channel, msg, s ):
	s.send('PRIVMSG '+channel+' :'+msg+'\r\n')

def send_msg_dft_channel( msg, s ):
	s.send('PRIVMSG '+DFT_CHANNEL+' :'+msg+'\r\n')

# /me like message
def send_action_msg_dft_channel( msg, s ):
	s.send('PRIVMSG '+DFT_CHANNEL+' :\01ACTION '+msg+'\r\n')

def send_notice_dft_channel( msg, s ):
	s.send('NOTICE '+DFT_CHANNEL+' :'+msg+'\r\n')

def quit( msg, s ):
	s.send('QUIT'+' :'+msg+'\r\n')
	
def quit( msg, s ):
	s.send('QUIT'+' :'+msg+'\r\n')

def send_and_quit( message ):
	readbuffer=""
	s=socket.socket()
	s.connect((HOST, PORT))
	s.send("NICK %s\r\n" % NICK)
	s.send("USER %s %s bla :%s\r\n" % (IDENT, HOST, REALNAME))

	done=0
	while 1:
		readbuffer=readbuffer+s.recv(1024)
		temp=string.split(readbuffer, "\r\n")
		readbuffer=temp.pop( )

		if done:
			quit( "good bye!", s )
			s.close()
			return

		for line in temp:
			print line
			if line.find('End of /MOTD command')!=-1:
				print "Connected to " + HOST
				join_channel( DFT_CHANNEL, s )
				send_notice_dft_channel( message, s )
				done=1

			line=string.rstrip(line)
			line=string.split(line)

		if(line[0]=="PING"):
			s.send("PONG %s\r\n" % line[1])

if __name__ == "__main__" :
	send_and_quit(sys.argv[1])
