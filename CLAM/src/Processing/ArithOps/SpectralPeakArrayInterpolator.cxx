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
#include "Spectrum.hxx"

namespace CLAM {

	void PeaksInterpConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		DefaultValues();
	}

	void PeaksInterpConfig::DefaultValues()
	{
		SetUseSpectralShape(false);
	}


	SpectralPeakArrayInterpolator::SpectralPeakArrayInterpolator()
		: mIn1("Input 1",this,1),
		  mIn2("Input 2",this,1),
		  mOut("Output",this,1),
		  mSpectralShape("SpectralShape",this,1),
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
		: mIn1("Input 1",this,1),
		  mIn2("Input 2",this,1),
		  mOut("Output",this,1),
		  mSpectralShape("SpectralShape",this,1),
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

		
		//First, we get values of the internal controls
		TData magFactor=mMagInterpolationFactorCtl.GetLastValue();
		TData freqFactor=mFreqInterpolationFactorCtl.GetLastValue();

		TData pitch1=mPitch1Ctl.GetLastValue();
		TData pitch2=mPitch2Ctl.GetLastValue();
		TData pitchFactor=mPitchInterpolationFactorCtl.GetLastValue();

		//we then chek if interpolation really needs to be done
		if(magFactor>0.99&&freqFactor>0.99&&pitchFactor>0.99)
		{
			//we return target spectral peak array
			out=in2;
			return true;
		}
		if(magFactor<0.01&&freqFactor<0.01&&pitchFactor<0.01)
		{
			//we return source spectral peak array
			out=in1;
			return true;
		}
		

		//else, it means we are in a intermediate point and we need to interpolate

		//we need to copy input peak arrays to convert them to linear
		SpectralPeakArray tmpIn1=in1;
		SpectralPeakArray tmpIn2=in2;
		tmpIn1.ToLinear();
		tmpIn2.ToLinear();

		int nPeaks1=in1.GetnPeaks();
		int nPeaks2=in2.GetnPeaks();

		if(nPeaks1==0||nPeaks2==0)
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
				outMag[i]=TData(0.0000000001);
				outFreq[i]=lastFreq+=100;
			}
			i++;
		}while(i<nPeaks1);
		
		//Finally we convert output to dB
		out.TodB();

		return true;
	}

	// Unsupervised Do() function.
	bool SpectralPeakArrayInterpolator::Do(const SpectralPeakArray& in1, const SpectralPeakArray& in2,const Spectrum& spectralShape, SpectralPeakArray& out)
	{
		CLAM_DEBUG_ASSERT(GetExecState() != Unconfigured &&
		                  GetExecState() != Ready,
		                  "SpectralPeakArrayInterpolator::Do(): Not in execution mode");

		if (GetExecState() == Disabled)
			return true;

		
		//First, we get values of the internal controls
		TData freqFactor=mFreqInterpolationFactorCtl.GetLastValue();

		TData pitch1=mPitch1Ctl.GetLastValue();
		TData pitch2=mPitch2Ctl.GetLastValue();
		TData pitchFactor=mPitchInterpolationFactorCtl.GetLastValue();

		//else, it means we are in a intermediate point and we need to interpolate

		//we need to copy input peak arrays to convert them to linear
		SpectralPeakArray tmpIn1=in1;
		SpectralPeakArray tmpIn2=in2;
		tmpIn1.ToLinear();
		tmpIn2.ToLinear();

		int nPeaks1=in1.GetnPeaks();
		int nPeaks2=in2.GetnPeaks();

		if(nPeaks1==0||nPeaks2==0)
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
		

		//TODO: this computation is duplicated
		TData newPitch=pitch1*(1-pitchFactor)+pitch2*pitchFactor;

		int pos=0,i=0;
		TData lastFreq=0;
		TData magFactor;
		do
		{
			
			if(!mIsHarmonicCtl.GetLastValue())
			{
				outFreq[i]=in1Freq[i]*(1-freqFactor)+in2Freq[i*factor2]*freqFactor;
				CLAM_DEBUG_ASSERT(outFreq[i]>=lastFreq,"Error");
				lastFreq=outFreq[i];

				magFactor=spectralShape.GetMag(outFreq[i]);
				if(magFactor>1) magFactor=1;
				if(magFactor<0) magFactor=0;
				outMag[i]=in1Mag[i]*(1-magFactor)+in2Mag[i*factor2]*magFactor;
				CLAM_DEBUG_ASSERT(outMag[i]<1,"Error");
				CLAM_DEBUG_ASSERT(outMag[i]>-1,"Error");
				CLAM_DEBUG_ASSERT(outFreq[i]<22000,"Error");
				CLAM_DEBUG_ASSERT(outFreq[i]>=0,"Error");
			}
			else if(FindHarmonic(in2Index,in1Index[i],pos))
			{
				//Morphing Using Harmonic No*/
				
				outFreq[i]=((in1Freq[i]/pitch1)*(1-freqFactor)+(in2Freq[pos]/pitch2)*freqFactor)*newPitch;
				CLAM_DEBUG_ASSERT(outFreq[i]>lastFreq,"Error");
				lastFreq=outFreq[i];
				
				magFactor=spectralShape.GetMag(outFreq[i]);
				if(magFactor>1) magFactor=1;
				if(magFactor<0) magFactor=0;
				outMag[i]=in1Mag[i]*(1-magFactor)+in2Mag[pos]*magFactor;
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
				outMag[i]=TData(0.0000000001);
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
		if(mConfig.GetUseSpectralShape())
			return Do(mIn1.GetData(),mIn2.GetData(),mSpectralShape.GetData(),mOut.GetData());
		else
			return Do(mIn1.GetData(),mIn2.GetData(),mOut.GetData());
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