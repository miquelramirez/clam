#include "AutoPanner.hxx"
#include <iostream>
#include "OSDefines.hxx"
#include "CLAM_Math.hxx"

namespace CLAM
{

AutoPanner::AutoPanner(CLAM::TData freq, 
			     CLAM::TData samplingRate,
			     CLAM::TData phase,
			     int frameSize)
	: mLeft("Left Control"),
	  mRight("Right Control")
{
	Configure(freq, samplingRate, phase, frameSize);
}

void AutoPanner::Configure(CLAM::TData freq, CLAM::TData samplingRate, CLAM::TData phase, 
			      int frameSize)
{
	mFreq = freq;
	mSamplingRate = samplingRate;
	mPhase = phase;
	mFrameSize = frameSize;
	mDeltaPhase = ((2* TData(M_PI) *mFreq)/mSamplingRate)* mFrameSize;
}


bool AutoPanner::Do()
{
	CLAM::TData newValue = sin(mPhase);
	mPhase += mDeltaPhase;
	if (mPhase > (2*M_PI))
	{
		mPhase = fmod(mPhase,TData(2*M_PI));
	}

	CLAM::TData firstValue = abs(newValue);
	CLAM::TData secondValue = 1 - abs(newValue);

	mLeft.SendControl(firstValue);
	mRight.SendControl(secondValue);
	       
	return true;
}

} // namespace CLAM

