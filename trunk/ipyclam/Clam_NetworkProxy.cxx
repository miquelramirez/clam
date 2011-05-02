#include <boost/python.hpp>
#include <CLAM/Network.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/OutControl.hxx>


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

//Helper to build a Python List of Tuples containing the peers
py::list extractPeers(CLAM::Network::NamesList peers)
{
	py::list connectorPeers;
	for(unsigned int i = 0; i < peers.size(); ++i)
	{
		std::list<std::string>::iterator it;
		for(it=peers.begin(); it!=peers.end(); it++)
		{
			size_t tokenPosition = (*it).find(".");
			py::tuple peer = py::make_tuple( (*it).substr(0, tokenPosition), (*it).substr(tokenPosition + 1) );
			connectorPeers.append( peer );
		}
	}
	return connectorPeers;
}

py::list processingTypes(CLAM::Network & network)
{
	CLAM::ProcessingFactory::Keys types;
	CLAM::ProcessingFactory::GetInstance().GetRegisteredNames(types);
	return pythonizeList(types);
}

void addProcessing(CLAM::Network & network, const std::string & type, const std::string & processingName)
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

bool hasProcessing(CLAM::Network & network, const std::string & processingName)
{
	return network.HasProcessing(processingName);
}

std::string processingType(CLAM::Network & network, const std::string & processingName)
{
	return network.GetProcessing(processingName).GetClassName();
}

bool processingHasConnector(CLAM::Network & network, const std::string & processingName, const std::string & kind, const std::string & direction, const std::string & connectorName)
{
	if (kind == "Port")
	{
		if (direction == "In")
			return network.GetProcessing(processingName).HasInPort(connectorName);
		else
			return network.GetProcessing(processingName).HasOutPort(connectorName);
	}
	else
	{
		if (direction == "In")
			return network.GetProcessing(processingName).HasInControl(connectorName);
		else
			return network.GetProcessing(processingName).HasOutControl(connectorName);
	}
}

std::string getDescription(CLAM::Network & network)
{
	return network.GetDescription();
}

void setDescription(CLAM::Network & network, const std::string & description)
{
	network.SetDescription(description);
}

py::list processingConnectors(CLAM::Network & network, const std::string & processingName, const std::string & kind, const std::string & direction)
{
	py::list connectors;
	CLAM::Processing & proc = network.GetProcessing(processingName);
	if (kind == "Port")
	{
		if (direction == "In")
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
		if (direction == "In")
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

bool connect(CLAM::Network & network, const std::string & kind, const std::string & fromProcessing, const std::string &fromConnector, const std::string & toProcessing, const std::string & toConnector)
{
	
	//TODO: Needs all the checks that the Dummy Proxy has
	
	const std::string producer = fromProcessing + "." + fromConnector;
	const std::string consumer = toProcessing + "." + toConnector;

	if (kind == "Port")
		return network.ConnectPorts( producer, consumer );
	else
		return network.ConnectControls( producer, consumer );
}

std::string connectorType(CLAM::Network & network, const std::string & processingName, const std::string & kind, const std::string & direction, const std::string & connectorName)
{
	const std::string connector = processingName + "." + connectorName;
	if (kind == "Port")
	{
		if (direction == "In")
		{
			CLAM::InPortBase & inport = network.GetInPortByCompleteName(connector);
			return inport.GetTypeId().name();
		}
		else
		{
			CLAM::OutPortBase & outport = network.GetOutPortByCompleteName(connector);
			return outport.GetTypeId().name();
		}
	}
	else
	{
		if (direction == "In")
		{
			CLAM::InControlBase & incontrol = network.GetInControlByCompleteName(connector);
			return incontrol.GetTypeId().name();
		}
		else
		{
			CLAM::OutControlBase & outcontrol = network.GetOutControlByCompleteName(connector);
			return outcontrol.GetTypeId().name();
		}
	}
}

bool connectionExists(CLAM::Network & network, const std::string & kind, const std::string & fromProcessing, const std::string &fromConnector, const std::string & toProcessing, const std::string & toConnector)
{
	const std::string producer = fromProcessing + "." + fromConnector;
	const std::string consumer = toProcessing + "." + toConnector;

	if (kind == "Port")
	{
		CLAM::OutPortBase & outport = network.GetOutPortByCompleteName(producer);
		CLAM::InPortBase & inport = network.GetInPortByCompleteName(consumer);
		return outport.IsVisuallyConnectedTo(inport);
	}
	else
	{
		CLAM::OutControlBase & outcontrol = network.GetOutControlByCompleteName(producer);
		CLAM::InControlBase & incontrol = network.GetInControlByCompleteName(consumer);
		return outcontrol.IsConnectedTo(incontrol);
	}
}

py::list connectorPeers(CLAM::Network & network, const std::string & processingName, const std::string & kind, const std::string & direction, const std::string & connectorName)
{
	py::list connectorPeers;
	const std::string connector = processingName + "." + connectorName;

	if (kind == "Port")
	{
		if (direction == "In")
		{
			CLAM::InPortBase & inport = network.GetInPortByCompleteName(connector);
			CLAM::OutPortBase * outport = inport.GetVisuallyConnectedOutPort();
			py::tuple peerTuple = py::make_tuple( network.GetProcessingName( *outport->GetProcessing() ), outport->GetName() );
			connectorPeers.append(peerTuple);
			return connectorPeers;
		}
		else
		{
			CLAM::Network::NamesList peers = network.GetInPortsConnectedTo(connector);
			return extractPeers(peers);
		}
	}
	else
	{
		if (direction == "In")
		{
			return connectorPeers;
		}
		else
		{
			CLAM::Network::NamesList peers = network.GetInControlsConnectedTo(connector);
			return extractPeers(peers);
		}
	}
}

bool disconnect(CLAM::Network & network, const std::string & kind, const std::string & fromProcessing, const std::string &fromConnector, const std::string & toProcessing, const std::string & toConnector)
{
	const std::string producer = fromProcessing + "." + fromConnector;
	const std::string consumer = toProcessing + "." + toConnector;

	if (kind == "Port")
		return network.DisconnectPorts( producer, consumer );
	else
		return network.DisconnectControls( producer, consumer );
}
/*
	TODO: Untested non-working code
*/
py::dict processingConfig(CLAM::Network & network, const std::string & processingName)
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
		.def("connect",
			connect,
			"Connects an outconnector with an inconnector"
			)
		.def("connectorType",
			connectorType,
			"Returns the type of the connector"
			)
		.def("connectionExists",
			connectionExists,
			"Returns true if the two connectors are connected. False otherwise"
			)
		.def("connectorPeers",
			connectorPeers,
			"Returns the list of peers from the connector"
			)
		.def("disconnect",
			disconnect,
			"Diconnects an outconnector from an inconnector"
			)
		.def("processingConfig",
			processingConfig, //TODO: Fake implementation for processingType
			"Returns the config dictionary of the processing"
			)
		;
}
