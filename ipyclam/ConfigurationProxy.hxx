#include <CLAM/Component.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Processing.hxx>

class ConfigurationProxy
{
	const CLAM::ProcessingConfig & config() const
	{
		if (_processing) return _processing->GetConfig();
		return *_processingConfig;
	}
public:
	CLAM::Processing * _processing;
	CLAM::ProcessingConfig * _processingConfig;

	ConfigurationProxy()
		: _processing(0)
		, _processingConfig(0)
	{
		CLAM_ASSERT(false, "Unexpected use of ConfigurationProxy default constructor");
	}
	ConfigurationProxy(const ConfigurationProxy &)
		: _processing(0)
		, _processingConfig(0)
	{
		CLAM_ASSERT(false, "Unexpected use of ConfigurationProxy copy constructor");
	}

	ConfigurationProxy(CLAM::Processing & processing)
		: _processing(&processing)
		, _processingConfig(0)
	{}
	ConfigurationProxy(const CLAM::ProcessingConfig & prototype)
		: _processing(0)
		, _processingConfig(0)
	{
		_processingConfig = (CLAM::ProcessingConfig*) prototype.DeepCopy();
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
		if (_processing && _processingConfig)
		{
			value_type & attribute = 
				*(value_type *)_processingConfig->GetAttributeAsVoidPtr( i );
			attribute = value;
			return;
		}
		if (_processing)
		{
			CLAM::ProcessingConfig * config = (CLAM::ProcessingConfig*) _processing->GetConfig().DeepCopy();
			value_type & attribute = 
				*(value_type *)config->GetAttributeAsVoidPtr( i );
			attribute = value;
			_processing->Configure(*config);
			delete config;
			return;
		}
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
	template <typename value_type>
	bool nonDefault(unsigned i)
	{
		CLAM_ASSERT(attributeType(i) == typeid(value_type),
			"Asking for the wrong type of value in configuration");
		CLAM::ProcessingConfig * defaultConfig = (CLAM::ProcessingConfig*) config().Species();
		bool nonDefault = !(attributeValue<value_type>(i) == *(value_type *) defaultConfig->GetAttributeAsVoidPtr(i));
		delete defaultConfig;
		return nonDefault;
	}
	void hold()
	{
		_processingConfig = (CLAM::ProcessingConfig*) _processing->GetConfig().DeepCopy();
	}
	void apply()
	{
		_processing->Configure(*_processingConfig);
		delete _processingConfig;
		_processingConfig = 0;
	}
	~ConfigurationProxy()
	{
		delete _processingConfig;
	}
};
