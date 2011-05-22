#include <boost/python.hpp>
#include <CLAM/ProcessingConfig.hxx>
#include "DummyConfigurations.hxx"

namespace py = boost::python;

CLAM::ProcessingConfig * DummyDTWithJustStrings()
{
	CLAM::ProcessingConfig * config = new IpyclamTests::DummyDTWithJustStrings;
	return config;
}

BOOST_PYTHON_MODULE(CppTestFixtures)
{
	using namespace boost::python;
	using namespace CLAM;

	class_<ProcessingConfig, boost::noncopyable>("ProcessingConfig", no_init);

	def("DummyDTWithJustStrings", DummyDTWithJustStrings, return_value_policy<manage_new_object>());
}
