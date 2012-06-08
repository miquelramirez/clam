
#include "SMSConfigurator.hxx"

namespace QtSMS
{

SMSConfiguratorFactory& SMSConfiguratorFactory::GetInstance()
{	
	static SMSConfiguratorFactory theInstance;
	return theInstance;
}

SMSConfigurator::SMSConfigurator()
{
}


SMSConfigurator::~SMSConfigurator()
{
}
	
} // namespace QtSMS

