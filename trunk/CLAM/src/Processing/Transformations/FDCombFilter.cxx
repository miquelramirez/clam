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

#include "Complex.hxx"
#include "FDCombFilter.hxx"
#include "ErrProcessingObj.hxx"

#define CLASS "FDCombFilter"

namespace CLAM {

	/* The  Configuration object has at least to have a name */

	void FDCombFilterConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		DefaultValues();
	}

	
	void FDCombFilterConfig::DefaultValues()
	{
		
	}


	/* Processing  object Method  implementations */

	FDCombFilter::FDCombFilter():mFreq("Frequency",this)
	{
		mFreq.DoControl(0);
		Configure(FDCombFilterConfig());
	}

	FDCombFilter::FDCombFilter(const FDCombFilterConfig &c = FDCombFilterConfig()):mFreq("Frequency",this)
	{
		mFreq.DoControl(0);
		Configure(c);
	}

	FDCombFilter::~FDCombFilter()
	{}


	/* Configure the Processing Object according to the Config object */

	bool FDCombFilter::ConcreteConfigure(const ProcessingConfig& c)
	{

		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	/* Setting Prototypes for faster processing */

	bool FDCombFilter::SetPrototypes(const Spectrum& input,Spectrum& output)
	{
		return true;
	}

	bool FDCombFilter::SetPrototypes()
	{
		return true;
	}
	
	bool FDCombFilter::UnsetPrototypes()
	{
		return true;
	}

	/* The supervised Do() function */

	bool  FDCombFilter::Do(void) 
	{
		CLAM_ASSERT(false,CLASS"::Do(): Supervised mode not implemented");
		return false;
	}

	
	/* The  unsupervised Do() function */
	bool  FDCombFilter::Do(const Spectrum& input, Spectrum& output)
	{
		if (mFreq.GetLastValue() > 0)
		{
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
			TSize sizeSpectrum=input.GetSize();
			TData pitch=mFreq.GetLastValue();
			TData period = pitch /	samplingRate * sizeSpectrum * 2.0; 
			int i;
			for(i=0; i<sizeSpectrum; i++)
			{
				TData combReal = (1 +cos(i/period * TWO_PI)) / 2.0;
				TData combImag = (1 -sin(i/period * TWO_PI)) / 2.0;
				
				TData mag=input.GetMag(i);
				TData phase=input.GetPhase(i);
				TData real=mag*cos(phase);
				TData imag=mag*sin(phase);

								
				TData newReal=real*combReal-imag*combImag;
				TData newImag=real*combImag+imag*combReal;
				TData newMag=sqrt(newReal*newReal+newImag*newImag);
				TData newPhase=atan2(newImag,newReal);

				
				
				output.SetMag(i,newMag);
				output.SetPhase(i,newPhase);
			}

			if(wasDB) output.ToDB();
		

			return true;
		}
		else return false;
	}


};//namespace CLAM
