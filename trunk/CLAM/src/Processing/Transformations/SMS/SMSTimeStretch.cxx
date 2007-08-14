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

#include "SMSTimeStretch.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{	
	static const char * metadata[] = {
		"key", "SMSTimeStretch",
		"category", "SMS Transformations",
		"description", "SMSTimeStretch",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, SMSTimeStretch> regSMSTimeStretch("SMSTimeStretch");
	static FactoryRegistrator<ProcessingFactory, SMSTimeStretch> reg = metadata;
}

bool SMSTimeStretch::Do(const Frame& in, Frame& out)
{
	return Do( in.GetSpectralPeakArray(),
		   in.GetFundamental(),
		   in.GetResidualSpec(),

		   out.GetSpectralPeakArray(),
		   out.GetFundamental(), 
		   out.GetResidualSpec() 
		 );
}

// bool SMSTimeStretch::Do(const Segment& in, Segment& out)
// {
// 	//Unused var: char test;
// 	if(mCurrentInputFrame>-1)
// 	{
// 		while(mCurrentInputFrame<in.mCurrentFrameIndex&&!HaveFinished())
// 		{
// 			if(mConcreteConfig.GetUseBPF())
// 			{
// 				UpdateControlValueFromBPF(((TData)mCurrentInputFrame)/in.GetnFrames());
// 			}
// 			TData previousAnalysisTime=mAnalysisTime;
// 			UpdateTimeAndIndex(in);
// 			if(mCurrentInputFrame>=in.mCurrentFrameIndex)
// 			{
// 				UpdateControlValueFromBPF(((TData)mCurrentInputFrame)/in.GetnFrames());
// 				mCurrentInputFrame=in.mCurrentFrameIndex-2;
// 				if(mCurrentInputFrame<0) mCurrentInputFrame=0;//I don't know why but this happens sometimes
// 				mLeftFrame=in.GetFrame(mCurrentInputFrame);
// 				mAnalysisTime=previousAnalysisTime;
// 				return true;
// 			}
// 			Do(GetCurrentFrame(in),GetCurrentFrame(out));
// 			CLAM_DEBUG_ASSERT(mCurrentInputFrame<in.mCurrentFrameIndex,"Error");
// 			out.mCurrentFrameIndex++;
// 		}
// 	}
// 	else mCurrentInputFrame++;
// 	return true;
// }
bool SMSTimeStretch::Do( const SpectralPeakArray& inPeaks, 
			const Fundamental& inFund,
			const Spectrum& inSpectrum, 
			SpectralPeakArray& outPeaks,
			Fundamental& outFund,
			Spectrum& outSpectrum
		       )
{
// /**TODO: This method does not work if called directly! it must be called from the Segment overload*/
// bool SMSTimeStretch::Do(const Frame& in, Frame& out)

	
	//FIXME
	TData interpFactor; // = (mAnalysisTime-mLeftFrame.GetCenterTime()) / (mConfig.GetHopSize()/mConfig.GetSamplingRate());
// 	out.SetCenterTime(mSynthesisTime);

	mSynthesisTime += (TData)mConfig.GetHopSize() / mConfig.GetSamplingRate();

	mnSynthesisFrames++;

// 	if (interpFactor>1)
// 	{
// 		TData tmpCenterTime = out.GetCenterTime();
// 		out = mLeftFrame;
// 		out.SetCenterTime( tmpCenterTime );
// 		return true;
// 	}

// 	if (interpFactor<0)
// 	{
// 		TData tmpCenterTime=out.GetCenterTime();
// 		out=in;
// 		out.SetCenterTime(tmpCenterTime);
// 		return true;
// 	}

// 	mFrameInterpolator.mFrameInterpolationFactorCtl.DoControl( interpFactor );
// 	mLeftFrame.GetSpectralPeakArray().SetIsIndexUpToDate(true);
// 	mFrameInterpolator.Do( in, mLeftFrame, out );
//TODO check morph interpolation
// 	TData newPitch = (1. - interpFactor)*inFund1.GetFreq() + interpFactor*inFund2.GetFreq();
// 	if( Harmonic ) newPitch=0;
// 	//Sets new fund freq
// 	if (outFund.GetnCandidates()==0)
// 		outFund.AddElem(newPitch,0);
// 	else
// 		outFund.SetFreq(0,newPitch);
// 	outFund.SetnCandidates(1);

// 	mPeaksInterpolator.GetInControl("MagInterpolationFactor").DoControl(interpFactor);
// 	mPeaksInterpolator.GetInControl("FreqInterpolationFactor").DoControl(alpha);
// 	mPeaksInterpolator.GetInControl("PitchInterpolationFactor").DoControl(alpha);
// 	mPeaksInterpolator.Do(inPeaks, inPeaks, outPeaks);


//TODO separate alpha/interpolation value for peaks and residual????

// 	outSpectrum = inSpectrum1; //FIXME asserts without this...
// 	CLAM_DEBUG_ASSERT( inSpectrum1.GetSize()==inSpectrum2.GetSize(), "Expected two spectrums of the same size");
// 	mSpectrumInterpolator.GetInControl("InterpolationFactor").DoControl(alpha);

// 	TODO fix (and check SpectrumInterpolator bug... (add/fix const inputs) mSpectrumInterpolator.Do(inSpectrum1, inSpectrum2, outSpectrum);)
// 	mSpectrumInterpolator.Do(const_cast<Spectrum&>(inSpectrum1), const_cast<Spectrum&>(inSpectrum2), outSpectrum);
	
	return true;
}

