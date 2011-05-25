#include <boost/python.hpp>
#include <CLAM/Network.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/XMLStorage.hxx>
#include "ConfigurationProxy.hxx"

namespace py = boost::python;

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
		;
}
