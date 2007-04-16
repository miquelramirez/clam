/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "SegmentTransformation.hxx"

namespace CLAM
{
	void SegmentTransformation::WrapFrameTransformation( FrameTransformation* trans)
	{
		CLAM_ASSERT( !mFrameTransformation, "SegmentTransformation::Wrapp object shoudn't have wrapped transformation");
		mFrameTransformation = trans;
	}
	
	SegmentTransformation::SegmentTransformation()
		: mOnCtrl("On",this)
		, mAmountCtrl("Amount",this)
	{
		mInput = 0;
		mOutput = 0;
		mUseTemporalBPF = 0;
		mCurrentInputFrame = 0;
		mFrameTransformation = 0;
	}

	SegmentTransformation::SegmentTransformation(const SegmentTransformationConfig& c) 
		: mOnCtrl("On",this)
		, mAmountCtrl("Amount",this)
	{
		mInput = 0;
		mOutput = 0;
		mUseTemporalBPF = 0;
		mCurrentInputFrame = 0;
		Configure(c);
		mFrameTransformation = 0;
	}

	SegmentTransformation::~SegmentTransformation()
	{
		if (mFrameTransformation)
			delete mFrameTransformation;
	};
	
	bool SegmentTransformation::Do(const Segment& in, Segment& out)
	{
		CLAM_ASSERT(mInput==&in, "sms transformation chain needs input segment");
		//TODO find out why this finalization condition (and not just using size)
		while( mCurrentInputFrame<in.mCurrentFrameIndex)
		{
			if(mUseTemporalBPF)
				UpdateControlValueFromBPF(((TData)in.mCurrentFrameIndex)/in.GetnFrames());
			
			AddFramesToOutputIfInputIsLonger(mCurrentInputFrame, in, out);
			
			const Frame & inframe = in.GetFrame(mCurrentInputFrame);
			Frame & outframe = out.GetFrame(mCurrentInputFrame);

			if (mFrameTransformation)
			{
				mFrameTransformation->Do(inframe, outframe);
			}
			else //TODO remove when refactoring is done 
			{
				Do( inframe, outframe );
			}
			

			
			if(&in!=&out)
				out.mCurrentFrameIndex++;
			
			mCurrentInputFrame++;
		}
		return true;
	}

	bool SegmentTransformation::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		mUseTemporalBPF=false;
		if(mConfig.HasAmount())
		{
			mAmountCtrl.DoControl(mConfig.GetAmount());								
		}
		else if(mConfig.HasBPFAmount()){
			mAmountCtrl.DoControl(mConfig.GetBPFAmount().GetValue(0)); 		
			mUseTemporalBPF=true;
		}
		else
		{
			mAmountCtrl.DoControl(0);										
		}

		return true;
	}	

	bool SegmentTransformation::UpdateControlValueFromBPF(TData pos)
	{
		if(mConfig.HasBPFAmount())
		{
			TControlData amount = mConfig.GetBPFAmount().GetValue(pos);
			mAmountCtrl.DoControl(amount);
			return true;
		}
		else return false;
	}

	bool SegmentTransformation::IsLastFrame()
	{
		bool isLast=mInput->mCurrentFrameIndex >= mInput->GetnFrames();
		
		if(isLast)
		{
			while(mOutput->GetnFrames()>=mOutput->mCurrentFrameIndex)
			{
				mOutput->DeleteFrame(mOutput->GetnFrames()-1);
			}
		}
		return isLast;
	}

	bool SegmentTransformation::ConcreteStart()
	{
		return true;
	}



}

