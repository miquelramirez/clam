#include <boost/python.hpp>
#include <CLAM/Network.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/XMLStorage.hxx>
#include "ConfigurationProxy.hxx"

namespace py = boost::python;

int getAttributeIndex(ConfigurationProxy & config, const std::string & attribute)
{
	for(unsigned int i = 0; i < config._processingConfig->GetNDynamicAttributes(); ++i)
	{
		if ( attribute == config._processingConfig->GetDynamicAttributeName(i) )
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
	if ( config._processingConfig->GetTypeId(index) == typeid(std::string) )
		return Py_BuildValue("s", (*(std::string *) config._processingConfig->GetAttributeAsVoidPtr(index)).c_str() );
	if ( config._processingConfig->GetTypeId(index) == typeid(int) )
		return Py_BuildValue("i", *(int *) config._processingConfig->GetAttributeAsVoidPtr(index) );
	if ( config._processingConfig->GetTypeId(index) == typeid(char) )
		return Py_BuildValue("c", *(char *) config._processingConfig->GetAttributeAsVoidPtr(index) );
	if ( config._processingConfig->GetTypeId(index) == typeid(bool) )
		return Py_BuildValue("b", *(bool *) config._processingConfig->GetAttributeAsVoidPtr(index) );
	if ( config._processingConfig->GetTypeId(index) == typeid(float) )
		return Py_BuildValue("f", *(float *) config._processingConfig->GetAttributeAsVoidPtr(index) );
	if ( config._processingConfig->GetTypeId(index) == typeid(double) )
		return Py_BuildValue("d", *(double *) config._processingConfig->GetAttributeAsVoidPtr(index) );
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
