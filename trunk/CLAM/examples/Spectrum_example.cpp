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

//////////////////////////////////////////////////////////////////
// Test for Spectrum
//
// written by Xavier Amatriain - June 2001
////////////////////////////////////////////////////////////////////


#define SIZE 100
#define SPECTRALRANGE 22050


#include "Spectrum.hxx"

//! Aixo es per provar la sortida xml
#include <fstream>
//typedef std::basic_fstream<char> std::fstream;

#ifdef CLAM_USE_XML
#	include "XMLStorage.hxx"
#	include "XMLStaticAdapter.hxx"
#	include "XMLComponentAdapter.hxx"
#	include "XMLable.hxx"
#endif//CLAM_USE_XML

using namespace CLAM;

#include "Complex.hxx"




int test1() {
	int i;

	try
		{
		fprintf(stdout, "Spectrum Test\n");
		Array<Complex_> *pData=new Array<Complex_>(SIZE);
		for (i=0; i<SIZE; i++)
			pData->AddElem(Complex<TData>(2,5) );

/*****Constructing diferent types of spectrums*****/

//Default Spectrum (size=0,type=MagPhase,spectralrng=22050,scale=linear)

		Spectrum defaultSpec;

//Explicitly changing config
		SpecTypeFlags specFlags;
		specFlags.bComplex=1;
		Spectrum complex;
		complex.SetType(specFlags);
		complex.SetSize(SIZE);
		complex.SetSpectralRange(SPECTRALRANGE);
		complex.SetScale(EScale::eLog);

//From already existing config

		SpectrumConfig myLoadedConfig; //Supposed to be loaded from file
		myLoadedConfig.SetSize(SIZE);
		myLoadedConfig.SetType(specFlags);
		myLoadedConfig.SetSpectralRange(SPECTRALRANGE);
		myLoadedConfig.SetScale(EScale::eLog);

		Spectrum loadedSpec(myLoadedConfig);


/*****Setting a buffer*****/
		complex.SetComplexArray(*pData);
		/*
		 * Important!! right now you cannot pass a reference in this function: 
		 * by doing a SetBuffer	you are granting delete privileges to the owner 
		 * object
		 */


/*****Using the spectrum (inefficiently)*****/

		printf("Spectral Range: %f\n", complex.GetSpectralRange());

		// Set values
		for (i=0; i<SIZE; i++) {
			complex.SetMag(i,i);
			complex.SetPhase(i,i+1);
		}

		// Get values
		for (i=0; i<SIZE; i++) {
			printf("Mag[%d] = %f, Phase[%d] = %f \n", i, complex.GetMag(i), i, complex.GetPhase(i) );
		}


/*****Using the spectrum (efficiently)*****/
		//Resizing spectrum
		defaultSpec.SetSize(SIZE);

		DataArray &mag=defaultSpec.GetMagBuffer();
		DataArray &phase=defaultSpec.GetPhaseBuffer();
		// Set values
		for (i=0; i<SIZE; i++) {
			mag[i]=i;
			phase[i]=i+1;
		}
		// Get values
		mag=defaultSpec.GetMagBuffer();
		phase=defaultSpec.GetPhaseBuffer();
		for (i=0; i<SIZE; i++) {
			printf("Mag[%d] = %f, Phase[%d] = %f \n", i, mag[i], i, phase[i] );
		}

/*****Using conversion routines*****/
		SpecTypeFlags newType;

		/*converting magphase to complex keeping original data*/
		newType.bMagPhase=newType.bComplex=1;
		defaultSpec.SetTypeSynchronize(newType);

		/*converting complex to magphaseBPF, deleting original data*/
		newType.bComplex=newType.bMagPhase=0;
		newType.bMagPhaseBPF=1;
		complex.SetTypeSynchronize(newType);

/*****Storing result to XML document*****/ 
#ifdef CLAM_USE_XML
		XMLStorage storage;
		storage.Dump(complex, "ComplexSpectrum", "SpectrumExample.1.xml");
		storage.Dump(defaultSpec, "DefaultSpectrum", "SpectrumExample.2.xml");
		storage.Dump(loadedSpec, "LoadedSpectrum", "SpectrumExample.3.xml");
#endif//CLAM_USE_XML





	}
 
	catch(Err error)
	{
		error.Print();
	}
	return 0;
}


#include <exception>
int main()
{


	//int i;

	int ret;
	ret=test1();
	return ret;
};



