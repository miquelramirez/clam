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
#include "SpecTypeFlags.hxx"
#include "SpectralPeakArrayInterpolator.hxx"
#include "ErrProcessingObj.hxx"
#include "BPF.hxx"
#include "Point.hxx"

namespace CLAM {

	void PeaksInterpConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}


	SpectralPeakArrayInterpolator::SpectralPeakArrayInterpolator()
		: mSize(0),
		  mIn1("Input 1",this,1),
		  mIn2("Input 2",this,1),
		  mOut("Output",this,1),
		  mMagInterpolationFactorCtl("MagInterpolationFactor",this),
		  mFreqInterpolationFactorCtl("FreqInterpolationFactor",this),
		  mPitchInterpolationFactorCtl("PitchInterpolationFactor",this),
		  mIsHarmonicCtl("IsHarmonic",this),
		  mPitch1Ctl("Pitch1",this),
		  mPitch2Ctl("Pitch2",this)
	{
		Configure(PeaksInterpConfig());
	}

	SpectralPeakArrayInterpolator::SpectralPeakArrayInterpolator(const PeaksInterpConfig &c)
		: mSize(0),
		  mIn1("Input 1",this,1),
		  mIn2("Input 2",this,1),
		  mOut("Output",this,1),
		  mMagInterpolationFactorCtl("MagInterpolationFactor",this),
		  mFreqInterpolationFactorCtl("FreqInterpolationFactor",this),
		  mPitchInterpolationFactorCtl("PitchInterpolationFactor",this),
		  mIsHarmonicCtl("IsHarmonic",this),
		  mPitch1Ctl("Pitch1",this),
		  mPitch2Ctl("Pitch2",this)
	{
		Configure(c);
	}


	bool SpectralPeakArrayInterpolator::ConcreteConfigure(const ProcessingConfig&c)
	{
		CopyAsConcreteConfig(mConfig, c);
		//Initialize interpolation factor control from value in the configuration
		mMagInterpolationFactorCtl.DoControl(mConfig.GetMagInterpolationFactor());
		mFreqInterpolationFactorCtl.DoControl(mConfig.GetFreqInterpolationFactor());
		mPitchInterpolationFactorCtl.DoControl(mConfig.GetPitchInterpolationFactor());
		mIsHarmonicCtl.DoControl(mConfig.GetHarmonic());

		return true;
	}

	// Unsupervised Do() function.
	bool SpectralPeakArrayInterpolator::Do(const SpectralPeakArray& in1, const SpectralPeakArray& in2, SpectralPeakArray& out)
	{
		CLAM_DEBUG_ASSERT(GetExecState() != Unconfigured &&
		                  GetExecState() != Ready,
		                  "SpectralPeakArrayInterpolator::Do(): Not in execution mode");

		if (GetExecState() == Disabled)
			return true;

		//we need to copy input peak arrays to convert them to linear
		SpectralPeakArray tmpIn1=in1;
		SpectralPeakArray tmpIn2=in2;
		tmpIn1.ToLinear();
		tmpIn2.ToLinear();

		int nPeaks1=in1.GetnPeaks();
		int nPeaks2=in2.GetnPeaks();

		if(nPeaks1==0)
		{
			out=in1;
			return true;
		}
		if(nPeaks2==0)
		{
			out=in1;
			return true;
		}

		//We initialize out with tmpIn1
		out=tmpIn1;
		
		DataArray& in1Mag=tmpIn1.GetMagBuffer();
		DataArray& in2Mag=tmpIn2.GetMagBuffer();
		DataArray& outMag=out.GetMagBuffer();

		DataArray& in1Freq=tmpIn1.GetFreqBuffer();
		DataArray& in2Freq=tmpIn2.GetFreqBuffer();
		DataArray& outFreq=out.GetFreqBuffer();

		IndexArray& in1Index=tmpIn1.GetIndexArray();
		IndexArray& in2Index=tmpIn2.GetIndexArray();
		
		
		
		TData factor2=(TData)nPeaks2/nPeaks1;
		
		TData magFactor=mMagInterpolationFactorCtl.GetLastValue();
		TData freqFactor=mFreqInterpolationFactorCtl.GetLastValue();

		TData pitch1=mPitch1Ctl.GetLastValue();
		TData pitch2=mPitch2Ctl.GetLastValue();
		TData pitchFactor=mPitchInterpolationFactorCtl.GetLastValue();

		//TODO: this computation is duplicated
		TData newPitch=pitch1*(1-pitchFactor)+pitch2*pitchFactor;

		int pos=0,i=0;
		TData lastFreq=0;
		do
		{
			if(!mIsHarmonicCtl.GetLastValue())
			{
				outMag[i]=in1Mag[i]*(1-magFactor)+in2Mag[i*factor2]*magFactor;
				outFreq[i]=in1Freq[i]*(1-freqFactor)+in2Freq[i*factor2]*freqFactor;
				CLAM_DEBUG_ASSERT(outFreq[i]>=lastFreq,"Error");
				lastFreq=outFreq[i];
				CLAM_DEBUG_ASSERT(outMag[i]<1,"Error");
				CLAM_DEBUG_ASSERT(outMag[i]>-1,"Error");
				CLAM_DEBUG_ASSERT(outFreq[i]<22000,"Error");
				CLAM_DEBUG_ASSERT(outFreq[i]>=0,"Error");
			}
			else if(FindHarmonic(in2Index,in1Index[i],pos))
			{
				//Morphing Using Harmonic No*/
				outMag[i]=in1Mag[i]*(1-magFactor)+in2Mag[pos]*magFactor;
				outFreq[i]=((in1Freq[i]/pitch1)*(1-freqFactor)+(in2Freq[pos]/pitch2)*freqFactor)*newPitch;
				CLAM_DEBUG_ASSERT(outFreq[i]>lastFreq,"Error");
				lastFreq=outFreq[i];
				CLAM_DEBUG_ASSERT(outMag[i]<1,"Error");
				CLAM_DEBUG_ASSERT(outMag[i]>-1,"Error");
				CLAM_DEBUG_ASSERT(outFreq[i]<22000,"Error");
				CLAM_DEBUG_ASSERT(outFreq[i]>=0,"Error");
			}
			else
			{
				if(i>in2.GetnPeaks()-1)
				{
					out.SetnPeaks(i-1);
					break;
				}
				outMag[i]=0.0000000001;
				outFreq[i]=lastFreq+=100;
			}
			i++;
		}while(i<nPeaks1);
		
		//Finally we convert output to dB
		out.TodB();

		return true;
	}

	bool SpectralPeakArrayInterpolator::Do(void)
	{
		throw(ErrProcessingObj("SpectralPeakArrayInterpolator::Do(): Not implemented"),this);
	}

	
	/** TODO: This method does not take into account a possible change in fundamental!!!*/
	bool SpectralPeakArrayInterpolator::FindHarmonic(const IndexArray& indexArray,int index,int& lastPosition)
	{
		int i;
		bool found=false;
		int nPeaks=indexArray.Size();
		for(i=lastPosition;i<nPeaks;i++)
		{
			if(indexArray[i]==index)
			{
				lastPosition=i;
				found=true;
				break;
			}
		}
		return found;

	}
}