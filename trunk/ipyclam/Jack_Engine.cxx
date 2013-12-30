#include <boost/python.hpp>

namespace py = boost::python;

#include <jack/jack.h>
#include <iostream>

class JackEngine
{
	class JackList
	{
		const char ** _list;
		const char ** _iterator;
	public:
		JackList(const char ** list)
			: _list(list)
			, _iterator(list)
		{
		}
		~JackList()
		{
			if (_list) jack_free(_list);
		}
		operator bool() { return _list and *_iterator; }
		const char * operator++(int) {return *_iterator++;}
		const char * operator * () { return *_iterator;}
	};

	class NotImplemented : public std::exception
	{
		std::string _name;
	public:
		NotImplemented(const std::string & name) throw()
			: _name(name)
		{
		}
		virtual ~NotImplemented() throw() {};
		virtual const char* what() const throw()
		{
			return ("Method " + _name + " not implemented yet").c_str();
		}
	};
	class InvalidClient : public std::exception
	{
	public:
		InvalidClient() throw()
		{
		}
		virtual ~InvalidClient() throw() {};
		virtual const char* what() const throw()
		{
			return "Invalid JACK client";
		}
	};
private:
	jack_client_t * _client;
public:
	bool _debug;
	JackEngine()
		:_client(0)
		, _debug(false)
	{
		jack_status_t status;
		_client = jack_client_open("ipyjack", JackNullOption, &status);
		if (not _client)
			throw InvalidClient();
	}
	~JackEngine()
	{
		if (_client) jack_client_close(_client);
	}

	py::list availableTypes() const
	{
		// TODO: No types
		return py::list();
	}

	void addProcessing(
		const std::string & type,
		const std::string & name)
	{
		// TODO: Just ignore or raise unsupported operation
	}

	void renameProcessing(
		const std::string & oldName,
		const std::string & newName)
	{
		// TODO: Just ignore or raise unsupported operation
	}

	void deleteProcessing(
		const std::string & name)
	{
		// TODO: Just ignore or raise unsupported operation
	}

	std::string processingType(const std::string & name)
	{
		if (_debug) std::cerr << "processingType " << name << std::endl;
		return "JackClient"; // Same for all units
	}

	py::object processingConfig(const std::string & name)
	{
		if (_debug) std::cerr << "processingConfig " << name << std::endl;
		return py::dict(); // Dummy empty configuration
	}

	py::list processingNames() const
	{
		if (_debug) std::cerr << "unitNames" << std::endl;
		py::list result;
		for (JackList l = jack_get_ports(_client, 0,0,0);
			l; l++)
		{
			py::object port(*l);
			py::object client = port.attr("split")(":")[0];
			if (result.contains(client)) continue;
			result.append(client);
		}
		return result;
	}

	bool hasProcessing(
		const std::string & name)
	{
		if (_debug) std::cerr << "hasProcessing " << name <<  std::endl;
		std::string nameFilter = name+":*";
		for (JackList l = jack_get_ports(_client, nameFilter.c_str(),0,0);
			l; l++)
		{
			py::object port(*l);
			py::object client = port.attr("split")(":")[0];
			if (client==name) return true;
		}
		return false;
	}

