#include <CLAM/ProcessingConfig.hxx>

class ConfigurationProxy
{
public:
	CLAM::ProcessingConfig * _processingConfig;
	ConfigurationProxy() {}
	virtual ~ConfigurationProxy() {}
};