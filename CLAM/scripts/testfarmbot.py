#!/usr/bin/python
# -*- coding: UTF-8 -*-

# TODO list 
#  * implement reconection (if irc connection is lost)
#  * a way to close the main server from the socket (for example "quit_all")

# Usage: 'telnet localhost 2222' or any simple TCP client connected to 2222 port
#
# Example code:
# import socket
# s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# s.connect(("localhost", 2222))
# s.send("Hello\n")
# s.close()
#
# Some basic commands:
# 	every line will be sent to the irc default channel
#	quit: ends the tcp connection
#	quit_irc: ends the irc connection

import sys
import string
from collections import deque
import socket, SocketServer
import threading, time
import Queue

HOST="irc.freenode.net"
PORT=6667
NICK="testfarmbot"
IDENT="testfarmbot"
REALNAME="testfarmbot"
DFT_CHANNEL="#clam"
DFT_MSG="hi clam folks"

MessageQ = Queue.Queue()

class IRC_Connection( threading.Thread ):
	
	def __init__( self, *args ):
		threading.Thread.__init__( self, *args )

		#irc connection socket
		self.irc_socket = socket.socket()

	def run( self ):
		print "Starting irc connection..."
		
		readbuffer = ""
		self.irc_socket.connect( (HOST, PORT) )
		self.irc_socket.send( "NICK %s\r\n" % NICK )
		self.irc_socket.send( "USER %s %s bla :%s\r\n" % (IDENT, HOST, REALNAME) )
	
		while True:
			self.irc_socket.setblocking(0)
			try:
				readbuffer = readbuffer + self.irc_socket.recv( 1024 )
			except:
				try:
					line = MessageQ.get_nowait()
				except:
					time.sleep( 1 )
					continue
				else:
					if line=="quit_irc":
						self.quit( "good bye!" )
						return
					self.send_message( line )
					continue
			
			self.irc_socket.setblocking(1)
			
			r_temp = string.split( readbuffer, "\r\n" )
			readbuffer = r_temp.pop()

			for line in r_temp:
				print line

				if line.find('End of /MOTD command') != -1:
					print "Connected to " + HOST
					self.join_channel( DFT_CHANNEL )
					self.send_message( DFT_MSG )

				line = string.rstrip(line)
				line = string.split(line)
				if line[0]=="PING":
					self.irc_socket.send( "PONG %s\r\n" % line[1] )

	def send_message( self, line ):
		"""set here the default way of sending messages to the default irc channel"""
		self.send_notice_dft_channel( line )

	#
	# IRC Protocol definition
	#
	def join_channel( self, channel ):
		self.irc_socket.send( 'JOIN '+channel+'\r\n' )
	
	def send_msg( self, channel, msg ):
		self.irc_socket.send( 'PRIVMSG ' + channel + ' :' + msg + '\r\n' )
	
	def send_msg_dft_channel( self, msg ):
		self.irc_socket.send( 'PRIVMSG ' + DFT_CHANNEL + ' :' + msg + '\r\n' )

	def send_action_msg_dft_channel( self, msg ):
		"""/me like message"""
		self.irc_socket.send( 'PRIVMSG ' + DFT_CHANNEL + ' :\01ACTION ' + msg + '\r\n' )
	
	def send_notice_dft_channel( self, msg ):
		print "send_notice_dft_channel: ",msg
		self.irc_socket.send( 'NOTICE ' + DFT_CHANNEL + ' :' + msg + '\r\n' )

	def quit( self, msg="" ):
		self.irc_socket.send( 'QUIT' + ' :' + msg + '\r\n' )
		self.irc_socket.close()
		print msg


class MyTCPServer( SocketServer.ThreadingTCPServer ):
	""" server """
	def server_bind( self ):
		self.socket.setsockopt( socket.SOL_SOCKET, socket.SO_REUSEADDR, 1 )
		self.socket.bind( self.server_address )


class Handler( SocketServer.StreamRequestHandler ):
	""" handles every conection """
	rbufsize = 1
	wbufsize = 1

	def handle( self ):
		for line in self.rfile:
			line = line.strip()
			if line:
				if line=="quit": return #socket close
				MessageQ.put( line )



IRC_Connection().start()
server = MyTCPServer( ("localhost", 2222), Handler )
server.serve_forever()
