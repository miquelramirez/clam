#ifndef LibloSingleton_hxx
#define LibloSingleton_hxx

#include <string>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>
#include <list>

#include <iostream>

#include "lo/lo.h"

namespace CLAM
{

class LibloSingleton
{
public:
	struct OSCInstance
	{
		OSCInstance() {}
		OSCInstance(unsigned int initPort,lo_server_thread initThread, std::string initPath, std::string initTypespec)
		: port(initPort)
		, thread(initThread)
		, path(initPath)
		, typespec(initTypespec)
		{}
		unsigned int port;
		lo_server_thread thread;
		std::string path;
		std::string typespec;
	};

private:
	std::list<OSCInstance> _OSCInstances;

	LibloSingleton() {}
	LibloSingleton(const LibloSingleton &) {}

	const lo_server_thread GetServerThread(const unsigned int & port) const
	{
		std::list<OSCInstance>::const_iterator it;
		for (it=_OSCInstances.begin();it!=_OSCInstances.end();it++)
		{
			if ((*it).port==port)
			{
				return (*it).thread;
			}
		}
		return NULL;
	}

	const bool IsPortUsed(const unsigned int & port) const
	{
		return (GetServerThread(port)!=NULL);
	}

	const lo_server_thread StartServer(const unsigned int & port)
	{
		if (IsPortUsed(port))
		{
			std::cout<< "LibloSigleton: server not started (previous instance exists)"<<std::endl;
			return NULL;
		}

		char portCString[10];
		sprintf(portCString,"%u",port);

		std::cout << "LibloSingleton: Starting the server on port " << portCString << std::endl;

		lo_server_thread serverThread = lo_server_thread_new(portCString, error);
		if (not serverThread) return NULL;
		/* add method that will match any path and args */
		lo_server_thread_add_method(serverThread, NULL, NULL, generic_handler, NULL);
		/* add method that will match the path /quit with no args */
		lo_server_thread_add_method(serverThread, "/quit", "", quit_handler, NULL);
		lo_server_thread_start(serverThread);
		return serverThread;
	}
	
public:
	static LibloSingleton& GetInstance();
	const bool RegisterOscCallback(const unsigned int & port, const std::string & path,
				 const std::string & typespec, lo_method_handler callbackMethod,void * instanceData)
	{
		bool newServer=false;
		lo_server_thread thread=GetServerThread(port);
		if (thread==NULL)
		{
			std::cout<<"LibloSingleton: starting server thread for port "<<port<<std::endl;
			std::cout<<" (path: "<<path<<")"<<std::endl;
			thread=StartServer(port);
			if (thread==NULL)
			{
				std::cout<<"LibloSingleton: Error starting the server on port "<<port<<std::endl;
				return false;
			}
			newServer=true;
		}

		if (not newServer and IsPathRegistered(port,path,typespec))
		{
			std::cout<<"LibloSingleton: Cannot create a method instance of an already registered path"<<std::endl;
			return false;
		}

		lo_server_thread_add_method(thread, path.c_str(), typespec.c_str(), callbackMethod, instanceData);
		_OSCInstances.push_back(OSCInstance(port,thread,path,typespec));
		return true;
	}

	const bool UnregisterOscCallback(const unsigned int & port, const std::string & path, const std::string & typespec)
	{
		if (not IsPathRegistered(port,path,typespec))
		{
			std::cout<<"LibloSingleton: cannot unregister an unexistent path " << path << " on port "<<port <<"!"<<std::endl;
			return false;
		}

		const lo_server_thread & thread=GetServerThread(port);
		RemoveRegisteredPath(port,path,typespec);

		if (not IsPortUsed(port)) // if was the last instance
		{
			std::cout <<"LibloSingleton: Shutting down the server..."<<std::endl;
			lo_server_thread_free(thread);
		}
		return true;
	}

	const bool IsPathRegistered(const unsigned int & port, const std::string & path, const std::string & typespec) const;
private:
	bool RemoveRegisteredPath(const unsigned int & port, const std::string & path, const std::string & typespec);

	static void error(int num, const char *m, const char *path);

	static int generic_handler(const char *path, const char *types, lo_arg **argv,
			 int argc, void *data, void *user_data);

	static int quit_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);

};

}
#endif