// SMSTimeStretch::SMSTimeStretch():mAmount("Amount",this)
// {
// 	mSynthesisTime=0;
// 	mAnalysisTime=0;
// 	mCurrentInputFrame=-1;
// 	Configure(SMSTimeStretchConfig());
// }

// SMSTimeStretch::SMSTimeStretch(const SegmentTransformationConfig &c):SegmentTransformation(c),mAmount("Amount",this)
// {
// 	mSynthesisTime=0;
// 	mAnalysisTime=0;
// 	mCurrentInputFrame=-1;
// 	Configure(c);
// }

bool SMSTimeStretch::ConcreteConfigure(const ProcessingConfig& config)
{
	CopyAsConcreteConfig( mConfig, config );

// 	mUseTemporalBPF=false;
// 	if(mConcreteConfig.HasAmount())
// 		mAmount.DoControl(mConcreteConfig.GetAmount());
// 	else if(mConcreteConfig.HasBPFAmount()){
// 		mAmount.DoControl(mConcreteConfig.GetBPFAmount().GetValue(0));
// 		mUseTemporalBPF=true;}
// 	else
// 		mAmount.DoControl(0);
	
// 	FrameInterpConfig tmpCfg;
// 	tmpCfg.SetHarmonic(mConfig.GetHarmonic());
// 	mFrameInterpolator.Configure(tmpCfg);
// 	mFrameInterpolator.Configure(FrameInterpConfig());

	return true;
}

//FIXME
bool SMSTimeStretch::ConcreteStop()
{
// 	mFrameInterpolator.Stop();
	return true;
}

//FIXME
bool SMSTimeStretch::ConcreteStart()
{
	mSynthesisTime=0;
	mAnalysisTime=0;
// 	mCurrentInputFrame=-1;
	mnSynthesisFrames=0;
// 	mFrameInterpolator.Start();
	mLeftFrame.SetCenterTime(-1);
	return true;
}

// void SMSTimeStretch::UpdateTimeAndIndex(const Segment& in)
// {
// 	mAnalysisTime+=(TData)mConcreteConfig.GetHopSize()*mAmount.GetLastValue()/mConcreteConfig.GetSamplingRate();
// 	while(mAnalysisTime>mLeftFrame.GetCenterTime()+mConcreteConfig.GetHopSize()/mConcreteConfig.GetSamplingRate()&&mCurrentInputFrame<=in.GetnFrames())
// 	{
// 		mLeftFrame=in.GetFrame(mCurrentInputFrame);
// 		mCurrentInputFrame++;
// 	}
// }
// 
// const Frame& SMSTimeStretch::GetCurrentFrame(const Segment& in)
// {
// 	return in.GetFrame(mCurrentInputFrame);
// }
// 
// 
// Frame& SMSTimeStretch::GetCurrentFrame(Segment& out)
// {
// 	if(mnSynthesisFrames==out.GetnFrames())
// 		out.AddFrame(out.GetFrame(out.GetnFrames()-1));
// 	return out.GetFrame(mnSynthesisFrames);
// }
// 
// 
// bool SMSTimeStretch::HaveFinished()
// {
// 	return mCurrentInputFrame>mInput->GetnFrames();
// }
// 
// bool SMSTimeStretch::IsLastFrame()
// {
// 	bool isLast=HaveFinished();
// 	if(isLast)
// 	{
// 		while(mOutput->GetnFrames()>mnSynthesisFrames-1)
// 		{
// 			mOutput->DeleteFrame(mOutput->GetnFrames()-1);
// 		}
// 	}
// 	return isLast;
// }


}

