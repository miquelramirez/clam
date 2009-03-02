#include "SpatDIFSourceReceiver.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "SpatDIFSourceReceiver",
	"category", "[plugin] Open Sound Control",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::SpatDIFSourceReceiver> registrator(metadata);

void CLAM::SpatDIFSourceReceiver::error(int num, const char *msg, const char *path)
{
	printf("liblo server error %d in path %s: %s\n", num, path, msg);
	fflush(stdout);
}


/* catch any incoming messages and display them. returning 1 means that the
 * message has not been fully handled and the server should try other methods */
int CLAM::SpatDIFSourceReceiver::generic_handler(const char *path, const char *types, lo_arg **argv,
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

int CLAM::SpatDIFSourceReceiver::controls_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 void *data, void *user_data)
{
	CLAM::SpatDIFSourceReceiver & self = *((CLAM::SpatDIFSourceReceiver*)user_data);


	std::string cuttedPath=path;
	if (cuttedPath.substr(cuttedPath.size()-1,1)=="/")
		cuttedPath=cuttedPath.substr(0,cuttedPath.size()-1);
	size_t lastBar=cuttedPath.rfind("/");
	if (lastBar!=-1)
		cuttedPath=cuttedPath.substr(lastBar+1,1+cuttedPath.size()-lastBar);
//	std::cout<<cuttedPath<<std::endl;


	unsigned int baseOutControlNumber=0;
	if (cuttedPath=="ypr") baseOutControlNumber=3;
	if (SplitPath(path,3)=="sampler")
	{
//		sendControl(&self.GetOutControl(6,&self._voice.GetLastValue());
//		if (cuttedPath=="setVoice")
//			return 0;
		dynamic_cast <TypedOutControl<int>*> (&self.GetOutControl(6))->SendControl(atoi(SplitPath(path,4).c_str()));
		if (cuttedPath=="play") baseOutControlNumber=7;
		if (cuttedPath=="setLoop") baseOutControlNumber=8;
	}

	for (int i=0;i<argc;i++)
	{
		sendControl(&self.GetOutControl(baseOutControlNumber+i),argv[i]);
	}
	return 0;
}

int CLAM::SpatDIFSourceReceiver::quit_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 void *data, void *user_data)
{
	printf("quiting\n\n");
	fflush(stdout);
	return 0;
}

lo_server_thread CLAM::SpatDIFSourceReceiver::ServerStart(const char* port)
{
	if (IsPortUsed(port))
	{
		std::cout<<"SpatDIFSourceReceiver: WARNING: trying to start an existent server port!"<<std::endl;
		ServersInstances().erase(ServersInstances().find(port));
	}
	std::cout << "SpatDIFSourceReceiver: Starting the server on port" << port << std::endl;
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
	std::cout<<"SpatDIFSourceReceiver: number of servers: "<<ServersInstances().size()<<std::endl;
	lo_server_thread_start(serverThread);
	return serverThread;
}
bool CLAM::SpatDIFSourceReceiver::RemoveServer(const char* port)
{
	OscServersMap::iterator itServer=ServersInstances().find(port);
	if (itServer==ServersInstances().end())
	{
		std::cout<<"SpatDIFSourceReceiver: can't remove an inexistent server"<<std::endl;
		return false;
	}
	if (itServer->second.methods.size()>0)
	{
		std::cout<<"SpatDIFSourceReceiver: can't remove an used server"<<std::endl;
		return false;
	}
	ServersInstances().erase(itServer);
	return true;
}

bool CLAM::SpatDIFSourceReceiver::InsertInstance(const char* port, const char * path, const char * typespec)
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
				//std::cout<<"SpatDIFSourceReceiver: Creating new method handler "<<port<<" - "<< path<<" - "<< typespec<<std::endl;
				return false;
			}
		}
	}
	else
		std::cout<<"SpatDIFSourceReceiver: InsertInstance: path "<< path<< " doesn't exists. Creating"<<std::endl;
	methods.insert(MethodsMultiMap::value_type(path,typespec));
	return true;
}

bool CLAM::SpatDIFSourceReceiver::EraseInstance(const char* port, const char * path, const char * typespec)
{
	OscServersMap::iterator itServer=ServersInstances().find(port);
	if (itServer==ServersInstances().end())
	{
		std::cout<<"SpatDIFSourceReceiver: EraseInstance: server doesn't exist!!" <<std::endl;
		return false;
	}
	MethodsMultiMap & methods=itServer->second.methods;
	MethodsMultiMap::iterator itMethods=methods.find(path);
	if (itMethods==methods.end())	// if path doesn't exists
	{
		std::cout<<"SpatDIFSourceReceiver: Method handler (path, typespec) doesn't exists, not removed!"<<std::endl;
		return false;
	}
	// iterate on existings methods matching path
	for (itMethods=methods.find(path); itMethods!=methods.upper_bound(path);itMethods++)
	{
		if (itMethods->second == typespec)
		{
			//std::cout<<"SpatDIFSourceReceiver: Erasing method handler "<<port<<" - "<< path<<" - "<< typespec<<std::endl;
			methods.erase(itMethods);
			return true;
		}
	}
	std::cout<<"SpatDIFSourceReceiver: Method handler (path, typespec) doesn't exists, not removed!"<<std::endl;
	return false;
}
const unsigned int CLAM::SpatDIFSourceReceiver::GetInstances (const char* port)
{
	OscServersMap::const_iterator itServer=ServersInstances().find(port);
	if (itServer==ServersInstances().end())
	{
		std::cout<<"SpatDIFSourceReceiver: GetInstances: server doesn't exist!! returning 0";
		return 0;
	}
	return itServer->second.methods.size();
}


