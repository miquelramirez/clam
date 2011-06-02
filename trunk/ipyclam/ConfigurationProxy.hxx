#ifndef ConfigurationProxy_hxx
#define ConfigurationProxy_hxx

#include <boost/python.hpp>
#include <CLAM/Component.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Processing.hxx>

std::string pythonType(PyObject * value)
{
	if ( PyString_Check(value) ) return "str";
	if ( PyBool_Check(value) ) return "bool";
	if ( PyInt_Check(value) ) return "int";
	if ( PyFloat_Check(value) ) return "float";
	return "";
}

void throwPythonError(PyObject * type, std::string errorString)
{
	PyErr_SetString(type, errorString.c_str() );
	boost::python::throw_error_already_set();
}

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
	ConfigurationProxy * cloneConfig()
	{
		return new ConfigurationProxy( config() );
	}
	~ConfigurationProxy()
	{
		delete _processingConfig;
	}
};

class ConfigurationProxyPlugin
{
public:
	typedef std::list<ConfigurationProxyPlugin *> Map;
	ConfigurationProxyPlugin()
	{
		GetList().push_back(this);
	}
	virtual ~ConfigurationProxyPlugin() {}
	static Map & GetList()
	{
		static Map map;
		return map;
	}
	static ConfigurationProxyPlugin & GetPlugin(const ConfigurationProxy & object, unsigned index)
	{
		static ConfigurationProxyPlugin nullPlugin;
		for (Map::iterator it = GetList().begin(); it!=GetList().end(); it++)
		{
			if ((*it)->accepts(object,index)) return **it;
		}
		return nullPlugin;
	}
	virtual bool accepts(const ConfigurationProxy & object, unsigned index)
	{
		return true;
	}
	virtual boost::python::object getAttribute(const ConfigurationProxy & object, unsigned index)
	{
		boost::python::object value(0);
		return value;
	}
	virtual void setAttribute(ConfigurationProxy & object, unsigned index, boost::python::object value)
	{}
};

class StringConfigurationProxyPlugin : public ConfigurationProxyPlugin
{
public:
	virtual bool accepts(const ConfigurationProxy & object, unsigned index)
	{
		return object.attributeType(index) == typeid(std::string);
	}
	virtual boost::python::object getAttribute(const ConfigurationProxy & object, unsigned index)
	{
		std::string value = object.attributeValue<std::string>(index);
		return boost::python::object(value);
	}
	virtual void setAttribute(ConfigurationProxy & object, unsigned index, boost::python::object value)
	{
		if (not PyString_Check(value.ptr()) )
			throwPythonError(PyExc_TypeError, "str value expected, got " + pythonType(value.ptr()));

		object.setAttributeValue<std::string>(index, boost::python::extract<std::string>(value) );
	}
};
static StringConfigurationProxyPlugin stringRegistrator;

class IntegerConfigurationProxyPlugin : public ConfigurationProxyPlugin
{
public:
	virtual bool accepts(const ConfigurationProxy & object, unsigned index)
	{
		return object.attributeType(index) == typeid(int);
	}
	virtual boost::python::object getAttribute(const ConfigurationProxy & object, unsigned index)
	{
		int value = object.attributeValue<int>(index);
		return boost::python::object(value);
	}
	virtual void setAttribute(ConfigurationProxy & object, unsigned index, boost::python::object value)
	{
		if (not PyInt_Check(value.ptr()) )
			throwPythonError(PyExc_TypeError, "int value expected, got " + pythonType(value.ptr()));

		object.setAttributeValue<int>(index, boost::python::extract<int>(value) );
	}
};
static IntegerConfigurationProxyPlugin intRegistrator;

class BooleanConfigurationProxyPlugin : public ConfigurationProxyPlugin
{
public:
	virtual bool accepts(const ConfigurationProxy & object, unsigned index)
	{
		return object.attributeType(index) == typeid(bool);
	}
	virtual boost::python::object getAttribute(const ConfigurationProxy & object, unsigned index)
	{
		bool value = object.attributeValue<bool>(index);
		return boost::python::object(value);
	}
	virtual void setAttribute(ConfigurationProxy & object, unsigned index, boost::python::object value)
	{
		if (not PyBool_Check(value.ptr()) )
			throwPythonError(PyExc_TypeError, "bool value expected, got " + pythonType(value.ptr()) );

		object.setAttributeValue<bool>(index, boost::python::extract<bool>(value) );
	}
};
static BooleanConfigurationProxyPlugin boolRegistrator;

class FloatConfigurationProxyPlugin : public ConfigurationProxyPlugin
{
public:
	virtual bool accepts(const ConfigurationProxy & object, unsigned attribute)
	{
		return object.attributeType(attribute) == typeid(float);
	}
	virtual boost::python::object getAttribute(const ConfigurationProxy & object, unsigned index)
	{
		float value = object.attributeValue<float>(index);
		return boost::python::object(value);
	}
	virtual void setAttribute(ConfigurationProxy & object, unsigned index, boost::python::object value)
	{
		if (not PyFloat_Check(value.ptr()) )
			throwPythonError(PyExc_TypeError, "float value expected, got " + pythonType(value.ptr()) );

		object.setAttributeValue<float>(index, boost::python::extract<float>(value) );
	}
};
static FloatConfigurationProxyPlugin floatRegistrator;

class DoubleConfigurationProxyPlugin : public ConfigurationProxyPlugin
{
public:
	virtual bool accepts(const ConfigurationProxy & object, unsigned attribute)
	{
		return object.attributeType(attribute) == typeid(double);
	}
	virtual boost::python::object getAttribute(const ConfigurationProxy & object, unsigned index)
	{
		double value = object.attributeValue<double>(index);
		return boost::python::object(value);
	}
	virtual void setAttribute(ConfigurationProxy & object, unsigned index, boost::python::object value)
	{
		if (not PyFloat_Check(value.ptr()) )
			throwPythonError(PyExc_TypeError, "double value expected, got " + pythonType(value.ptr()) );

		object.setAttributeValue<double>(index, boost::python::extract<double>(value) );
	}
};
static DoubleConfigurationProxyPlugin doubleRegistrator;

#endif
