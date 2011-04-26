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

py::list processingNames(CLAM::Network & network)
{
	std::list<std::string> names;
	CLAM::Network::ProcessingsMap::iterator it;
	for (it=network.BeginProcessings(); it!=network.EndProcessings(); it++)
	{
		names.push_back(it->first);
	}
	return pythonizeList(names);
}

void addProcessing(CLAM::Network & network, char * type, char * processingName)
{
	network.AddProcessing(processingName, type);
}

bool hasProcessing(CLAM::Network & network, char * processingName)
{
	return network.HasProcessing(processingName);
}

std::string processingType(CLAM::Network & network, char * processingName)
{
	return network.GetProcessing(processingName).GetClassName();
}
//TODO: Implement correctly
py::dict processingConfig(CLAM::Network & network, char * processingName)
{
	py::dict processingConfig;
	return processingConfig;
}
//TODO: Implement correctly
py::list processingConnectors(CLAM::Network & network, char * processingName, char * kind, char * direction)
{
	py::list connectors;
	if (strcmp(kind, "Port") == 0)
	{
		if (strcmp(direction, "In") == 0)
			connectors.append("InPort1");
		else
			connectors.append("OutPort1");
	}
	else
	{
		if (strcmp(direction, "In") == 0)
			connectors.append("InControl1");
		else
			connectors.append("OutControl1");
	}
	//network.GetProcessing(processingName).GetInControls();
	return connectors;
}

bool processingHasConnector(CLAM::Network & network, char * processingName, char * kind, char * direction, char * connectorName)
{
	if (strcmp(kind, "Port") == 0)
	{
		if (strcmp(direction, "In") == 0)
			return network.GetProcessing(processingName).HasInPort(connectorName);
		else
			return network.GetProcessing(processingName).HasOutPort(connectorName);
	}
	else
	{
		if (strcmp(direction, "In") == 0)
			return network.GetProcessing(processingName).HasInControl(connectorName);
		else
			return network.GetProcessing(processingName).HasOutControl(connectorName);
	}
}
//TODO: Implement correctly
py::list portConnections(CLAM::Network & network)
{
	py::list portConnections;
	return portConnections;
}
//TODO: Implement correctly
py::list controlConnections(CLAM::Network & network)
{
	py::list controlConnections;
	return controlConnections;
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
		.def("processingNames",
			processingNames,
			"Returns the names of all the processing modules"
			)
		.def("addProcessing",
			addProcessing,
			"Adds a Processing with the name and type specified to the network"
			)
		.def("hasProcessing",
			hasProcessing,
			"Tells whether or not there is a processing with the given name on the network"
			)
		.def("processingType",
			processingType,
			"Returns the type of the processing"
			)
		.def("processingConfig",
			processingConfig, //TODO: Implement correctly
			"Returns the config dictionary of the processing"
			)
		.def("processingConnectors", //TODO: Implement correctly
			processingConnectors,
			""
			)
		.def("processingHasConnector",
			processingHasConnector,
			"Returns true if the processing has the specified connector"
			)
		.def("portConnections", //TODO: Implement correctly
			portConnections,
			""
			)
		.def("controlConnections", //TODO: Implement correctly
			controlConnections,
			""
			)
		;
}
