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
// Test for FDFilterGenerator
//
// written by Xavier Amatriain - July 2001
////////////////////////////////////////////////////////////////////

#include "Spectrum.hxx"
#include "FDFilterGen.hxx"
#include <iostream>

using namespace CLAM;

int main()
{
	try {

	Spectrum mySpec;
	SpecTypeFlags specFlags;

	specFlags.bMagPhaseBPF=1;
	mySpec.SetType(specFlags);
	mySpec.SetSize(0);
	mySpec.SetSpectralRange(22100);
	mySpec.SetScale(EScale::eLog);

	
	FDFilterGenConfig myFilterGeneratorConfig;
	myFilterGeneratorConfig.SetName(std::string("exampleFilterGenerator"));
	myFilterGeneratorConfig.SetType(EFDFilterType(EFDFilterType::eLowPass));
	myFilterGeneratorConfig.SetSpectralRange(22050);
	myFilterGeneratorConfig.SetGain(1);
	myFilterGeneratorConfig.SetLowCutOff(1000);
	myFilterGeneratorConfig.SetStopBandSlope(3);

	FDFilterGen myFilterGenerator(myFilterGeneratorConfig);

	myFilterGenerator.Start();

	myFilterGenerator.Do(mySpec);

	BPF &bpf = mySpec.GetMagBPF();
	std::cout << bpf.Size() << " points BPF:" << std::endl;
	for (int i=0; i<bpf.Size(); i++)
		std::cout << "  " << bpf.GetXValue(i) << ", " << bpf.GetValueFromIndex(i) << std::endl;

	}
	catch (Err e) {
		e.Print();
		return 1;
	}


	return 0;

}
	
