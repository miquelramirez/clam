
#include "DummyLadspa.hxx"
#include <CLAM/OSDefines.hxx>
#include <CLAM/CLAM_Math.hxx>
#include <CLAM/Factory.hxx>
#include <ctime>
#include <cstdlib>

#define DEFAULTDELAYFACTOR 10


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<DummyLadspa> regtDummyLadspa( "DummyLadspa" );
	}
	
	
void DummyLadspaConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
	SetName("DummyLadspa");
	SetDelayFactor(DEFAULTDELAYFACTOR);
	SetMinNote(36);
	SetMaxNote(98);
	srand( (unsigned)time( NULL ) );
}

DummyLadspa::DummyLadspa()
	: mInput("In delay factor", this ),
	  mOutput("Out frequency", this ),
	  mMidiToFreq(),
	  mCounter(DEFAULTDELAYFACTOR)
{
	mInput.DoControl(10);
	mMidiToFreq.Set(69.0,440.0);
	DummyLadspaConfig cfg;
	Configure(cfg);
}

DummyLadspa::DummyLadspa( const DummyLadspaConfig & cfg)
	: mInput("In delay factor", this ),
	  mOutput("Out frequency", this ),
	  mMidiToFreq(),
	  mCounter(DEFAULTDELAYFACTOR)
{
	Configure(cfg);
}


bool DummyLadspa::Do()
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

bool DummyLadspa::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	mDelayFactor = mConfig.GetDelayFactor();
	mMinNote = mConfig.GetMinNote();
	mMaxNote = mConfig.GetMaxNote();
	mInput.DoControl(mDelayFactor);

	return true;
}

} // namespace CLAM




