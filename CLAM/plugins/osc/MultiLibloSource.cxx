#include "MultiLibloSource.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "MultiLibloSource",
	"category", "[plugin] Open Sound Control",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::MultiLibloSource> registrator(metadata);

void CLAM::MultiLibloSource::error(int num, const char *msg, const char *path)
{
	printf("liblo server error %d in path %s: %s\n", num, path, msg);
	fflush(stdout);
}


/* catch any incoming messages and display them. returning 1 means that the
 * message has not been fully handled and the server should try other methods */
int CLAM::MultiLibloSource::generic_handler(const char *path, const char *types, lo_arg **argv,
		    int argc, void *data, void *user_data)
{
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
	return 1;
}

int CLAM::MultiLibloSource::controls_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 void *data, void *user_data)
{
	CLAM::MultiLibloSource & self = *((CLAM::MultiLibloSource*)user_data);
	/* example showing pulling the argument values out of the argv array */
	printf("%s <- f:",path);
	for (int i=0;i<argc;i++)
	{
		if(i>0) 
			printf(", ");
		printf ("%f", argv[i]->f);
		self._outControls[i].SendControl( argv[i]->f );
	}
	printf("\n\n");
	fflush(stdout);
	return 0;
}

int CLAM::MultiLibloSource::quit_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 void *data, void *user_data)
{
	printf("quiting\n\n");
	fflush(stdout);
	return 0;
}

lo_server_thread CLAM::MultiLibloSource::ServerStart(const char* port, void * parent)
{
	if (IsPortUsed(port))
	{
		std::cout<<"WARNING: trying to start an existent server port!"<<std::endl;
		ServersInstances().erase(ServersInstances().find(port));
	}
	std::cout << "Starting the server" << std::endl;		
	std::cout << "MultiLibloSource::ConcreteConfigure: STARTING the server. port " << port << std::endl;
	lo_server_thread serverThread = lo_server_thread_new(port, error);
	ServerInstance server;
	server.thread=serverThread;
	MethodsMultiMap methods; // create an empty map
	server.methods=methods;
	ServersInstances().insert(OscServersMap::value_type(port,server));
	// common methods are not counted in map!:
	/* add method that will match any path and args */
	lo_server_thread_add_method(serverThread, NULL, NULL, generic_handler, parent);
	/* add method that will match the path /quit with no args */
	lo_server_thread_add_method(serverThread, "/quit", "", quit_handler, parent);
	std::cout<<"number of servers: "<<ServersInstances().size()<<std::endl;
	lo_server_thread_start(serverThread);
	return serverThread;
}
bool CLAM::MultiLibloSource::RemoveServer(const char* port)
{
	OscServersMap::iterator itServer=ServersInstances().find(port);
	if (itServer==ServersInstances().end())
	{
		std::cout<<"MultiLibloSource: can't remove an inexistent server"<<std::endl;
		return false;
	}
	if (itServer->second.methods.size()>0)
	{
		std::cout<<"MultiLibloSource: can't remove an used server"<<std::endl;
		return false;
	}
	ServersInstances().erase(itServer);
	return true;
}

bool CLAM::MultiLibloSource::InsertInstance(const char* port, const char * path, const char * typespec)
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
				std::cout<<"Method handler (path, typespec) exists, not added!"<<std::endl;
				return false;
			}
		}
	}
//	std::cout<<"added instance: "<<path<<" - "<<typespec<<std::endl;
	methods.insert(MethodsMultiMap::value_type(path,typespec));
	return true;
}

bool CLAM::MultiLibloSource::EraseInstance(const char* port, const char * path, const char * typespec)
{
	OscServersMap::iterator itServer=ServersInstances().find(port);
	if (itServer==ServersInstances().end())
		return false;
	MethodsMultiMap & methods=itServer->second.methods;
	MethodsMultiMap::iterator itMethods=methods.find(path);
	if (itMethods==methods.end())	// if path doesn't exists
	{
		std::cout<<"Method handler (path, typespec) doesn't exists, not removed!"<<std::endl;
		return false;
	}
	// iterate on existings methods matching path
	for (itMethods=methods.find(path); itMethods!=methods.upper_bound(path);itMethods++)
	{
		if (itMethods->second == typespec)
		{
			methods.erase(itMethods);
//			std::cout<<"removed instance: "<<path<<" - "<<typespec<<std::endl;
			return true;
		}
	}
	std::cout<<"Method handler (path, typespec) doesn't exists, not removed!"<<std::endl;
	return false;
}
const unsigned int CLAM::MultiLibloSource::GetInstances (const char* port)
{
	OscServersMap::const_iterator itServer=ServersInstances().find(port);
	if (itServer==ServersInstances().end())
		return 0;
	return itServer->second.methods.size();
}
