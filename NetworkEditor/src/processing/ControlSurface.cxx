#include "ControlSurface.hxx"
#include <CLAM/ProcessingFactory.hxx>


namespace CLAM
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, ControlSurface> regControlSurface("ControlSurface");
}
	
void ControlSurfaceConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
	SetMinX( 0.0 );
	SetDefaultX( 0.0 );
	SetMaxX( 1.0 );
	SetMinY( 0.0 );
	SetDefaultY( 0.0 );
	SetMaxY( 1.0 );
}

ControlSurface::ControlSurface()
	: mOutputX("outX", this)
	, mOutputY("outY", this)
	, mFirstDoAfterStart(true)
{
	ControlSurfaceConfig cfg;
	Configure(cfg);
}

ControlSurface::ControlSurface( const ControlSurfaceConfig & cfg)
	: mOutputX("outX", this)
	, mOutputY("outY", this)
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
		mOutputX.SendControl( mLastValueX );
		mOutputY.SendControl( mLastValueY );
	}
	return true;
}

void ControlSurface::SendControl(TControlData valueX, TControlData valueY)
{
	// TODO: Solve thread boundary here
	mLastValueX=valueX;
	mLastValueY=valueY;
	mOutputX.SendControl( mLastValueX );
	mOutputY.SendControl( mLastValueY );
}

bool ControlSurface::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	if (mConfig.GetMinX() > mConfig.GetMaxX() )
	{
		AddConfigErrorMessage("Control 1 min value greater than max. ");
		return false;
	}
	if (mConfig.GetMinY() > mConfig.GetMaxY() )
	{
		AddConfigErrorMessage("Control 2 min value greater than max. ");
		return false;
	}
	if ((mConfig.GetDefaultX() > mConfig.GetMaxX()) || (mConfig.GetDefaultX() < mConfig.GetMinX()))
	{
		AddConfigErrorMessage("Default value for control 1 is out of range. ");
		return false;
	}
	if ((mConfig.GetDefaultY() > mConfig.GetMaxY()) || (mConfig.GetDefaultY() < mConfig.GetMinY()))
	{
		AddConfigErrorMessage("Default value for control 2 is out of range. ");
		return false;
	}
	mLastValueX = mConfig.GetDefaultX();
	mLastValueY = mConfig.GetDefaultY();
	return true;
}

} // namespace CLAM



