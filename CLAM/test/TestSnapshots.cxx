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

#include <iostream>
#include <cstdlib>
#include <exception>
using std::cout;
using std::endl;
using std::cerr;
#include <string>
#include "FFT_rfftw.hxx"
#include "DataTypes.hxx"
#include "ErrProcessingObj.hxx"

//#include "DebugSnapshots.hxx"
#include "Plot.hxx"

//using CLAMVM::WidgetTKWrapper;
//using CLAMVM::Geometry;

using namespace CLAM;

int main(int argc, char* argv[])
{
	try {
		
		int i,Size=1024;
		float SampleRate=8000.0;
		

		// Audio creation
		CLAM::Audio myaudio;
		myaudio.SetSize( Size );
		for (i=0;i<Size;i++)
			myaudio.GetBuffer()[i]=0.5*sin(2.0*PI*400.0*(((float)i)/SampleRate));

		// Spectrum attribute selection and config
		CLAM::SpecTypeFlags sflags;
		sflags.bMagPhase=true;
		sflags.bComplex=true;
		
		CLAM::SpectrumConfig sconfig;
		sconfig.SetType(sflags);
		sconfig.SetSize(Size/2+1);
		
		// Spectrum creation
		CLAM::Spectrum myspectrum(sconfig);
		std::cout << myspectrum.GetComplexArray()[0] << std::endl;
		
		// Processing object configuration
		CLAM::FFTConfig fconfig;
		fconfig.SetName(std::string("local.My_beautiful_fft"));
		fconfig.SetAudioSize(Size);
		
		// Processing object creation
		CLAM::FFT_rfftw myfft(fconfig);
		
		// Processing object execution
		std::cout << "Running object " << std::endl;
		myfft.Start();
		myfft.Do(myaudio,myspectrum);
		
		std::cout << "Test commences" << std::endl;
		std::cout << "You should get ten (10) consecutive fully functional snapshots of" << std::endl;
		std::cout << "the input audio. Otherwise please submit a bug report to Mantis ASAP." << std::endl;
		std::cout << "Audio Snapshot test" << std::endl;

		for ( i = 0; i < 10; i++ )
			{
				std::cout << "Opening Audio snapshot" << std::endl;
				CLAMVM::plot( myaudio );
				std::cout << "Closing Audio snapshot" << std::endl;
			} 
		
		std::cout << "You should get ten (10) consecutive fully functional snapshots of" << std::endl;
		std::cout << "the fft output spectrum. Otherwise please submit a bug report to Mantis ASAP." << std::endl;
		std::cout << "Spectrum Snapshot test" << std::endl;

		for ( i = 0; i < 10; i++ )
			{
				std::cout << "Opening spectrum snapshot" << std::endl;
				CLAMVM::plot( myspectrum );
				std::cout << "Closing Spectrum Snapshot" << std::endl;
			}
		
	}
	catch(Err& err) 
		{
			err.Print();
			abort();
		}
	catch ( std::exception& e )
		{
			std::cerr << "Standard library exception caught dump follows:" << std::endl;
			std::cerr << e.what() << std::endl;
			abort();
		}
	catch (...)
		{
			std::cerr << "Unknown exception caught. If you are using Windows and your" << std::endl;
			std::cerr << "binary is in debug mode probably there is some memory corruption somewhere"<< std::endl;

			abort();
		}
	
	return 0;
	
}
