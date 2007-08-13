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

#include "SegmentSMSTimeStretch.hxx"
#include "ProcessingFactory.hxx"



namespace CLAM
{

namespace Hidden
{	
	static const char * metadata[] = {
		"key", "SegmentSMSTimeStretch",
		"category", "SMS Transformations",
		"description", "SegmentSMSTimeStretch",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, SegmentSMSTimeStretch> regSegmentSMSTimeStretch("SegmentSMSTimeStretch");
	static FactoryRegistrator<ProcessingFactory, SegmentSMSTimeStretch> reg = metadata;
}

SegmentSMSTimeStretch::SegmentSMSTimeStretch():mAmount("Amount",this)
{
	mSynthesisTime=0;
	mAnalysisTime=0;
	mCurrentInputFrame=-1;
	Configure(SegmentSMSTimeStretchConfig());
}

SegmentSMSTimeStretch::SegmentSMSTimeStretch(const SegmentTransformationConfig &c):SegmentTransformation(c),mAmount("Amount",this)
{
	mSynthesisTime=0;
	mAnalysisTime=0;
	mCurrentInputFrame=-1;
	Configure(c);
}

bool SegmentSMSTimeStretch::ConcreteConfigure(const ProcessingConfig& cfg)
{
	CopyAsConcreteConfig(mConcreteConfig,cfg);
	mUseTemporalBPF=false;
	if(mConcreteConfig.HasAmount())
		mAmount.DoControl(mConcreteConfig.GetAmount());
	else if(mConcreteConfig.HasBPFAmount()){
		mAmount.DoControl(mConcreteConfig.GetBPFAmount().GetValue(0));
		mUseTemporalBPF=true;}
	else
		mAmount.DoControl(0);
	
	FrameInterpConfig tmpCfg;
	tmpCfg.SetHarmonic(mConcreteConfig.GetHarmonic());
	mPO_FrameInterpolator.Configure(tmpCfg);

	mPO_FrameInterpolator.Configure(FrameInterpConfig());

	return true;
}

bool SegmentSMSTimeStretch::ConcreteStop()
{
	mPO_FrameInterpolator.Stop();
	return true;
}

bool SegmentSMSTimeStretch::ConcreteStart()
{
	mSynthesisTime=0;
	mAnalysisTime=0;
	mCurrentInputFrame=-1;
	mnSynthesisFrames=0;
	mPO_FrameInterpolator.Start();
	mLeftFrame.SetCenterTime(-1);
	return true;
}

/**TODO: This method does not work if called directly! it must be called from the Segment overload*/
bool SegmentSMSTimeStretch::Do(const Frame& in, Frame& out)
{
	TData interpFactor= (mAnalysisTime-mLeftFrame.GetCenterTime())/(mConcreteConfig.GetHopSize()/mConcreteConfig.GetSamplingRate());
	out.SetCenterTime(mSynthesisTime);
	mSynthesisTime+=(TData)mConcreteConfig.GetHopSize()/mConcreteConfig.GetSamplingRate();
	mnSynthesisFrames++;
	if(interpFactor>1)
	{
		TData tmpCenterTime=out.GetCenterTime();
		out=mLeftFrame;
		out.SetCenterTime(tmpCenterTime);
		return true;
	}
	if (interpFactor<0)
	{
		TData tmpCenterTime=out.GetCenterTime();
		out=in;
		out.SetCenterTime(tmpCenterTime);
		return true;
	}

	mPO_FrameInterpolator.mFrameInterpolationFactorCtl.DoControl(interpFactor);
	mLeftFrame.GetSpectralPeakArray().SetIsIndexUpToDate(true);
	mPO_FrameInterpolator.Do(in,mLeftFrame,out);
	
	return true;
}


bool SegmentSMSTimeStretch::Do(const Segment& in, Segment& out)
{
	//Unused var: char test;
	if(mCurrentInputFrame>-1)
	{
		while(mCurrentInputFrame<in.mCurrentFrameIndex&&!HaveFinished())
		{
			if(mConcreteConfig.GetUseBPF())
			{
				UpdateControlValueFromBPF(((TData)mCurrentInputFrame)/in.GetnFrames());
			}
			TData previousAnalysisTime=mAnalysisTime;
			UpdateTimeAndIndex(in);
			if(mCurrentInputFrame>=in.mCurrentFrameIndex)
			{
				UpdateControlValueFromBPF(((TData)mCurrentInputFrame)/in.GetnFrames());
				mCurrentInputFrame=in.mCurrentFrameIndex-2;
				if(mCurrentInputFrame<0) mCurrentInputFrame=0;//I don't know why but this happens sometimes
				mLeftFrame=in.GetFrame(mCurrentInputFrame);
				mAnalysisTime=previousAnalysisTime;
				return true;
			}
			Do(GetCurrentFrame(in),GetCurrentFrame(out));
			CLAM_DEBUG_ASSERT(mCurrentInputFrame<in.mCurrentFrameIndex,"Error");
			out.mCurrentFrameIndex++;
		}
	}
	else mCurrentInputFrame++;
	return true;
}

void SegmentSMSTimeStretch::UpdateTimeAndIndex(const Segment& in)
{
	mAnalysisTime+=(TData)mConcreteConfig.GetHopSize()*mAmount.GetLastValue()/mConcreteConfig.GetSamplingRate();
	while(mAnalysisTime>mLeftFrame.GetCenterTime()+mConcreteConfig.GetHopSize()/mConcreteConfig.GetSamplingRate()&&mCurrentInputFrame<=in.GetnFrames())
	{
		mLeftFrame=in.GetFrame(mCurrentInputFrame);
		mCurrentInputFrame++;
	}
}

const Frame& SegmentSMSTimeStretch::GetCurrentFrame(const Segment& in)
{
	return in.GetFrame(mCurrentInputFrame);
}


Frame& SegmentSMSTimeStretch::GetCurrentFrame(Segment& out)
{
	if(mnSynthesisFrames==out.GetnFrames())
		out.AddFrame(out.GetFrame(out.GetnFrames()-1));
	return out.GetFrame(mnSynthesisFrames);
}


bool SegmentSMSTimeStretch::HaveFinished()
{
	return mCurrentInputFrame>mInput->GetnFrames();
}

bool SegmentSMSTimeStretch::IsLastFrame()
{
	bool isLast=HaveFinished();
	if(isLast)
	{
		while(mOutput->GetnFrames()>mnSynthesisFrames-1)
		{
			mOutput->DeleteFrame(mOutput->GetnFrames()-1);
		}
	}
	return isLast;
}


}

