#include <CLAM/Component.hxx>
#include <CLAM/ProcessingConfig.hxx>

class ConfigurationProxy
{
public:
	CLAM::ProcessingConfig * _processingConfig;
	ConfigurationProxy() : _processingConfig(0)
	{
		CLAM_ASSERT(false, "Unexpected use of ConfigurationProxy default constructor");
	}
	ConfigurationProxy(const ConfigurationProxy &)
	{
		CLAM_ASSERT(false, "Unexpected use of ConfigurationProxy copy constructor");
	}
	ConfigurationProxy(const CLAM::ProcessingConfig & prototype)
	{
		_processingConfig = (CLAM::ProcessingConfig*) prototype.DeepCopy();
	}
	template <typename value_type>
	const value_type & attributeValue(unsigned i) const
	{
		CLAM_ASSERT(attributeType(i) == typeid(value_type),
			"Asking for the wrong type of value in configuration");
		return *(value_type *)
			_processingConfig->GetAttributeAsVoidPtr(i );
	}
	const std::type_info & attributeType(unsigned i) const
	{
		return _processingConfig->GetTypeId(i);
	}
	const char * attributeName(unsigned i) const
	{
		return _processingConfig->GetDynamicAttributeName(i);
	}
	unsigned nAttributes() const
	{
		return _processingConfig->GetNDynamicAttributes();
	}
	~ConfigurationProxy()
	{
		delete _processingConfig;
	}
};
