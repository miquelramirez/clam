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
from math import sin

samples = 1024
sampleRate = 8000.0
sineFreq = 400.0

myaudio = Audio()
myaudio.SetSize( samples );
myaudio.SetSampleRate( sampleRate );

vector = myaudio.GetBuffer()
for i in range(samples):
	vector.set( i, 0.625+0.5*sin(2.0*sineFreq*400.0*((float(i))/sampleRate)) )
	#print vector[i]

myspectrum = Spectrum()
specFlags = SpecTypeFlags()

specFlags.SetFlag( specFlags.GetFlagPosition("Polar"), 1 )
specFlags.SetFlag( specFlags.GetFlagPosition("Complex"), 1 )
specFlags.SetFlag( specFlags.GetFlagPosition("MagPhase"), 1 )
specFlags.SetFlag( specFlags.GetFlagPosition("Polar"), 1 )
specFlags.SetFlag( specFlags.GetFlagPosition("MagPhaseBPF"), 1 )

myspectrum.SetType( specFlags )
myspectrum.SetSize( samples/2+1 )

fconfig = FFTConfig()
fconfig.SetAudioSize( samples )

myfft = FFT_ooura() #FIXME: plain FFT() should work
myfft.Configure( fconfig.getReal() ) #FIXME
myfft.Start()

print "Running object "
myfft.Do( myaudio.getBase(), myspectrum.getBase() )

print "Storing spectrum"
XmlStorage.Dump( toComponent(myspectrum), "FFTResult", "pyFFT_example.xml" )
