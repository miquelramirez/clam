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

OutControlSender::OutControlSender( const OutControlSenderConfig & cfg)
	: _output( "out", this )
	, _firstDoAfterStart(true)
{
	Configure(cfg);
}


bool OutControlSender::ConcreteStart()
{
	_firstDoAfterStart=true;
	return true;
}

bool OutControlSender::Do()
{
	if (_firstDoAfterStart)
	{
		_firstDoAfterStart=false;
		_output.SendControl( _lastValue );
	}
	return true;
}

void OutControlSender::SendControl(TControlData value)
{
	// TODO: Solve thread boundary here
	_lastValue=value;
	_output.SendControl( _lastValue );
}

bool OutControlSender::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(_config, c);

	if (_config.GetMin() > _config.GetMax() )
		return AddConfigErrorMessage("Min value greater than max");

	if ((_config.GetDefault() > _config.GetMax()) || (_config.GetDefault() < _config.GetMin()))
		return AddConfigErrorMessage("Default value out of range");

	if (_config.GetStep() == 0 )
		return AddConfigErrorMessage("Step value equal to 0");

	_lastValue = _config.GetDefault();
	return true;
}

} // namespace CLAM



