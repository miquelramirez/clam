/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include<iostream>
#include<fstream>
#include<string>
#include"FFT_rfftw.hxx"
#ifdef CLAM_USE_XML
#include"XMLStorage.hxx"
#endif//CLAM_USE_XML
#include"DataTypes.hxx"
#include "ErrProcessingObj.hxx"

#include<cmath>

int main(int argc, char* argv[])
{
	try {

		int i,Size=1024;
		float SampleRate=8000.0;

		// Processing data declaration.
		CLAM::Audio myaudio;
		CLAM::Spectrum myspectrum;

		// Audio configuration and initialization.
		myaudio.SetSize(Size);
		for (i=0;i<Size;i++)
			myaudio.GetBuffer()[i]=0.625+0.5*sin(2.0*PI*400.0*(((float)i)/SampleRate));

		// Spectrum configuration. We want to use the cartesian representation
		// For the FFT.
		myspectrum.RemoveMagBuffer();
		myspectrum.RemovePhaseBuffer();
		myspectrum.AddComplexArray();
		myspectrum.UpdateData();

		// Now we change the size.
		myspectrum.SetSize(Size/2+1);

		// Processing object configuration.
		CLAM::FFTConfig fconfig;
		fconfig.SetName("local.My_beautiful_fft");
		fconfig.SetAudioSize(Size);

		// Processing object creation
		CLAM::FFT_rfftw myfft(fconfig);

		// Data conection
		myfft.Attach(myaudio,myspectrum);
		
		// End of Processing object configuration.
		myfft.Start();

		// Processing object execution
		std::cout << "Running object " << std::endl;
		myfft.Do();

#ifdef CLAM_USE_XML
		CLAM::XMLStorage storage;
		storage.Dump(myspectrum, "FFTResult", "FFT_example.xml");
#endif//CLAM_USE_XML

	}
	catch(CLAM::Err err) {
		err.Print();
	}

	return 0;

}
