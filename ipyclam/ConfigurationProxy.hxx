#include <CLAM/Component.hxx>
#include <CLAM/ProcessingConfig.hxx>

class ConfigurationProxy
{
	
	const CLAM::ProcessingConfig & config() const
	{
		return *_processingConfig;
	}
public:
	CLAM::ProcessingConfig * _processingConfig;
	CLAM::ProcessingConfig * _processingConfigDefault;

	ConfigurationProxy() : _processingConfig(0), _processingConfigDefault(0)
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
		_processingConfigDefault = 0;
	}
	template <typename value_type>
	const value_type & attributeValue(unsigned i) const
	{
		CLAM_ASSERT(attributeType(i) == typeid(value_type),
			"Asking for the wrong type of value in configuration");
		return *(value_type *)
			config().GetAttributeAsVoidPtr(i );
	}
	template <typename value_type>
	void setAttributeValue(unsigned i, const value_type & value)
	{
		value_type & attribute = 
			*(value_type *)_processingConfig->GetAttributeAsVoidPtr( i );
		attribute = value;
	}
	const std::type_info & attributeType(unsigned i) const
	{
		return config().GetTypeId(i);
	}
	const char * attributeName(unsigned i) const
	{
		return config().GetDynamicAttributeName(i);
	}
	unsigned nAttributes() const
	{
		return config().GetNDynamicAttributes();
	}
	void setDefaultConfig(const CLAM::ProcessingConfig & prototype)
	{
		_processingConfigDefault = (CLAM::ProcessingConfig*) prototype.DeepCopy();
	}
	template <typename value_type>
	bool nonDefault(unsigned i)
	{
		CLAM_ASSERT(attributeType(i) == typeid(value_type),
			"Asking for the wrong type of value in configuration");
		return !(attributeValue<value_type>(i) == *(value_type *) _processingConfigDefault->GetAttributeAsVoidPtr(i));
	}
	~ConfigurationProxy()
	{
		delete _processingConfig;
		delete _processingConfigDefault;
	}
};
