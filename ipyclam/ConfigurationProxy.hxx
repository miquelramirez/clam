#include <CLAM/Component.hxx>
#include <CLAM/ProcessingConfig.hxx>

class ConfigurationProxy
{
public:
	CLAM::ProcessingConfig * _processingConfig;
	ConfigurationProxy() {}
	ConfigurationProxy(const CLAM::ProcessingConfig & prototype)
	{
		_processingConfig = (CLAM::ProcessingConfig*) prototype.DeepCopy();
	}
	~ConfigurationProxy()
	{
		delete _processingConfig;
	}
};