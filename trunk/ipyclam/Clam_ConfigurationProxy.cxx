#include <boost/python.hpp>
#include <CLAM/Network.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/XMLStorage.hxx>
#include "ConfigurationProxy.hxx"

namespace py = boost::python;

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
		PyErr_SetString(PyExc_AttributeError, attribute.c_str() );
		py::throw_error_already_set();
	}
	if ( config.attributeType(index) == typeid(std::string) )
		return Py_BuildValue("s", config.attributeValue<std::string>(index).c_str() );
	if ( config.attributeType(index) == typeid(int) )
		return Py_BuildValue("i", config.attributeValue<int>(index));
	if ( config.attributeType(index) == typeid(char) )
		return Py_BuildValue("c", config.attributeValue<char>(index));
	if ( config.attributeType(index) == typeid(bool) )
		return Py_BuildValue("b", config.attributeValue<bool>(index));
	if ( config.attributeType(index) == typeid(float) )
		return Py_BuildValue("f", config.attributeValue<float>(index));
	if ( config.attributeType(index) == typeid(double) )
		return Py_BuildValue("d", config.attributeValue<double>(index));
	return Py_None;
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
			""
			)
		;
}
