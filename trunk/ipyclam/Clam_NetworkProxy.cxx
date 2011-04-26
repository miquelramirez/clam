#include <boost/python.hpp>
#include <CLAM/Network.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/XMLStorage.hxx>


namespace py = boost::python;

std::string Dump(CLAM::Network & net)
{
	std::ostringstream str;
	CLAM::XMLStorage::Dump(net, "Network", str);
	return str.str();
}

/// Helper to turn an stl list into a python list
py::list pythonizeList(std::list<std::string> & list)
{
	py::list result; 
	for (std::list<std::string>::iterator it=list.begin(); it!=list.end(); it++)
		result.append(*it);
	return result;
}

py::list processingTypes(CLAM::Network & network)
{
	CLAM::ProcessingFactory::Keys types;
	CLAM::ProcessingFactory::GetInstance().GetRegisteredNames(types);
	return pythonizeList(types);
}

void addProcessing(CLAM::Network & network, char * type, char * processingName)
{
	network.AddProcessing(processingName, type);
}

BOOST_PYTHON_MODULE(Clam_NetworkProxy)
{
	// Keep 'using namespace' in the inner scope
	using namespace boost::python;
	using namespace CLAM;

	typedef const std::string & cstringref;
	
	class_<Processing, boost::noncopyable>("Processing", no_init);

	class_<Network>("Clam_NetworkProxy")
		.def("xml",
			Dump,
			"Returns the xml representation" )
		.def("availableTypes",
			processingTypes,
			"Returns the names of the processing types than can be added to the network"
			)
		.def("addProcessing",
			addProcessing,
			"Adds a Processing with the name and type specified to the network"
			)
		;
}
