#include "OutControlSender.hxx"
#include "ProcessingFactory.hxx"


namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "OutControlSender",
		"category", "Controls",
		"description", "OutControlSender",
		"icon", "hslider.png",
		"control_sender_type", typeid(TControlData).name(),
		0
	};
	static FactoryRegistrator<ProcessingFactory, OutControlSender> reg = metadata;
}
	
Enum::tEnumValue OutControlSenderConfig::EControlRepresentation::sEnumValues[] =
	{
		{ EControlRepresentation::eUndetermined, "Undetermined" },
		{ EControlRepresentation::eVerticalSlider, "Vertical Slider" },
		{ EControlRepresentation::eHorizontalSlider, "Horizontal Slider" },
		{ EControlRepresentation::eKnot, "Knot" },
		{ EControlRepresentation::eSpinBox, "Spin Box" },
		{ 0, NULL }
	};

Enum::tEnumValue OutControlSenderConfig::EMapping::sEnumValues[] =
	{
		{ EMapping::eLinear, "Linear" },
		{ EMapping::eInverted, "Inverted" },
		{ EMapping::eLog, "Log" },
		{ EMapping::eReverseLog, "Reverse Log" },
		{ 0, NULL }
	};


Enum::tValue OutControlSenderConfig::EControlRepresentation::sDefault =
	OutControlSenderConfig::EControlRepresentation::eHorizontalSlider;

Enum::tValue OutControlSenderConfig::EMapping::sDefault =
	OutControlSenderConfig::EMapping::eLinear;

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
	, mFirstDoAfterStart(true)
{
	OutControlSenderConfig cfg;
	Configure(cfg);
}

OutControlSender::OutControlSender( const OutControlSenderConfig & cfg)
	: mOutput( "out", this )
	, mFirstDoAfterStart(true)
{
	Configure(cfg);
}


bool OutControlSender::ConcreteStart()
{
	mFirstDoAfterStart=true;
	std::cout << "Start" << std::endl;
	return true;
}

bool OutControlSender::Do()
{
	if( !AbleToExecute() ) return true;
	if (mFirstDoAfterStart)
	{
		std::cout << "First do" << std::endl;
		mFirstDoAfterStart=false;
		mOutput.SendControl( mLastValue );
	}
	return true;
}

void OutControlSender::SendControl(TControlData value)
{
	// TODO: Solve thread boundary here
	mLastValue=value;
	mOutput.SendControl( mLastValue );
}

bool OutControlSender::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	if(mConfig.GetMin() > mConfig.GetMax() )
	{
		AddConfigErrorMessage(" min value greater than max");
		return false;
	}
	if((mConfig.GetDefault() > mConfig.GetMax()) || (mConfig.GetDefault() < mConfig.GetMin()))
	{
		AddConfigErrorMessage(" default value out of range");
		return false;
	}
	if(mConfig.GetStep() == 0 )
	{
		AddConfigErrorMessage(" step value equal to 0");
		return false;
	}
	mLastValue = mConfig.GetDefault();
	return true;
}

} // namespace CLAM



