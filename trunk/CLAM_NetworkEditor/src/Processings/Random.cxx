
#include "Random.hxx"
#include "OSDefines.hxx"
#include "CLAM_Math.hxx"
#include <time.h>
#include <stdlib.h>

namespace CLAM
{

void RandomConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
	SetName("Random");
	SetDelayFactor(10);
	SetMinNote(36);
	SetMaxNote(98);
	srand( (unsigned)time( NULL ) );
}

Random::Random()
	: mInput("In delay factor", this ),
	  mOutput("Out frequency", this ),
	  mMidiToFreq(),
	  mCounter(0)
{
	mInput.DoControl(10);
	mMidiToFreq.Set(69.0,440.0);
	RandomConfig cfg;
	Configure(cfg);
}

Random::Random( const RandomConfig & cfg)
	: mInput("In delay factor", this ),
	  mOutput("Out frequency", this ),
	  mMidiToFreq(),
	  mCounter(0)
{
	Configure(cfg);
}


bool Random::Do()
{
	if( !AbleToExecute() ) return true;

//	static int Counter = (int)mInput.GetLastValue(); // Sends control from the first call
	mCounter = mInput.GetLastValue();
	mCounter++;
	if (mCounter > (int)mInput.GetLastValue())
	{
	  mCounter = 0;
	  TData OutValue = mMidiToFreq.Map((float)(rand()%(mMaxNote-mMinNote)+mMinNote));
	  mOutput.SendControl(OutValue);
	}

	return true;
}

bool Random::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	mDelayFactor = mConfig.GetDelayFactor();
	mMinNote = mConfig.GetMinNote();
	mMaxNote = mConfig.GetMaxNote();
	mInput.DoControl(mDelayFactor);

	return true;
}

} // namespace CLAM



