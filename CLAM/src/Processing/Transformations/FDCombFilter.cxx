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

#include "Complex.hxx"
#include "FDCombFilter.hxx"

#define CLASS "FDCombFilter"

namespace CLAM {


	/* Processing  object Method  implementations */

	FDCombFilter::FDCombFilter(const Config &c)
		: mFreq("Frequency",this)
	{
		mFreq.DoControl(0);
		Configure(c);
	}

	FDCombFilter::~FDCombFilter()
	{}


	/* The supervised Do() function */

	bool  FDCombFilter::Do() 
	{
		CLAM_ASSERT(false,CLASS"::Do(): Supervised mode not implemented");
		return false;
	}

	
	/* The  unsupervised Do() function */
	bool  FDCombFilter::Do(const Spectrum& input, Spectrum& output)
	{
		if (mFreq.GetLastValue() <= 0) return false;

		output.SetSize(input.GetSize());
		Spectrum tmpSpec=input;
		bool wasDB=false;
		if(output.GetScale()==EScale::eLog)
		{
			wasDB=true;
			output.SetScale(EScale::eLinear);
		}
		
		tmpSpec.ToLinear();

		
		TData samplingRate=input.GetSpectralRange()*2;
		const TSize sizeSpectrum=input.GetSize();
		TData pitch=mFreq.GetLastValue();
		TData period = pitch /	samplingRate * sizeSpectrum * 2.0; 
		DataArray & inputMag = input.GetMagBuffer();
		DataArray & inputPhase = input.GetPhaseBuffer();
		DataArray & outputMag = output.GetMagBuffer();
		DataArray & outputPhase = output.GetPhaseBuffer();
		
		TData twoPiOverPeriod = TWO_PI/period;
		TData oneOverTwo = 1./2.0;
		for(unsigned i=0; i<sizeSpectrum; i++)
		{
			//todo: this loop is very inefficient because of the sin and cos but there are ways of optimizing
			//these kind of iterative sine computations
			TData combReal = (1.f +CLAM_cos(i*twoPiOverPeriod)) * oneOverTwo;
			TData combImag = (1.f -CLAM_sin(i*twoPiOverPeriod)) * oneOverTwo;
			
			TData mag=inputMag[i];
			TData phase=inputPhase[i];
			TData real=mag*CLAM_cos(phase);
			TData imag=mag*CLAM_sin(phase);

			TData newReal=real*combReal-imag*combImag;
			TData newImag=real*combImag+imag*combReal;
			TData newMag=CLAM_sqrt(newReal*newReal+newImag*newImag);
			TData newPhase=CLAM_atan2(newImag,newReal);
			
			outputMag[i] = newMag;
			outputPhase[i] = newPhase;
		}

		if(wasDB) output.ToDB();

		return true;
	}


};//namespace CLAM

