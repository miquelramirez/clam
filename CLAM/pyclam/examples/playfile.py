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
import time
import sys

if len(sys.argv)==1: # No input file
	print "No input file."
	print "Usage: playfile.py <input file name>"
	sys.exit(-1)
elif len(sys.argv)>2: #Too many parameters
	print "Too many parameters"
	print "Usage: playfile.py <input file name>"
	sys.exit(-1)


def main(args):
	filename = sys.argv[1]

	network = Network()
	
	reader = network.AddProcessing( "MonoAudioFileReader" )
	config = MonoAudioFileReaderConfig()
	config.SetSourceFile( filename )
	
	if not network.ConfigureProcessing( reader, toProcessingConfig(config) ): #FIXME
		print "Could not open the file"
	
	sink = network.AddProcessing( "AudioSink" )
	
	network.ConnectPorts( reader+".Samples Read", sink+".AudioIn" )

	portaudio_player = PANetworkPlayer()
	network.SetPlayer( portaudio_player.real() ) #FIXME
	
	network.Start()
	time.sleep( int( network.GetProcessing(reader).GetHeader().GetLength()/1000) )
	network.Stop()

	return 0
#main()

if __name__ == '__main__':
	sys.exit( main(sys.argv) )