	py::object processingConnectors(
		const std::string & name,
		const std::string & kind,
		const std::string & direction)
	{
		if (_debug) std::cerr << "processingConnectors" << std::endl;
		unsigned long flags = 0;
		if (kind == "Control") return py::list();
		if (direction == "In") flags |= JackPortIsInput;
		if (direction == "Out") flags |= JackPortIsOutput;
		std::string nameFilter = name + ":*";
		py::list result;
		for (JackList l = jack_get_ports(
				_client, nameFilter.c_str(), 0, flags);
			l; l++)
		{
			py::object port(*l);
			py::object connector = port.attr("split")(":")[1];
			result.append(connector);
		}
		return result;
	}
	bool processingHasConnector(
		const std::string & unit,
		const std::string & kind,
		const std::string & direction,
		const std::string & connectorName)
	{
		if (_debug) std::cerr << "processingHasConnector" << std::endl;

		if (kind == "Control") return false;
		jack_port_t * port = findPort(unit, connectorName);
		unsigned long flags = jack_port_flags(port);
		if (direction == "In") return flags | JackPortIsInput;
		if (direction == "Out") return flags | JackPortIsOutput;
		return false;
	}
	bool disconnect(
		const std::string & kind,
		const std::string & fromUnit,
		const std::string & fromConnector,
		const std::string & toUnit,
		const std::string & toConnector)
	{
		if (_debug) std::cerr << "disconnect" << std::endl;
		std::string from = fromUnit + ":" + fromConnector;
		std::string to = toUnit + ":" + toConnector;
		int error = jack_disconnect(_client, from.c_str(), to.c_str());
		return not error;
	}
	bool connect(
		const std::string & kind,
		const std::string & fromUnit,
		const std::string & fromConnector,
		const std::string & toUnit,
		const std::string & toConnector)
	{
		if (_debug) std::cerr << "connect" << std::endl;
		std::string from = fromUnit + ":" + fromConnector;
		std::string to = toUnit + ":" + toConnector;
		int error = jack_connect(_client, from.c_str(), to.c_str());
		return not error;
	}
	bool areConnectable(
		const std::string & kind,
		const std::string & fromUnit,
		const std::string & fromConnector,
		const std::string & toUnit,
		const std::string & toConnector)
	{
		if (_debug) std::cerr << "areConnectable" << std::endl;
		return true;
	}
	std::string connectorIndex(
		const std::string & unit,
		const std::string & kind,
		const std::string & direction,
		const std::string & connectorName)
	{
		if (_debug) std::cerr << "connectorIndex" << unit << std::endl;
		// TODO: Implement this
		throw NotImplemented("connectorIndex");
	}
	std::string connectorType(
		const std::string & unit,
		const std::string & kind,
		const std::string & direction,
		const std::string & connectorName)
	{
		if (_debug) std::cerr << "connectorType" << unit << std::endl;
		jack_port_t * port = findPort(unit, connectorName);
		// TODO: if not port throw
		return jack_port_type(port);
	}
	bool connectionExists(
		const std::string & kind,
		const std::string & fromProcessing,
		const std::string & fromConnector,
		const std::string & toProcessing,
		const std::string & toConnector)
	{
		if (_debug) std::cerr << "connectionExists" << std::endl;
		return false; // TODO
	}
	py::list portConnections()
	{
		if (_debug) std::cerr << "portConnections " << std::endl;
		py::list result;
		for (JackList l = jack_get_ports(_client, 0,0, JackPortIsOutput);
			l;l++)
		{
			jack_port_t * port = jack_port_by_name(_client, *l);
			for (JackList peers = jack_port_get_all_connections(_client, port);
				peers; peers++)
			{
				result.append(
					( py::object(*l)+":"+py::object(*peers) )
						.attr("split")(":"));
			}
		}
		return result;
	}
	py::list controlConnections()
	{
		if (_debug) std::cerr << "controlConnections " <<  std::endl;
		// TODO; No control connections?
		return py::list();
	}
	py::list connectorPeers(
		const std::string &unit,
		const std::string &kind,
		const std::string &direction,
		const std::string &connectortName)
	{
		if (_debug) std::cerr << "connectorPeers" << std::endl;

		jack_port_t * port = findPort(unit, connectortName);
		py::list result;
		for (JackList peers = jack_port_get_all_connections(_client, port);
			peers; peers++)
		{
			result.append(py::object(*peers).attr("split")(":"));
		}
		return result;
	}

private:
	std::string full(
		const std::string & unit,
		const std::string & connector)
	{
		return unit + ":" + connector;
	}

	jack_port_t * findPort(
		const std::string & unit,
		const std::string & connector)
	{
		return jack_port_by_name(_client,
			full(unit,connector).c_str());
	}
};


std::string getDescription(JackEngine & client)
{
	return "";
}



BOOST_PYTHON_MODULE(Jack_Engine)
{
	// Keep 'using namespace' in the inner scope
	using namespace boost::python;

	class_<JackEngine>("Jack_Engine")
		.def("addProcessing",
			&JackEngine::addProcessing,
			"Adds a Processing with the name and type specified to the network"
			)
		.def("renameProcessing",
			&JackEngine::renameProcessing,
			"Renames a processing. Returns true if successfull."
			)
		.def("deleteProcessing",
			&JackEngine::deleteProcessing,
			"Removes a processing from the network."
			)
		.def("processingType",
			&JackEngine::processingType,
			"Returns the type of the processing"
			)
		.def("processingNames",
			&JackEngine::processingNames,
			"Returns the names of all the processing modules"
			)
		.def("hasProcessing",
			&JackEngine::hasProcessing,
			"Tells whether or not there is a processing with the given name on the network"
			)
		.def("processingConfig",
			&JackEngine::processingConfig,
//			return_value_policy<manage_new_object>(),
			"Returns the config dictionary of the processing"
			)
		.def("processingConnectors",
			&JackEngine::processingConnectors,
			"Returns a list of the connectors of certain kind and direction from a Processing"
			)
		.def("processingHasConnector",
			&JackEngine::processingHasConnector,
			"Returns true if the processing has the specified connector"
			)
		.def("connectorType",
			&JackEngine::connectorType,
			"Returns a list of the connectors of certain kind and direction from a Processing"
			)
		.def("processingConnectors",
			&JackEngine::processingConnectors,
			"Returns a list of the connectors of certain kind and direction from a Processing"
			)
		.def("connect",
			&JackEngine::connect,
			"Connects an outconnector with an inconnector"
			)
		.def("disconnect",
			&JackEngine::disconnect,
			"Connects an outconnector with an inconnector"
			)
		.def("areConnectable",
			&JackEngine::areConnectable,
			"Returns true if the two connectors are connectable."
			)
		.def("connectionExists",
			&JackEngine::connectionExists,
			"Returns true if the two connectors are connected."
			)
		.def("getDescription",
			getDescription,
			"Returns the description of the network"
			)
		.def("portConnections",
			&JackEngine::portConnections,
			""
			)
		.def("connectorPeers",
			&JackEngine::connectorPeers,
			""
			)
		.def("controlConnections",
			&JackEngine::controlConnections,
			""
			)
		.def("connectorIndex",
			&JackEngine::connectorIndex,
			""
			)

		// Extras

		.def_readwrite("debug",
			&JackEngine::_debug,
			"Enables or disables tracing API calls"
			)
		.def("availableTypes",
			&JackEngine::availableTypes,
			"Returns the names of the processing types than can be added to the network"
			)
/*
		.def("setDescription",
			setDescription,
			"Sets the description of the network"
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
*/
		;
}
