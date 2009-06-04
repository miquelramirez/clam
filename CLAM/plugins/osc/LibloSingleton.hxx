#ifndef LibloSingleton_hxx
#define LibloSingleton_hxx


#include <string>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>

#include <iostream>

#include "lo/lo.h"

namespace CLAM
{

class LibloSingleton
{
public:
	struct PathHandler
	{
		std::string oscPath;
		std::string typespec;
	};
private:
	std::vector<PathHandler> _configuredPaths;

	LibloSingleton() {}
	LibloSingleton(const LibloSingleton &) {}

public:
	static LibloSingleton& GetInstance();

	const std::vector<PathHandler> & GetConfiguredPaths()
	{
std::cout<<"=-=-=-=-=-=-=-"<< _configuredPaths.size()<<std::endl;
		return _configuredPaths;
	}

	void UnregisterOscCallback(std::string port, std::string oscPath, std::string typespec)
	{
		std::vector<PathHandler>::iterator itConfiguredPaths;
		for (itConfiguredPaths=_configuredPaths.begin();itConfiguredPaths!=_configuredPaths.end();itConfiguredPaths++)
		{
			if ((*itConfiguredPaths).oscPath==oscPath and (*itConfiguredPaths).typespec==typespec)
				break;
		}
		// if don't exist a previous method handler
		if (itConfiguredPaths == _configuredPaths.end())
			return;

		lo_server_thread serverThread=GetServerThread(port);

		if (EraseInstance(port.c_str(), oscPath.c_str(),typespec.c_str()))
			lo_server_thread_del_method(serverThread,oscPath.c_str(),typespec.c_str()); //delete it
		_configuredPaths.erase(itConfiguredPaths); //remove from the list
std::cout<<"\t\tactual number of configured paths: "<<_configuredPaths.size()<<std::endl;
		if (IsPortUsed(port.c_str()) && GetInstances(port.c_str())==0) // if was the last instance
		{
			std::cout <<"Liblo: Shutting down the server..."<<std::endl;
			lo_server_thread_free(serverThread);
			RemoveServer(port.c_str());
			serverThread=0;
		}
	}

	bool RegisterOscCallback(std::string port, std::string oscPath, std::string typespec, int (*callBackMethod)(const char *, const char *, lo_arg **, int, void *, void *))
	{
std::cout<<"trying to register callback for path \""<<oscPath<<"\", port: "<<port<<" - typespec: \""<<typespec<<"\""<<std::endl;
		if (InsertInstance(port.c_str(), oscPath.c_str(), typespec.c_str()))
		{
std::cout<<"\tsi"<<std::endl;
			lo_server_thread serverThread=GetServerThread(port);
			lo_server_thread_add_method(serverThread, oscPath.c_str(), typespec.c_str(), callBackMethod, this);
			PathHandler configuredPath;
			configuredPath.oscPath=oscPath;
			configuredPath.typespec=typespec;
			_configuredPaths.push_back(configuredPath);
std::cout<<"\t\tactual number of configured paths: "<<_configuredPaths.size()<<std::endl;
//std::cout<<"\t\t\tfrom GetConfiguredPaths: "<<GetConfiguredPaths().size()<<std::endl;
			return true;
		}
std::cout<<"\tno"<<std::endl;
		return false;
	}
	static bool IsPortUsed(const char* port)
	{
		return ( ServersInstances().find(port) != ServersInstances().end()); 
	}

	const lo_server_thread GetServerThread(const std::string & port)
	{
		lo_server_thread serverThread;

		if (IsPortUsed(port.c_str())) // if exist server on port
			serverThread = (*ServersInstances().find(port.c_str())).second.thread;	//uses existing thread
		else
			serverThread = ServerStart(port.c_str()); // start new server

		return serverThread;
	}

private:
	static void error(int num, const char *m, const char *path);

	static int generic_handler(const char *path, const char *types, lo_arg **argv,
			 int argc, void *data, void *user_data);

//	static int controls_handler(const char *path, const char *types, lo_arg **argv, int argc,
//			 void *data, void *user_data);

	static int quit_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);

	static bool EraseInstance(const char* port, const char * path, const char * typespec);
	static bool InsertInstance(const char* port, const char * path, const char * typespec);

	// server management related structs, methods, and attributes
	// key: path, value: typespec
	typedef std::multimap<std::string,std::string> MethodsMultiMap;
	struct ServerInstance
	{
		lo_server_thread thread;
		MethodsMultiMap methods;
	};

	// map key: port
	typedef std::map<std::string,ServerInstance> OscServersMap;
	static OscServersMap & ServersInstances()
	{
		static OscServersMap sOscServersMap;
		return sOscServersMap;
	}
	static lo_server_thread ServerStart(const char* port);
	static bool RemoveServer(const char* port);
	static bool IncInstance(const char* port);
	static bool DecInstance(const char* port);
	static const unsigned int GetInstances (const char* port);


};

}
#endif
