
#include "Random.hxx"
#include <CLAM/OSDefines.hxx>
#include <CLAM/CLAM_Math.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include <ctime>
#include <cstdlib>

#define DEFAULTDELAYFACTOR 10


namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "Random",
		"category", "Controls",
		"description", "Random",
		0
	};
	static FactoryRegistrator<ProcessingFactory, Random> reg = metadata;
}
	
	
void RandomConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
	SetName("Random");
	SetDelayFactor(DEFAULTDELAYFACTOR);
	SetMinNote(36);
	SetMaxNote(98);
	srand( (unsigned)time( NULL ) );
}

Random::Random()
	: mInput("In delay factor", this ),
	  mOutput("Out frequency", this ),
	  mMidiToFreq(),
	  mCounter(DEFAULTDELAYFACTOR)
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
	  mCounter(DEFAULTDELAYFACTOR)
{
	Configure(cfg);
}


bool Random::Do()
{
	if( !AbleToExecute() ) return true;

//	static int Counter = (int)mInput.GetLastValue(); // Sends control from the first call
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




