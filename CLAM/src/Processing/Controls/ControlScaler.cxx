#include "ControlScaler.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "ControlScaler",
		"category", "Controls",
		"description", "ControlScaler",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ControlScaler> reg = metadata;
}

void ControlScalerConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetAmount( 1.0 );
}

ControlScaler::ControlScaler()
	: mInControl( "Control In", this , &ControlScaler::InControlCallback )
	, mGainControl( "Gain Amount", this , &ControlScaler::InControlCallback )
	, mOutControl( "Control Out", this )
{
	Configure( mConfig );	
}

ControlScaler::ControlScaler( const ControlScalerConfig& cfg ) 
	: mInControl( "Control In", this , &ControlScaler::InControlCallback )
	, mGainControl( "Gain Amount", this , &ControlScaler::InControlCallback )
	, mOutControl( "Control Out", this )
{ 
	Configure( cfg );
}

bool ControlScaler::ConcreteConfigure( const ProcessingConfig& cfg )
{
	CopyAsConcreteConfig( mConfig, cfg );
	mGainControl.DoControl(mConfig.GetAmount());
	return true; 		
}
void ControlScaler::InControlCallback(const TControlData & value)
{
	TControlData in = mInControl.GetLastValue();
	TControlData gain = mGainControl.GetLastValue();
	mOutControl.SendControl(in * gain);
}
bool ControlScaler::Do()
{
	return true;
}
}

