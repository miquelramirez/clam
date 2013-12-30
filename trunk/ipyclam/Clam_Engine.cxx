#include <boost/python.hpp>
#include <CLAM/Network.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/PANetworkPlayer.hxx>
#include <CLAM/JACKNetworkPlayer.hxx>
#include <CLAM/qtmonitors/QtBinder.hxx>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtGui/QMessageBox>
#include "ConfigurationProxy.hxx"
#include "sipunwrap.hxx"
#include "shibokenunwrap.hxx"
#include <QtUiTools/QUiLoader>
#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <CLAM/XMLStorage.hxx>

namespace py = boost::python;

/// ModificatioNetwork.pyn of boost::python::import
py::object relative_import(py::str name)
{
	char *n = py::extract<char *>(name);
	if (py::scope() == py::object()) return py::import(name);
	py::object globals = py::scope().attr("__dict__");

	py::handle<> module(
		PyImport_ImportModuleLevel(
			n, globals.ptr(), globals.ptr(), 0, -1));
	return py::object(module);
}

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

void throwPythonException(PyObject * type, const std::string & msg)
{
	PyErr_SetString(type, msg.c_str() );
	py::throw_error_already_set();
}

void throwIpyclamException(const std::string & type, const std::string & msg)
{
	py::object exception = relative_import("ipyclam.Exceptions").attr(type.c_str());
	PyErr_SetString(exception.ptr(), msg.c_str() );
	py::throw_error_already_set();
}

void assertProcessingExists(CLAM::Network & network, const std::string & processingName)
{
	if (network.HasProcessing(processingName)) return;
	throwIpyclamException("ProcessingNotFound", processingName);
}

void assertProcessingNameAvailable(CLAM::Network & network, const std::string & processingName)
{
	if (not network.HasProcessing(processingName)) return;
	throwIpyclamException("NameAlreadyExists", processingName);
}

void assertTypeExists(CLAM::Network & network, const std::string & typeName)
{
	if (CLAM::ProcessingFactory::GetInstance().KeyExists(typeName)) return;
	throwIpyclamException("BadProcessingType", typeName);
}

py::tuple connectorTuple(const std::string & name)
{
	size_t tokenPosition = name.find(".");
	// TODO: Handle no point at all
	// TODO: Handle point at the end
	// TODO: Handle point at the begining
	return py::make_tuple( 
		name.substr(0, tokenPosition), 
		name.substr(tokenPosition + 1)
		);
}

