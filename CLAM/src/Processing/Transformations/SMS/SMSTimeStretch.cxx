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

#include "SMSTimeStretch.hxx"
#include "Factory.hxx"

using namespace CLAM;

SMSTimeStretch::SMSTimeStretch()
{
	mSynthesisTime=0;
	mAnalysisTime=0;
	mCurrentInputFrame=-1;
}

bool SMSTimeStretch::ConcreteConfigure(const ProcessingConfig& cfg)
{
	CopyAsConcreteConfig(mConcreteConfig,cfg);
	mUseTemporalBPF=false;
	if(mConcreteConfig.HasAmount())
		mAmountCtrl.DoControl(mConcreteConfig.GetAmount());
	else if(mConcreteConfig.HasBPFAmount()){
		mAmountCtrl.DoControl(mConcreteConfig.GetBPFAmount().GetValue(0));
		mUseTemporalBPF=true;}
	else
		mAmountCtrl.DoControl(0);
	mPO_FrameInterpolator.Configure(FrameInterpConfig());
	return true;
}

bool SMSTimeStretch::ConcreteStart()
{
	mSynthesisTime=0;
	mAnalysisTime=0;
	mCurrentInputFrame=-1;
	mnSynthesisFrames=0;
	mPO_FrameInterpolator.Start();
	return SMSTransformationTmpl<Frame>::ConcreteStart();
}

bool SMSTimeStretch::Do(const Frame& in, Frame& out)
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
	mPO_FrameInterpolator.Do(in,mLeftFrame,out);
	
	return true;
}


bool SMSTimeStretch::Do(const Segment& in, Segment& out)
{
	if(mCurrentInputFrame>-1)
	{
		while(mCurrentInputFrame<in.mCurrentFrameIndex&&!HaveFinished())
		{
			UpdateControlValueFromBPF(((TData)mCurrentInputFrame)/in.GetnFrames());
			TData previousAnalysisTime=mAnalysisTime;
			UpdateTimeAndIndex(in);
			if(mCurrentInputFrame>=in.mCurrentFrameIndex)
			{
				mCurrentInputFrame=in.mCurrentFrameIndex-2;
				mLeftFrame=in.GetFrame(mCurrentInputFrame);
				mAnalysisTime=previousAnalysisTime;
				return true;
			}
			Do(UnwrapSegment(in),UnwrapSegment(out));
			CLAM_DEBUG_ASSERT(mCurrentInputFrame<in.mCurrentFrameIndex,"Error");
			out.mCurrentFrameIndex++;
		}
	}
	else mCurrentInputFrame++;
	return true;
}

void SMSTimeStretch::UpdateTimeAndIndex(const Segment& in)
{
	mAnalysisTime+=(TData)mConcreteConfig.GetHopSize()*mAmountCtrl.GetLastValue()/mConcreteConfig.GetSamplingRate();
	while(mAnalysisTime>mLeftFrame.GetCenterTime()+mConcreteConfig.GetHopSize()/mConcreteConfig.GetSamplingRate()&&mCurrentInputFrame<=in.GetnFrames())
	{
		mLeftFrame=in.GetFrame(mCurrentInputFrame);
		mCurrentInputFrame++;
	}
}

const Frame& SMSTimeStretch::UnwrapProcessingData(const Segment& in,Frame*)
{
	return in.GetFrame(mCurrentInputFrame);
}


Frame& SMSTimeStretch::UnwrapProcessingData(Segment& out,Frame*)
{
	if(mnSynthesisFrames==out.GetnFrames())
		out.AddFrame(out.GetFrame(out.GetnFrames()-1));
	return out.GetFrame(mnSynthesisFrames);
}

bool SMSTimeStretch::HaveFinished()
{
	return mCurrentInputFrame>mInput.GetData().GetnFrames();
}

bool SMSTimeStretch::IsLastFrame()
{
	bool isLast=HaveFinished();
	if(isLast)
	{
		while(mOutput.GetData().GetnFrames()>mnSynthesisFrames-1)
		{
			mOutput.GetData().DeleteFrame(mOutput.GetData().GetnFrames()-1);
		}
	}
	return isLast;
}

bool SMSTimeStretch::UpdateControlValueFromBPF(TData pos)
{
	if(mConcreteConfig.HasBPFAmount())
	{
		mAmountCtrl.DoControl(mConcreteConfig.GetBPFAmount().GetValue(pos));
		return true;
	}
	else return false;
}

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
static ProcessingFactory::Registrator<CLAM::SMSTimeStretch> regtSMSTimeStretch( "SMSTimeStretch" );
