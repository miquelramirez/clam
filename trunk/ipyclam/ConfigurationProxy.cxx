#include "ConfigurationProxy.hxx"

void throwPythonError(PyObject * type, std::string errorString)
{
	PyErr_SetString(type, errorString.c_str() );
	boost::python::throw_error_already_set();
}

std::string pythonType(PyObject * value)
{
	if ( PyString_Check(value) ) return "str";
	if ( PyBool_Check(value) ) return "bool";
	if ( PyInt_Check(value) ) return "int";
	if ( PyFloat_Check(value) ) return "float";
	return "";
}

class StringConfigurationProxyPlugin : public ConfigurationProxyPlugin
{
public:
	virtual bool accepts(const ConfigurationProxy & config, unsigned index)
	{
		return config.attributeType(index) == typeid(std::string);
	}
	virtual boost::python::object getAttribute(const ConfigurationProxy & config, unsigned index)
	{
		std::string value = config.attributeValue<std::string>(index);
		return boost::python::object(value);
	}
	virtual void setAttribute(ConfigurationProxy & config, unsigned index, boost::python::object value)
	{
		if (not PyString_Check(value.ptr()) )
			throwPythonError(PyExc_TypeError, "str value expected, got " + pythonType(value.ptr()));

		config.setAttributeValue<std::string>(index, boost::python::extract<std::string>(value) );
	}
	virtual bool nonDefault(ConfigurationProxy & config, unsigned index)
	{
		return config.nonDefault<std::string>(index);;
	}
};
static StringConfigurationProxyPlugin stringRegistrator;

class IntegerConfigurationProxyPlugin : public ConfigurationProxyPlugin
{
public:
	virtual bool accepts(const ConfigurationProxy & config, unsigned index)
	{
		return config.attributeType(index) == typeid(int);
	}
	virtual boost::python::object getAttribute(const ConfigurationProxy & config, unsigned index)
	{
		int value = config.attributeValue<int>(index);
		return boost::python::object(value);
	}
	virtual void setAttribute(ConfigurationProxy & config, unsigned index, boost::python::object value)
	{
		if (not PyInt_Check(value.ptr()) )
			throwPythonError(PyExc_TypeError, "int value expected, got " + pythonType(value.ptr()));

		config.setAttributeValue<int>(index, boost::python::extract<int>(value) );
	}
	virtual bool nonDefault(ConfigurationProxy & config, unsigned index)
	{
		return config.nonDefault<int>(index);;
	}
};
static IntegerConfigurationProxyPlugin intRegistrator;

class BooleanConfigurationProxyPlugin : public ConfigurationProxyPlugin
{
public:
	virtual bool accepts(const ConfigurationProxy & config, unsigned index)
	{
		return config.attributeType(index) == typeid(bool);
	}
	virtual boost::python::object getAttribute(const ConfigurationProxy & config, unsigned index)
	{
		bool value = config.attributeValue<bool>(index);
		return boost::python::object(value);
	}
	virtual void setAttribute(ConfigurationProxy & config, unsigned index, boost::python::object value)
	{
		if (not PyBool_Check(value.ptr()) )
			throwPythonError(PyExc_TypeError, "bool value expected, got " + pythonType(value.ptr()) );

		config.setAttributeValue<bool>(index, boost::python::extract<bool>(value) );
	}
	virtual bool nonDefault(ConfigurationProxy & config, unsigned index)
	{
		return config.nonDefault<bool>(index);;
	}
};
static BooleanConfigurationProxyPlugin boolRegistrator;

class FloatConfigurationProxyPlugin : public ConfigurationProxyPlugin
{
public:
	virtual bool accepts(const ConfigurationProxy & config, unsigned attribute)
	{
		return config.attributeType(attribute) == typeid(float);
	}
	virtual boost::python::object getAttribute(const ConfigurationProxy & config, unsigned index)
	{
		float value = config.attributeValue<float>(index);
		return boost::python::object(value);
	}
	virtual void setAttribute(ConfigurationProxy & config, unsigned index, boost::python::object value)
	{
		if (not PyFloat_Check(value.ptr()) )
			throwPythonError(PyExc_TypeError, "float value expected, got " + pythonType(value.ptr()) );

		config.setAttributeValue<float>(index, boost::python::extract<float>(value) );
	}
	virtual bool nonDefault(ConfigurationProxy & config, unsigned index)
	{
		return config.nonDefault<float>(index);;
	}
};
static FloatConfigurationProxyPlugin floatRegistrator;

class DoubleConfigurationProxyPlugin : public ConfigurationProxyPlugin
{
public:
	virtual bool accepts(const ConfigurationProxy & config, unsigned attribute)
	{
		return config.attributeType(attribute) == typeid(double);
	}
	virtual boost::python::object getAttribute(const ConfigurationProxy & config, unsigned index)
	{
		double value = config.attributeValue<double>(index);
		return boost::python::object(value);
	}
	virtual void setAttribute(ConfigurationProxy & config, unsigned index, boost::python::object value)
	{
		if (not PyFloat_Check(value.ptr()) )
			throwPythonError(PyExc_TypeError, "double value expected, got " + pythonType(value.ptr()) );

		config.setAttributeValue<double>(index, boost::python::extract<double>(value) );
	}
	virtual bool nonDefault(ConfigurationProxy & config, unsigned index)
	{
		return config.nonDefault<double>(index);;
	}
};
static DoubleConfigurationProxyPlugin doubleRegistrator;