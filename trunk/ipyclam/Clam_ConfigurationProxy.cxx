#include <boost/python.hpp>
#include <CLAM/Network.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include "ConfigurationProxy.hxx"

namespace py = boost::python;

void setConfig(ConfigurationProxy * config, CLAM::ProcessingConfig * processingConfig)
{
	config->_processingConfig = processingConfig;
	std::cout << config->_processingConfig->GetNDynamicAttributes() << std::endl;
}

const void * GetAttributeAsVoidPtr(ConfigurationProxy * config)
{
	return config->_processingConfig->GetAttributeAsVoidPtr(0);
}

BOOST_PYTHON_MODULE(Clam_ConfigurationProxy)
{
	using namespace boost::python;

		class_<ConfigurationProxy>("Clam_ConfigurationProxy")
			.def("setConfig",
				setConfig,
				"" )
/*			.def("GetAttributeAsVoidPtr",
				GetAttributeAsVoidPtr,
				return_internal_reference<>(),
				"" )
*/			;
}