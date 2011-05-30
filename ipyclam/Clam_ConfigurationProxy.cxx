#include <boost/python.hpp>
#include <CLAM/Network.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/XMLStorage.hxx>
#include "ConfigurationProxy.hxx"

namespace py = boost::python;

std::string pythonType(PyObject * value)
{
	if ( PyString_Check(value) )
		return "str";
	if ( PyBool_Check(value) )
		return "bool";
	if ( PyInt_Check(value) )
		return "int";
	if ( PyFloat_Check(value) )
		return "float";
	return "";
}

void throwPythonError(std::string errorString)
{
	PyErr_SetString(PyExc_TypeError, errorString.c_str() );
	py::throw_error_already_set();
}

int getAttributeIndex(const ConfigurationProxy & config, const std::string & attribute)
{
	for(unsigned int i = 0; i < config.nAttributes(); ++i)
	{
		if ( attribute == config.attributeName(i) )
			return (int) i;
	}
	return -1;
}

ConfigurationProxy * createConfigurationProxy(const std::string & type)
{
	CLAM::Processing * proc = CLAM::ProcessingFactory::GetInstance().CreateSafe( type );
	const CLAM::ProcessingConfig & config = proc->GetConfig();

	ConfigurationProxy * configurationProxy = new ConfigurationProxy(config);

	return configurationProxy;
}

std::string Dump(ConfigurationProxy & config)
{
	std::ostringstream str;
	CLAM::XMLStorage::Dump(*config._processingConfig, "Configuration", str);
	return str.str();
}

static PyObject * getAttribute(ConfigurationProxy & config, const std::string & attribute)
{
	int index = getAttributeIndex(config, attribute);
	if (index == -1)
	{
		PyErr_SetString(PyExc_KeyError, attribute.c_str() );
		py::throw_error_already_set();
	}
	if ( config.attributeType(index) == typeid(std::string) )
		return Py_BuildValue("s", config.attributeValue<std::string>(index).c_str() );
	if ( config.attributeType(index) == typeid(int) )
		return Py_BuildValue("i", config.attributeValue<int>(index));
	if ( config.attributeType(index) == typeid(bool) )
		return Py_BuildValue("b", config.attributeValue<bool>(index));
	if ( config.attributeType(index) == typeid(float) )
		return Py_BuildValue("f", config.attributeValue<float>(index));
	if ( config.attributeType(index) == typeid(double) )
		return Py_BuildValue("d", config.attributeValue<double>(index));
	return Py_None;
}

void setAttribute(ConfigurationProxy & config, const std::string & attribute, PyObject * value)
{
	int index = getAttributeIndex(config, attribute);
	if (index == -1)
	{
		PyErr_SetString(PyExc_KeyError, attribute.c_str() );
		py::throw_error_already_set();
	}

	if ( config.attributeType(index) == typeid(std::string) )
	{
		if (not PyString_Check(value) )
			throwPythonError("str value expected, got " + pythonType(value));
		std::string stringvalue = PyString_AsString(value);
		config.setAttributeValue<std::string>(index, stringvalue);
		return;
	}
	if ( config.attributeType(index) == typeid(int) )
	{
		if (not PyInt_Check(value) )
			throwPythonError("int value expected, got " + pythonType(value));
		int intvalue = PyInt_AsLong(value);
		config.setAttributeValue<int>(index, intvalue);
		return;
	}
	//TODO: CHAR
	if ( config.attributeType(index) == typeid(bool) )
	{
		if (not PyBool_Check(value) )
			throwPythonError("bool value expected, got " + pythonType(value));
		bool boolvalue = value==Py_True;
		config.setAttributeValue<bool>(index, boolvalue);
		return;
	}
	if ( config.attributeType(index) == typeid(float) )
	{
		if (not PyFloat_Check(value) )
			throwPythonError("float value expected, got " + pythonType(value));
		float floatvalue = PyFloat_AsDouble(value);
		config.setAttributeValue<float>(index, floatvalue);
		return;
	}
}

py::list keys(ConfigurationProxy & config)
{
	py::list keys;
	for(unsigned int i = 0; i < config.nAttributes(); ++i)
		keys.append(config.attributeName(i));
	return keys;
}

BOOST_PYTHON_MODULE(Clam_ConfigurationProxy)
{
	using namespace boost::python;

	def("createConfigurationProxy",
		createConfigurationProxy,
		return_value_policy<manage_new_object>()
		);

	class_<ConfigurationProxy>("Clam_ConfigurationProxy")
		.def("xml",
			Dump,
			"Returns the xml representation"
			)
		.def("__getitem__",
			getAttribute,
			"Returns the attribute value"
			)
		.def("__setitem__",
			setAttribute,
			"Sets a new value for the attribute"
			)
		.def("keys",
			keys,
			"Returns a list with the keys of the proxy"
			)
		;
}
