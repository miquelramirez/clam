#include "ConfigurationProxy.hxx"
#include <boost/python.hpp>

namespace py = boost::python;

void throwPythonError(PyObject * type, std::string errorString)
{
	PyErr_SetString(type, errorString.c_str() );
	py::throw_error_already_set();
}

std::string pythonType(py::object value)
{
	return py::extract<std::string>(value.attr("__class__").attr("__name__"));
}

template <typename Type, typename BridgeType=Type>
class BasicConfigurationPlugin : public ConfigurationProxyPlugin
{
	std::string _typeName;
public:
	BasicConfigurationPlugin(const std::string & typeName)
		: _typeName(typeName)
	{
	}
	virtual bool accepts(const ConfigurationProxy & config, unsigned index)
	{
		return config.attributeType(index) == typeid(Type);
	}
	virtual boost::python::object getAttribute(const ConfigurationProxy & config, unsigned index)
	{
		CLAM_ASSERT(accepts(config,index),
			"Configuration plugins should be accepted by the parameter before using them");
		BridgeType value = config.attributeValue<Type>(index);
		return boost::python::object(value);
	}
	virtual void setAttribute(ConfigurationProxy & config, unsigned index, boost::python::object value)
	{
		CLAM_ASSERT(accepts(config,index),
			"Configuration plugins should be accepted by the parameter before using them");
		py::extract<BridgeType> extractor(value);
		if (not extractor.check())
			throwPythonError(PyExc_TypeError, std::string() +
				"While setting parameter '" + config.attributeName(index) +
				"', cannot convert a Python value of type '" + pythonType(value) +
				"' into a C++ value of type '" + _typeName +
				"'.");

		config.setAttributeValue<Type>(index, Type(extractor()) );
	}
	virtual bool nonDefault(ConfigurationProxy & config, unsigned index)
	{
		return config.nonDefault<Type>(index);;
	}
};
#include <CLAM/AudioInFilename.hxx>
#include <CLAM/AudioOutFilename.hxx>
static BasicConfigurationPlugin<CLAM::AudioOutFilename, std::string> audioFileOutRegistrator("AudioOutFilename");
static BasicConfigurationPlugin<CLAM::AudioInFilename, std::string> audioFileInRegistrator("AudioInFilename");
static BasicConfigurationPlugin<std::string> stringRegistrator("string");
static BasicConfigurationPlugin<int> intRegistrator("int");
static BasicConfigurationPlugin<bool> boolRegistrator("bool");
static BasicConfigurationPlugin<float> floatRegistrator("float");
static BasicConfigurationPlugin<double> doubleRegistrator("double");



