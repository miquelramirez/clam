#include "MultiLibloSourceTyped.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "MultiLibloSourceTyped",
	"category", "[plugin] Open Sound Control",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::MultiLibloSourceTyped> registrator(metadata);

void CLAM::MultiLibloSourceTyped::error(int num, const char *msg, const char *path)
{
	printf("liblo server error %d in path %s: %s\n", num, path, msg);
	fflush(stdout);
}


/* catch any incoming messages and display them. returning 1 means that the
 * message has not been fully handled and the server should try other methods */
int CLAM::MultiLibloSourceTyped::generic_handler(const char *path, const char *types, lo_arg **argv,
		    int argc, void *data, void *user_data)
{
#if 0
	int i;
	printf("path: <%s>\n", path);
	for (i=0; i<argc; i++) 
	{
		printf("arg %d '%c' ", i, types[i]);
		lo_arg_pp((lo_type)types[i], argv[i]);
		printf("\n");
	}
	printf("\n");
	fflush(stdout);
#endif
	return 1;
}

int CLAM::MultiLibloSourceTyped::controls_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 void *data, void *user_data)
{
	CLAM::MultiLibloSourceTyped & self = *((CLAM::MultiLibloSourceTyped*)user_data);
	/* example showing pulling the argument values out of the argv array */
//	printf("%s <- f:",path);
	for (int i=0;i<argc;i++)
	{
		sendControl(self._outControls[i],argv[i]);
		
	}
//	printf("\n\n");
//	fflush(stdout);
	return 0;
}

int CLAM::MultiLibloSourceTyped::quit_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 void *data, void *user_data)
{
	printf("quiting\n\n");
	fflush(stdout);
	return 0;
}

lo_server_thread CLAM::MultiLibloSourceTyped::ServerStart(const char* port)
{
	if (IsPortUsed(port))
	{
		std::cout<<"MultiLibloSourceTyped: WARNING: trying to start an existent server port!"<<std::endl;
		ServersInstances().erase(ServersInstances().find(port));
	}
	std::cout << "MultiLibloSourceTyped: Starting the server on port" << port << std::endl;
	lo_server_thread serverThread = lo_server_thread_new(port, error);
	if (not serverThread) return 0;
	ServerInstance server;
	server.thread=serverThread;
	MethodsMultiMap methods; // create an empty map
	server.methods=methods;
	ServersInstances().insert(OscServersMap::value_type(port,server));
	// common methods are not counted in map!:
	/* add method that will match any path and args */
	lo_server_thread_add_method(serverThread, NULL, NULL, generic_handler, NULL);
	/* add method that will match the path /quit with no args */
	lo_server_thread_add_method(serverThread, "/quit", "", quit_handler, NULL);
	std::cout<<"MultiLibloSourceTyped: number of servers: "<<ServersInstances().size()<<std::endl;
	lo_server_thread_start(serverThread);
	return serverThread;
}
bool CLAM::MultiLibloSourceTyped::RemoveServer(const char* port)
{
	OscServersMap::iterator itServer=ServersInstances().find(port);
	if (itServer==ServersInstances().end())
	{
		std::cout<<"MultiLibloSourceTyped: can't remove an inexistent server"<<std::endl;
		return false;
	}
	if (itServer->second.methods.size()>0)
	{
		std::cout<<"MultiLibloSourceTyped: can't remove an used server"<<std::endl;
		return false;
	}
	ServersInstances().erase(itServer);
	return true;
}

bool CLAM::MultiLibloSourceTyped::InsertInstance(const char* port, const char * path, const char * typespec)
{
	OscServersMap::iterator itServer=ServersInstances().find(port);
	if (itServer==ServersInstances().end())
		return false;
	MethodsMultiMap & methods=itServer->second.methods;
	MethodsMultiMap::const_iterator itMethods=methods.find(path);
	if (itMethods!=methods.end())	// if path exists
	{
		// iterate on existings methods mathing path
		for (itMethods=methods.find(path); itMethods != methods.upper_bound(path);itMethods++)
		{
			if (itMethods->second == typespec)
			{
				//std::cout<<"MultiLibloSourceTyped: Creating new method handler "<<port<<" - "<< path<<" - "<< typespec<<std::endl;
				return false;
			}
		}
	}
	else
		std::cout<<"MultiLibloSourceTyped: InsertInstance: path "<< path<< " doesn't exists. Creating"<<std::endl;
	methods.insert(MethodsMultiMap::value_type(path,typespec));
	return true;
}

bool CLAM::MultiLibloSourceTyped::EraseInstance(const char* port, const char * path, const char * typespec)
{
	OscServersMap::iterator itServer=ServersInstances().find(port);
	if (itServer==ServersInstances().end())
	{
		std::cout<<"MultiLibloSourceTyped: EraseInstance: server doesn't exist!!" <<std::endl;
		return false;
	}
	MethodsMultiMap & methods=itServer->second.methods;
	MethodsMultiMap::iterator itMethods=methods.find(path);
	if (itMethods==methods.end())	// if path doesn't exists
	{
		std::cout<<"MultiLibloSourceTyped: Method handler (path, typespec) doesn't exists, not removed!"<<std::endl;
		return false;
	}
	// iterate on existings methods matching path
	for (itMethods=methods.find(path); itMethods!=methods.upper_bound(path);itMethods++)
	{
		if (itMethods->second == typespec)
		{
			//std::cout<<"MultiLibloSourceTyped: Erasing method handler "<<port<<" - "<< path<<" - "<< typespec<<std::endl;
			methods.erase(itMethods);
			return true;
		}
	}
	std::cout<<"MultiLibloSourceTyped: Method handler (path, typespec) doesn't exists, not removed!"<<std::endl;
	return false;
}
const unsigned int CLAM::MultiLibloSourceTyped::GetInstances (const char* port)
{
	OscServersMap::const_iterator itServer=ServersInstances().find(port);
	if (itServer==ServersInstances().end())
	{
		std::cout<<"MultiLibloSourceTyped: GetInstances: server doesn't exist!! returning 0";
		return 0;
	}
	return itServer->second.methods.size();
}
