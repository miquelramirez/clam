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
// Test for Spectrum
//
// written by Xavier Amatriain - June 2001
////////////////////////////////////////////////////////////////////


#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "GlobalEnums.hxx"
#include "XMLStorage.hxx"
#include <iostream>




int test1() {
	const unsigned spectrumSize=100;
	const unsigned spectralRange=22050;

	try
	{
		std::cout << "Spectrum Test" << std::endl;
		CLAM::Array<CLAM::Complex> *pData=new CLAM::Array<CLAM::Complex>(spectrumSize);
		for (int i=0; i<spectrumSize; i++)
			pData->AddElem(CLAM::Complex(2,5) );

/*****Constructing diferent types of spectrums*****/

//Default Spectrum (size=0,type=MagPhase,spectralrng=22050,scale=linear)

		CLAM::Spectrum defaultSpec;

//Explicitly changing config
		CLAM::SpecTypeFlags specFlags;
		specFlags.bComplex=1;
		CLAM::Spectrum complex;
		complex.SetType(specFlags);
		complex.SetSize(spectrumSize);
		complex.SetSpectralRange(spectralRange);
		complex.SetScale(CLAM::EScale::eLog);

//From already existing config

		CLAM::SpectrumConfig myLoadedConfig; //Supposed to be loaded from file
		myLoadedConfig.SetSize(spectrumSize);
		myLoadedConfig.SetType(specFlags);
		myLoadedConfig.SetSpectralRange(spectralRange);
		myLoadedConfig.SetScale(CLAM::EScale::eLog);

		CLAM::Spectrum loadedSpec(myLoadedConfig);


/*****Setting a buffer*****/
		complex.SetComplexArray(*pData);
		/*
		 * Important!! right now you cannot pass a reference in this function: 
		 * by doing a SetBuffer	you are granting delete privileges to the owner 
		 * object
		 */


/*****Using the spectrum (inefficiently)*****/

		std::cout << "Spectral Range: " << complex.GetSpectralRange() << std::endl;

		// Set values
		for (int i=0; i<spectrumSize; i++) {
			complex.SetMag(i,i);
			complex.SetPhase(i,i+1);
		}

		// Get values
		for (int i=0; i<spectrumSize; i++) {
			std::cout 
				<< "Mag[" << i << "] = " << complex.GetMag(i) << ", " 
				<< "Phase[" << i << "] = " << complex.GetPhase(i) << std::endl;
		}


/*****Using the spectrum (efficiently)*****/
		//Resizing spectrum
		defaultSpec.SetSize(spectrumSize);

		CLAM::DataArray &mag=defaultSpec.GetMagBuffer();
		CLAM::DataArray &phase=defaultSpec.GetPhaseBuffer();
		// Set values
		for (int i=0; i<spectrumSize; i++) {
			mag[i]=i;
			phase[i]=i+1;
		}
		// Get values
		mag=defaultSpec.GetMagBuffer();
		phase=defaultSpec.GetPhaseBuffer();
		for (int i=0; i<spectrumSize; i++) {
			std::cout 
				<< "Mag[" << i << "] = " << mag[i] << ", " 
				<< "Phase[" << i << "] = " << phase[i] << std::endl;
		}

/*****Using conversion routines*****/
		CLAM::SpecTypeFlags newType;

		/*converting magphase to complex keeping original data*/
		newType.bMagPhase=newType.bComplex=1;
		defaultSpec.SetTypeSynchronize(newType);

		/*converting complex to magphaseBPF, deleting original data*/
		newType.bComplex=newType.bMagPhase=0;
		newType.bMagPhaseBPF=1;
		complex.SetTypeSynchronize(newType);

/*****Storing result to XML document*****/ 
		CLAM::XMLStorage::Dump(complex, "ComplexSpectrum", "SpectrumExample.1.xml");
		CLAM::XMLStorage::Dump(defaultSpec, "DefaultSpectrum", "SpectrumExample.2.xml");
		CLAM::XMLStorage::Dump(loadedSpec, "LoadedSpectrum", "SpectrumExample.3.xml");





	}
 
	catch(CLAM::Err error)
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



