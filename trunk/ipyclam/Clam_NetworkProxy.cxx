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

bool hasProcessing(CLAM::Network & network, char * processingName)
{
	return network.HasProcessing(processingName);
}

std::string processingType(CLAM::Network & network, char * processingName)
{
	return network.GetProcessing(processingName).GetClassName();
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

std::string getDescription(CLAM::Network & network)
{
	return network.GetDescription();
}

void setDescription(CLAM::Network & network, char * description)
{
	network.SetDescription(description);
}

py::list processingConnectors(CLAM::Network & network, char * processingName, char * kind, char * direction)
{
	py::list connectors;
	CLAM::Processing & proc = network.GetProcessing(processingName);
	if (strcmp(kind, "Port") == 0)
	{
		if (strcmp(direction, "In") == 0)
		{
			for(unsigned int i = 0; i < proc.GetNInPorts(); ++i)
			{
				connectors.append(proc.GetInPort(i).GetName());
			}
			return connectors;
		}	
		else
		{
			for(unsigned int i = 0; i < proc.GetNOutPorts(); ++i)
			{
				connectors.append(proc.GetOutPort(i).GetName());
			}
			return connectors;
		}
	}
	else
	{
		if (strcmp(direction, "In") == 0)
		{
			for(unsigned int i = 0; i < proc.GetNInControls(); ++i)
			{
				connectors.append(proc.GetInControl(i).GetName());
			}
			return connectors;
		}
		else
		{
			for(unsigned int i = 0; i < proc.GetNOutControls(); ++i)
			{
				connectors.append(proc.GetOutControl(i).GetName());
			}
			return connectors;
		}
	}
}

/*
	TODO: Untested non-working code
*/
py::dict processingConfig(CLAM::Network & network, char * processingName)
{
	py::dict processingConfig;
	return processingConfig;
}
/*
	End of non-working code
*/

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
		.def("processingNames",
			processingNames,
			"Returns the names of all the processing modules"
			)
		.def("hasProcessing",
			hasProcessing,
			"Tells whether or not there is a processing with the given name on the network"
			)
		.def("processingType",
			processingType,
			"Returns the type of the processing"
			)
		.def("processingHasConnector",
			processingHasConnector,
			"Returns true if the processing has the specified connector"
			)
		.def("getDescription",
			getDescription,
			"Returns the description of the network"
			)
		.def("setDescription",
			setDescription,
			"Sets the description of the network"
			)
		.def("processingConnectors",
			processingConnectors,
			"Returns a list of the connectors of certain kind and direction from a Processing"
			)
		.def("processingConfig",
			processingConfig, //TODO: Fake implementation for processingType
			"Returns the config dictionary of the processing"
			)
		;
}
