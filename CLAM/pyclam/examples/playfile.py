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

filename = "./test4seg.mp3"

network = Network()

reader = network.AddProcessing( "MonoAudioFileReader" )
config = MonoAudioFileReaderConfig()
config.SetSourceFile( filename )

if not network.ConfigureProcessing( reader, config.real() ): #FIXME
	print "Could not open the file"

sink = network.AddProcessing( "AudioSink" )

network.ConnectPorts( reader+".Samples Read", sink+".AudioIn" )

portaudio_player = PANetworkPlayer()
network.SetPlayer( portaudio_player.real() ) #FIXME

network.Start()
time.sleep( MonoAudioFileReader( network.GetProcessing(reader) ).GetLength() )
network.Stop()
