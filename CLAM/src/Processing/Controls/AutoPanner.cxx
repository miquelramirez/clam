
#include "AutoPanner.hxx"
#include "OSDefines.hxx"
#include "CLAM_Math.hxx"

namespace CLAM
{

void AutoPannerConfig::DefaultInit(void)
{
	AddAll();       
	UpdateData();	
	SetFrequency(440.0);
	SetPhase(0.0);
	SetSamplingRate( 44100 );
	SetFrameSize( 512 );
}

AutoPanner::AutoPanner()
	: mLeft("Left Control", this ),
	  mRight("Right Control", this )
{
	AutoPannerConfig cfg;

	Configure(cfg);
}

AutoPanner::AutoPanner( const AutoPannerConfig & cfg)
	: mLeft("Left Control", this ),
	  mRight("Right Control", this )
{

	Configure(cfg);
}


bool AutoPanner::Do()
{	
	if( !AbleToExecute() ) return true;

	CLAM::TData newValue = sin(mPhase);
	mPhase += mDeltaPhase;
	if (mPhase > (2*M_PI))
	{
		mPhase = fmod(mPhase,TData(2*M_PI));
	}

	CLAM::TData firstValue = fabs(newValue);
	CLAM::TData secondValue = 1 - fabs(newValue);

	mLeft.SendControl(firstValue);
	mRight.SendControl(secondValue);
       	return true;
}

bool AutoPanner::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	mFreq = mConfig.GetFrequency();
	mSamplingRate = mConfig.GetSamplingRate();
	mPhase = mConfig.GetPhase();
	mFrameSize = mConfig.GetFrameSize();
	mDeltaPhase = ((2* TData(M_PI) *mFreq)/mSamplingRate)* mFrameSize;

	return true;
}

} // namespace CLAM

