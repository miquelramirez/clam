#include "SMSTransformation.hxx"

namespace CLAM
{
	SMSTransformation::SMSTransformation()
		: mAmountCtrl("Amount",this), mOnCtrl("On",this), 
		mInput(0), mOutput(0), mUseTemporalBPF( false )
	{
		mCurrentInputFrame=0;
	}

	SMSTransformation::SMSTransformation(const SMSTransformationConfig& c)
		:mAmountCtrl("Amount",this),mOnCtrl("On",this),
		mInput(0),mOutput(0), mUseTemporalBPF( false )
	{
		mCurrentInputFrame=0;
		Configure(c);
	}

	bool SMSTransformation::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		mUseTemporalBPF=false;
		if(mConfig.HasAmount())
			mAmountCtrl.DoControl(mConfig.GetAmount());
		else if(mConfig.HasBPFAmount()){
			mAmountCtrl.DoControl(mConfig.GetBPFAmount().GetValue(0));
			mUseTemporalBPF=true;}
		else
			mAmountCtrl.DoControl(0);
		return true;
	}	

	bool SMSTransformation::UpdateControlValueFromBPF(TData pos)
	{
		if(mConfig.HasBPFAmount())
		{
			mAmountCtrl.DoControl(mConfig.GetBPFAmount().GetValue(pos));
			return true;
		}
		else return false;
	}

	bool SMSTransformation::IsLastFrame()
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

	bool SMSTransformation::ConcreteStart()
	{
		mCurrentInputFrame=0;
		return true;
	}



}
