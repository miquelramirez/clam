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

//////////////////////////////////////////////////////////////////
// Test for FDFilterGenerator
//
// written by Xavier Amatriain - July 2001
// modified by Miguel Ramirez - September 2003
////////////////////////////////////////////////////////////////////

#include "Spectrum.hxx" // imports CLAM::Spectrum declaration
#include "SpectrumConfig.hxx" // imports CLAM::SpectrumConfig declaration
#include "FDFilterGen.hxx" // imports CLAM::FDFilterGen declaration
#include <iostream>
#include <exception>

int main( int argc, char** argv )
{
	try // Note the exception handling
	{
		// Since we are interested in frequency domain filtering we define the
		// H(w) - the filter frequency response envelope. This envelope will be
		// provided as a CLAM::Spectrum by the CLAM::FDFilterGen Processing object, 
		// so filtering in frequency domain using CLAM brokes down to the following
		// steps:
		// 1) Define H(w) shape using FDFilterGenConfig class parameters
		// 2) Configure the FDFilterGen Processing object
		// 3) Generate H(w) with the configured FDFilterGen object
		// 4) Multiply H(w) by X(w), the fourier transform of the signal we
		//    want to filter


		// We first delare the configuration class for the Processing
		// of interest
		CLAM::FDFilterGenConfig myFilterGeneratorConfig;

		// Filter kind setup. There are three possible kinds: low pass ( eLowPass ),
		// band pass ( eBandPass ) and high pass ( eHighPass ). This 'flag' defines
		// how the FDFilterGen Processing will interpret the config object values.
		myFilterGeneratorConfig.SetType(CLAM::EFDFilterType::eLowPass);
		// Spectral range setup. Must match with the spectral range of the signal
		// to be filtered.
		myFilterGeneratorConfig.SetSpectralRange(22050);
		// Sets maximum filter gain in dB. In this case, the maximum gain will
		// be of just a dB.
		myFilterGeneratorConfig.SetGain(1);
		// Sets the frequency where the filter gain will begin to decrease. In this
		// case it is 1000 Hz.
		myFilterGeneratorConfig.SetLowCutOff(1000);
		// Sets the filter gain decay slope. In this is case it is set to 3 db/octave.
		myFilterGeneratorConfig.SetStopBandSlope(3);

		// We declare the FDFilterGen Processing object
		CLAM::FDFilterGen myFilterGenerator;

		// We configure it...
		myFilterGenerator.Configure( myFilterGeneratorConfig );

		// We start it...
		myFilterGenerator.Start();

		// Now we declare and setup the CLAM::Spectrum that will host the filter
		// frequency response
		CLAM::Spectrum mySpec;
		CLAM::SpecTypeFlags specFlags;

		// We want the spectrum to be defined as a BPF ( break point function )
		specFlags.bMagPhaseBPF=1;
		mySpec.SetType(specFlags);
		// We setup the CLAM::Spectrum object spectral range
		mySpec.SetSpectralRange(22050);
		// We tell the spectrum object that the magnitude values
		// are specified in dB's
		mySpec.SetScale(CLAM::EScale::eLog);

		// We generate the filter envelope
		myFilterGenerator.Do(mySpec);


		// Now we can see the filter envelope we just generated. For each
		// point the 'x' value is frequency, and the 'y' value is magnitude
		// gain in dB's
		CLAM::BPF &bpf = mySpec.GetMagBPF();

		std::cout << bpf.Size() << " points BPF:" << std::endl;
		
		for (int i=0; i<bpf.Size(); i++)
		{
			std::cout << "  " << bpf.GetXValue(i);
			std::cout << ", " << bpf.GetValueFromIndex(i) << std::endl;
		}

	}
	catch ( CLAM::Err& e) // First, we check wether the raised exception was a 'CLAM exception'
	{
		e.Print();
		exit(-1);
	}
	catch ( std::exception& e ) // If not a CLAM exception, we check if it is a standard
								// library exception
	{
		std::cerr << e.what() << std::endl;
		exit(-1);
	}

	return 0;

}
	
