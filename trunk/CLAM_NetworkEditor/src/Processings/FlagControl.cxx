#include "FlagControl.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{

	namespace detail
	{
		static ProcessingFactory::Registrator<FlagControl> regtFlagControl( "FlagControl" );
	}

void FlagControlConfig::DefaultInit(void)
{
	AddAll();       
	UpdateData();	
	SetName("FlagControl");
	SetFlag(false);
}

FlagControl::FlagControl()
	: mFlagOutControl("Flag", this)
{
	FlagControlConfig cfg;

	Configure(cfg);
}

FlagControl::FlagControl( const FlagControlConfig & cfg)
	: mFlagOutControl("Flag", this)
	
{

	Configure(cfg);
}


bool FlagControl::Do()
{	
	if( !AbleToExecute() ) return true;
	mFlagOutControl.SendControl(mConfig.GetFlag());
       	return true;
}

bool FlagControl::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	return true;
}

} // namespace CLAM

