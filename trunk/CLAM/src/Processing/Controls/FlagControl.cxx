#include "FlagControl.hxx"
#include "ProcessingFactory.hxx"



namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "FlagControl",
		"category", "Controls",
		"description", "FlagControl",
		0
	};
	static FactoryRegistrator<ProcessingFactory, FlagControl> reg = metadata;
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

