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

#ifndef _ScaleCnv_
#define _ScaleCnv_

#include "Array.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"

namespace CLAM {

template <class T> class ScaleCnv
{
public:

/* Constructors and destructors */
	ScaleCnv(){};
	~ScaleCnv(){};

/* Converting routines */
	void Convert(DataArray<T>& inputArray,
		DataArray<T>& outputArray,EScaleType oScaleType=eUnknown)// non-destructive overload
	{
		if(oScaleType==0) oScaleType=outputArray.GetScale();
		if(inputArray.GetScale()!=oScaleType)
		{
			if(inputArray.GetScale()==eLinear)
			{
				ConvertTodB(inputArray,outputArray,(int)oScaleType);
			}
			else if(oScaleType==eLinear)
			{
				ConvertToLin(inputArray,outputArray,inputArray.GetScale());
			}
			else
			{
				double factor = ((double)oScaleType)/((double)inputArray.GetScale());
				for(int i=0;i<inputArray.Size();i++)
				{
					outputArray[i]=inputArray[i]*factor;
				}
			}
		}
		if(outputArray.GetScale() != oScaleType) outputArray.SetScale(oScaleType);
	}

	void Convert(DataArray<T>& array,EScaleType oDataType)//destructive overload
	{
		Convert(array,array,oDataType);
	}

	/* Scale convert for a Spectrum */
	void Convert(Spectrum &inputSpectrum, 
				 Spectrum &outputSpectrum, EScaleType oScaleType=eUnknown)
	{
		if(oScaleType==0) oScaleType=outputSpectrum.GetScale();
		CLAM_ASSERT(inputSpectrum.GetSpectrum() == eMagPhase,
			"ScaleConv::Convert: spectrum must be eMagPhase"); 
		// Other Spectrum types should be implemented

		Convert(inputSpectrum.GetRefToMagBuffer(), outputSpectrum.GetRefToMagBuffer(), oScaleType);
		outputSpectrum.SetScale(oScaleType);
	}
	
	void Convert(Spectrum &spectrum, EScaleType oScaleType)
	{
		Convert(spectrum,spectrum,oScaleType);
	}

	/* Scale convert for a Peak Array */
	void Convert(SpectralPeakArray &inputPeakArray, 
				 SpectralPeakArray &outputPeakArray, EScaleType oScaleType=eUnknown)
	{
		if(oScaleType==0) oScaleType=outputPeakArray.GetScale();	 
		Convert(inputPeakArray.GetRefToPeakArray(), outputPeakArray.GetRefToPeakArray(), oScaleType);
		outputPeakArray.SetScale(oScaleType);
	}
	
	void Convert(SpectralPeakArray &spectrum, EScaleType oScaleType)
	{
		Convert(spectrum,spectrum,oScaleType);
	}
			
private:
		
	void ConvertTodB(const DataArray<T>& inputArray,
		DataArray<T>& outputArray,int scalingFactor)//non-destructive dB conv.
	{
		outputArray=inputArray;
		ConvertTodB(outputArray,scalingFactor);
	}
	
	void ConvertTodB(DataArray<T>& array,int scalingFactor)//destructive dB conv.
	{
		array.Apply(DB,(scalingFactor));
	}

	void ConvertToLin(const Array<T>& inputArray,
		Array<T>& outputArray,int scalingFactor)//non-destructive lin conv.
	{
		outputArray=inputArray;
		ConvertToLin(outputArray,scalingFactor);
	}

	void ConvertToLin(Array<T>& array,int scalingFactor)//destruct. lin. conv.
	{
		array.Apply(Lin,scalingFactor);
	}
	
};

}

#endif

