#include "ControlScaler.hxx"
#include "Factory.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<CLAM::ControlScaler> regtControlScaler( "ControlScaler" );
	}

	void ControlScalerConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetAmount( 1.0 );
	}

	ControlScaler::ControlScaler()
		: mInControl( "Control In", this )
		, mGainControl( "Gain Amount", this )
		, mOutControl( "Control Out", this )
	{
		Configure( mConfig );	
	}

	ControlScaler::ControlScaler( const ControlScalerConfig& cfg ) 
		: mInControl( "Control In", this )
		, mGainControl( "Gain Amount", this )
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

	bool ControlScaler::Do()
	{
		TControlData in = mInControl.GetLastValue();
		TControlData gain = mGainControl.GetLastValue();
		return mOutControl.SendControl(in * gain);
	}
}

