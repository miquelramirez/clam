#include "ControlSurface.hxx"
#include <CLAM/Factory.hxx>

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<ControlSurface> regtControlSurface( "ControlSurface" );
	}
	
void ControlSurfaceConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
	SetMin1( 0.0 );
	SetDefault1( 0.0 );
	SetMax1( 1.0 );
	SetMin2( 0.0 );
	SetDefault2( 0.0 );
	SetMax2( 1.0 );
}

ControlSurface::ControlSurface()
	: mOutput1("out1", this)
	, mOutput2("out2", this)
	, mFirstDoAfterStart(true)
{
	ControlSurfaceConfig cfg;
	Configure(cfg);
}

ControlSurface::ControlSurface( const ControlSurfaceConfig & cfg)
	: mOutput1("out1", this)
	, mOutput2("out2", this)
	, mFirstDoAfterStart(true)
{
	Configure(cfg);
}


bool ControlSurface::ConcreteStart()
{
	mFirstDoAfterStart=true;
	return true;
}

bool ControlSurface::Do()
{
	if( !AbleToExecute() ) return true;
	if (mFirstDoAfterStart)
	{
		mFirstDoAfterStart=false;
		mOutput1.SendControl( mLastValue1 );
		mOutput2.SendControl( mLastValue2 );
	}
	return true;
}

void ControlSurface::SendControl(TControlData value1, TControlData value2)
{
	// TODO: Solve thread boundary here
	mLastValue1=value1;
	mLastValue2=value2;
	mOutput1.SendControl( mLastValue1 );
	mOutput2.SendControl( mLastValue2 );
}

bool ControlSurface::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	if (mConfig.GetMin1() > mConfig.GetMax1() )
	{
		AddConfigErrorMessage("Control 1 min value greater than max. ");
		return false;
	}
	if (mConfig.GetMin2() > mConfig.GetMax2() )
	{
		AddConfigErrorMessage("Control 2 min value greater than max. ");
		return false;
	}
	if ((mConfig.GetDefault1() > mConfig.GetMax1()) || (mConfig.GetDefault1() < mConfig.GetMin1()))
	{
		AddConfigErrorMessage("Default value for control 1 is out of range. ");
		return false;
	}
	if ((mConfig.GetDefault2() > mConfig.GetMax2()) || (mConfig.GetDefault2() < mConfig.GetMin2()))
	{
		AddConfigErrorMessage("Default value for control 2 is out of range. ");
		return false;
	}
	mLastValue1 = mConfig.GetDefault1();
	mLastValue2 = mConfig.GetDefault2();
	return true;
}

} // namespace CLAM



