#include "SMS_Configurator.hxx"

namespace CLAMVM
{
	SMSConfigurator::SMSConfigurator()
	{
		UserEditedParameters.Wrap( this, &SMSConfigurator::OnChangeNotification );
	}

	SMSConfigurator::~SMSConfigurator()
	{
	}

	void SMSConfigurator::OnChangeNotification()
	{
		ConfigurationChanged.Emit();
	}
	
}
