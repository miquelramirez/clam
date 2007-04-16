/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/** @file UserTutorial5.cxx CLAM tutorial, example 5.
 *  Ilustrates how to use a simple processing object.
 */
#include <CLAM/FFT.hxx>
#include <CLAM/FFTConfig.hxx>
#include <CLAM/SpectrumConfig.hxx>
#include <iostream>
#include <cmath>

int main()
{
	int i,Size=1024;
	float SampleRate=8000.0;

	// Audio creation
	CLAM::Audio myaudio;
	myaudio.SetSize(Size);
	for (i=0;i<Size;i++)
		myaudio.GetBuffer()[i]=
			0.625+0.5*sin(2.0*PI*400.0*(((float)i)/SampleRate));

	// Spectrum settings
	CLAM::SpectrumConfig ssettings;
	ssettings.GetType().bMagPhase=false;
	ssettings.GetType().bComplex=true;
	ssettings.SetSize(Size/2+1);

	// Spectrum creation
	CLAM::Spectrum myspectrum(ssettings);

	// Processing object configuration
	CLAM::FFTConfig fconfig;
	fconfig.SetAudioSize(Size);

	// Processing object creation
	CLAM::FFT myfft(fconfig);

	// Processing object execution
	std::cout << "Running object " << std::endl;

	// This puts the object in execution mode.
	myfft.Start();

	// And this performs the computation.
	myfft.Do(myaudio,myspectrum);

	return 0;
}

