#!/usr/bin/python
# -*- coding: UTF-8 -*-

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

from clam import *
import sys
import time

if len(sys.argv)!=2:
	print "needs a network filename."
	sys.exit(-1)

def error(msg):
	print msg
	return -1

def main(args):
	filename = sys.argv[1]
	network = Network()
	
	try:
		XmlStorage.Restore( toComponent(network), filename )
	except RuntimeError:
		return error( "Could not open the network file" )
	
	# Set the audio backend to PortAudio
	portaudio_player = PANetworkPlayer()
	network.SetPlayer( portaudio_player.real() ) #FIXME
	
	network.Start()
	time.sleep(4) #TODO: until?
	network.Stop()

if __name__ == '__main__':
	sys.exit( main(sys.argv) )