//Helper to build a Python List of Tuples containing the peers
py::list extractPeers(CLAM::Network::NamesList peers)
{
	py::list connectorPeers;
	std::list<std::string>::iterator it;
	for(it=peers.begin(); it!=peers.end(); it++)
	{
		connectorPeers.append( connectorTuple(*it) );
	}
	return connectorPeers;
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

void addProcessing(CLAM::Network & network, const std::string & type, const std::string & processingName)
{
	assertProcessingNameAvailable(network, processingName);
	assertTypeExists(network, type);
	network.AddProcessing(processingName, type);
}

bool hasProcessing(CLAM::Network & network, const std::string & processingName)
{
	return network.HasProcessing(processingName);
}

std::string processingType(CLAM::Network & network, const std::string & processingName)
{
	assertProcessingExists(network, processingName);
	return network.GetProcessing(processingName).GetClassName();
}

bool processingHasConnector(CLAM::Network & network, const std::string & processingName, const std::string & kind, const std::string & direction, const std::string & connectorName)
{
	assertProcessingExists(network, processingName);
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

void assertConnectorExists(CLAM::Network & network,
		const std::string & processingName,
		const std::string & kind,
		const std::string & direction,
		const std::string & name
		)
{
	if ( processingHasConnector(network, processingName, kind, direction, name) )
		return;
	py::object exception = relative_import("ipyclam.Exceptions").attr("ConnectorNotFound");
	py::object errorArgs = py::make_tuple(
		py::object(processingName),
		py::object(kind),
		py::object(direction),
		py::object(name));
	PyErr_SetObject(exception.ptr(), errorArgs.ptr() );
	py::throw_error_already_set();
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
				connectors.append(proc.GetInPort(i).GetName());
		}	
		else
		{
			for(unsigned int i = 0; i < proc.GetNOutPorts(); ++i)
				connectors.append(proc.GetOutPort(i).GetName());
		}
	}
	else
	{
		if (direction == "In")
		{
			for(unsigned int i = 0; i < proc.GetNInControls(); ++i)
				connectors.append(proc.GetInControl(i).GetName());
		}
		else
		{
			for(unsigned int i = 0; i < proc.GetNOutControls(); ++i)
				connectors.append(proc.GetOutControl(i).GetName());
		}
	}
	return connectors;
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

bool areConnectable(CLAM::Network & network, const std::string & kind, const std::string & fromProcessing, const std::string &fromConnector, const std::string & toProcessing, const std::string & toConnector)
{
	const std::string producer = fromProcessing + "." + fromConnector;
	const std::string consumer = toProcessing + "." + toConnector;

	assertConnectorExists(network, fromProcessing, kind, "Out", fromConnector);
	assertConnectorExists(network, toProcessing, kind, "In", toConnector);

	if (kind == "Port")
	{
		CLAM::OutPortBase & outport = network.GetOutPortByCompleteName(producer);
		CLAM::InPortBase & inport = network.GetInPortByCompleteName(consumer);
		return outport.IsConnectableTo(inport);
	}
	else
	{
		CLAM::OutControlBase & outcontrol = network.GetOutControlByCompleteName(producer);
		CLAM::InControlBase & incontrol = network.GetInControlByCompleteName(consumer);
		return outcontrol.IsLinkable(incontrol);
	}
}

bool connect(CLAM::Network & network, const std::string & kind, const std::string & fromProcessing, const std::string &fromConnector, const std::string & toProcessing, const std::string & toConnector)
{
	const std::string producer = fromProcessing + "." + fromConnector;
	const std::string consumer = toProcessing + "." + toConnector;
	
	assertConnectorExists(network, fromProcessing, kind, "Out", fromConnector);
	assertConnectorExists(network, toProcessing, kind, "In", toConnector);

	if ( connectionExists(network, kind, fromProcessing, fromConnector, toProcessing, toConnector) )
	{
		std::string errorMsg = producer + " and "+ consumer + " already connected";
		throwPythonException(PyExc_AssertionError, errorMsg);
	}
	if ( not areConnectable(network, kind, fromProcessing, fromConnector, toProcessing, toConnector) )
	{
		std::string errorMsg = producer + " and "+ consumer + " have incompatible types";
		throwPythonException(PyExc_AssertionError, errorMsg);
	}

	if (kind == "Port")
		return network.ConnectPorts( producer, consumer );
	else
		return network.ConnectControls( producer, consumer );
}

std::string connectorType(CLAM::Network & network, const std::string & processingName, const std::string & kind, const std::string & direction, const std::string & connectorName)
{
	assertConnectorExists(network, processingName, kind, direction, connectorName);
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

py::list connectorPeers(CLAM::Network & network, const std::string & processingName, const std::string & kind, const std::string & direction, const std::string & connectorName)
{
	// TODO: assertConnectorExists
	const std::string connector = processingName + "." + connectorName;
	if (kind == "Port")
	{
		if (direction == "In")
		{
			py::list connectorPeers;
			CLAM::InPortBase & inport = network.GetInPortByCompleteName(connector);
			CLAM::OutPortBase * outport = inport.GetVisuallyConnectedOutPort();
			if (!outport)
				return connectorPeers;
			py::tuple peerTuple = py::make_tuple( network.GetProcessingName( *outport->GetProcessing() ), outport->GetName() );
			connectorPeers.append(peerTuple);
			return connectorPeers;
		}
		else
		{
			return extractPeers(network.GetInPortsConnectedTo(connector));
		}
	}
	else
	{
		if (direction == "In")
		{
			return extractPeers(network.GetOutControlsConnectedTo(connector));
		}
		else
		{
			return extractPeers(network.GetInControlsConnectedTo(connector));
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

int connectorIndex(CLAM::Network & network, const std::string & processingName, const std::string & kind, const std::string & direction, const std::string & connectorName)
{
	// TODO: assertConnectorExists{
	CLAM::Processing & proc = network.GetProcessing(processingName);
	const std::string connector = processingName + "." + connectorName;
	if (kind == "Port")
	{
		if (direction == "In")
		{
			for(unsigned int i = 0; i < proc.GetNInPorts(); ++i)
				if(connectorName == proc.GetInPort(i).GetName())
					return i;
		}
		else
		{
			for(unsigned int i = 0; i < proc.GetNOutPorts(); ++i)
				if(connectorName == proc.GetOutPort(i).GetName())
					return i;
		}
	}
	else
	{
		if (direction == "In")
		{
			for(unsigned int i = 0; i < proc.GetNInControls(); ++i)
				if(connectorName == proc.GetInControl(i).GetName())
					return i;
		}
		else
		{
			for(unsigned int i = 0; i < proc.GetNOutControls(); ++i)
				if(connectorName == proc.GetOutControl(i).GetName())
					return i;
		}
	}
	return -1;
}

py::list portConnections(CLAM::Network & network)
{
	py::list portConnections;
	CLAM::Network::ProcessingsMap::iterator it;
	for (it=network.BeginProcessings(); it!=network.EndProcessings(); it++)
	{
		for(unsigned int i = 0; i < it->second->GetNOutPorts(); ++i)
		{
			CLAM::OutPortBase::InPortsList::iterator itInPort;
			std::string processingName = it->first;
			CLAM::OutPortBase & outport = it->second->GetOutPort(i);
			for (itInPort=outport.BeginVisuallyConnectedInPorts();
				itInPort!=outport.EndVisuallyConnectedInPorts();
				itInPort++)
			{
				CLAM::InPortBase & inPort = **itInPort;
				py::tuple peer = py::make_tuple(
					processingName,
					outport.GetName(),
					network.GetProcessingName(*inPort.GetProcessing()),
					inPort.GetName()
				);
				portConnections.append(peer);
			}
		}
	}
	return portConnections;
}

py::list controlConnections(CLAM::Network & network)
{
	py::list controlConnections;
	CLAM::Network::ProcessingsMap::iterator it;
	for (it=network.BeginProcessings(); it!=network.EndProcessings(); it++)
	{
		for(unsigned int i = 0; i < it->second->GetNOutControls(); ++i)
		{
			CLAM::OutControlBase::Peers::iterator itInControl;
			std::string processingName = it->first;
			CLAM::OutControlBase & outcontrol = it->second->GetOutControl(i);
			for (itInControl=outcontrol.BeginInControlsConnected();
				itInControl!=outcontrol.EndInControlsConnected();
				itInControl++)
			{
				CLAM::InControlBase & incontrol = **itInControl;
				py::tuple peer = py::make_tuple(
					processingName,
					outcontrol.GetName(),
					network.GetProcessingName(*incontrol.GetProcessing()),
					incontrol.GetName() );
				controlConnections.append(peer);
			}
		}
	}
	return controlConnections;
}

void renameProcessing(CLAM::Network & network, const std::string & oldName, const std::string & newName)
{
	assertProcessingExists(network, oldName);
	assertProcessingNameAvailable(network, newName);
	if ( !network.RenameProcessing( oldName, newName ) )
	{
		std::string errorMsg = "Unable to rename '"+oldName+"' as '" + newName + "' already exists";
		PyErr_SetString(PyExc_RuntimeError, errorMsg.c_str() );
		py::throw_error_already_set();
	}
}

void deleteProcessing(CLAM::Network & network, const std::string & processingName)
{
	assertProcessingExists(network, processingName);
	network.RemoveProcessing(processingName);
}

ConfigurationProxy * processingConfig(CLAM::Network & network, const std::string & processingName)
{
	CLAM::Processing & processing = network.GetProcessing(processingName);
	return new ConfigurationProxy(processing);
}

void play(CLAM::Network & network)
{
	static const std::string problem = "Unable to play the network: ";
	if ( network.IsEmpty() )
	{
		std::string cause = "A network without processings is not playable.";
		throwPythonException(PyExc_RuntimeError, problem+cause);
		return;
	}
	if ( network.HasMisconfiguredProcessings() )
	{
		std::string cause = "Not all the processings are properly configured.";
		throwPythonException(PyExc_RuntimeError, problem+cause);
		return;
	}
	if ( network.HasUnconnectedInPorts() )
	{
		std::string cause = "Some inports in the network are not connected.";
		throwPythonException(PyExc_RuntimeError, problem+cause);
		return;
	}
	network.Start();
}

// need a prefix since 'pause' is a POSIX call
void network_pause(CLAM::Network & network)
{
	network.Pause();
}

void stop(CLAM::Network & network)
{
	network.Stop();
}

bool isStopped(CLAM::Network & network)
{
	return network.IsStopped();
}

bool isPlaying(CLAM::Network & network)
{
	return network.IsPlaying();
}

bool isPaused(CLAM::Network & network)
{
	return network.IsPaused();
}

void setBackend(CLAM::Network & network, const std::string & backend)
{
	CLAM::NetworkPlayer * player = 0;
	if (backend == "JACK")
		player = new CLAM::JACKNetworkPlayer();
	else if (backend == "PortAudio")
		player = new CLAM::PANetworkPlayer();
	else
		throwPythonException(PyExc_RuntimeError, "No such backend.");

	if (not player->IsWorking())
	{
		std::string reason = player->NonWorkingReason();
		delete player;
		throwPythonException(PyExc_RuntimeError,
			"Unable to set backend:" + reason);
	}
	network.SetPlayer( player );
}

void bindUi(CLAM::Network & network, PyObject * object)
{
	QObject * qobject = 0;
	if (not qobject) qobject = (QObject*) shibokenUnwrap(object);
	if (not qobject) qobject = (QObject*) sipUnwrap(object);
	if (not qobject)
		std::cerr << "Not a QObject" << std::endl;

	QStringList errors;
	CLAM::QtBinder::bindAllBinders(qobject, network, errors);
	if (errors.empty()) return;
	QMessageBox::warning((QWidget*)qobject, 
		QObject::tr("Error binding interface"),
		errors.join("\n"));
}

void load(CLAM::Network & network, const std::string & filename)
{
	try
	{
		CLAM::XMLStorage::Restore(network, filename);
	}
	catch(CLAM::XmlStorageErr &e)
	{
		std::string reason = e.what();
		throwPythonException(PyExc_RuntimeError,
			"Unable to load the network: " + reason);
	}
}

void save(CLAM::Network & network, const std::string & filename)
{
	CLAM::XMLStorage::Dump(network, "network", filename);
}


BOOST_PYTHON_MODULE(Clam_Engine)
{
	// Keep 'using namespace' in the inner scope
	using namespace boost::python;
	using namespace CLAM;

	relative_import("Clam_ConfigurationProxy");

	class_<Processing, boost::noncopyable>("Processing", no_init);

	
	class_<Network>("Clam_Engine")
		.def("xml",
			Dump,
			"Returns the xml representation" )
		.def("load",
			load,
			"Loads a Network from an xml file."
			)
		.def("save",
			save,
			"Saves the Network on an xml file."
			)
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
		.def("connectorIndex",
			connectorIndex,
			"Returns the index of the connector"
			)
		.def("portConnections",
			portConnections,
			"Returns a list of tuples containing the port connections of the network"
			)
		.def("controlConnections",
			controlConnections,
			"Returns a list of tuples containing the control connections of the network"
			)
		.def("renameProcessing",
			renameProcessing,
			"Renames a processing. Returns true if successfull."
			)
		.def("deleteProcessing",
			deleteProcessing,
			"Removes a processing from the network."
			)
		.def("areConnectable",
			areConnectable,
			"Returns true if the two connectors are connectable."
			)
		.def("processingConfig",
			processingConfig,
			return_value_policy<manage_new_object>(),
			"Returns the config dictionary of the processing"
			)
		.def("play",
			play,
			"Starts to play the network or resumes a pause status"
			)
		.def("pause",
			network_pause,
			"Makes the backend to skip the network"
			)
		.def("stop",
			stop,
			"Stops playing the network"
			)
		.def("setBackend",
			setBackend,
			"Sets a backend to play the network"
			)
		.def("isStopped",
			isStopped,
			"Returns true if the network is stooped."
			)
		.def("isPlaying",
			isPlaying,
			"Returns true if the network is playing."
			)
		.def("isPaused",
			isPaused,
			"Returns true if the network is paused."
			)
		.def("bindUi",
			bindUi,
			"Binds..."
			)
		;
}
