
#include "OutControlSender.hxx"
#include <iostream> // TODO: remove
namespace CLAM
{

Enum::tEnumValue OutControlSenderConfig::EControlRepresentation::sEnumValues[] =
	{
		{ EControlRepresentation::eUndetermined, "Undetermined" },
		{ EControlRepresentation::eVerticalSlider, "Vertical Slider" },
		{ EControlRepresentation::eHorizontalSlider, "Horizontal Slider" },
		{ EControlRepresentation::eKnot, "Knot" },
		{ EControlRepresentation::eSpinBox, "Spin Box" },
		{ 0, NULL }
	};

Enum::tValue OutControlSenderConfig::EControlRepresentation::sDefault = OutControlSenderConfig::EControlRepresentation::eVerticalSlider;

void OutControlSenderConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
	SetMin( 0.0 );
	SetDefault( 0.0 );
	SetMax( 1.0 );
	SetStep(1.0);
}

OutControlSender::OutControlSender()
	: mOutput("out", this)
{
	OutControlSenderConfig cfg;
	Configure(cfg);
}

OutControlSender::OutControlSender( const OutControlSenderConfig & cfg)
	: mOutput( "out", this )
{
	Configure(cfg);
}


bool OutControlSender::Do()
{
	if( !AbleToExecute() ) return true;
	return true;
}

bool OutControlSender::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	if(mConfig.GetMin() > mConfig.GetMax() )
	{
		mStatus += " min value greater than max";
		return false;
	}
	if((mConfig.GetDefault() > mConfig.GetMax()) || (mConfig.GetDefault() < mConfig.GetMin()))
	{
		mStatus += " default value out of range";
		return false;
	}
	if(mConfig.GetStep() == 0 )
	{
		mStatus += " step value equal to 0";
		return false;
	}

	mOutput.SendControl( mConfig.GetDefault() );
	return true;
}

} // namespace CLAM



