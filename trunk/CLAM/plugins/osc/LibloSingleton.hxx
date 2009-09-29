#ifndef LibloSingleton_hxx
#define LibloSingleton_hxx

//#include <string>
#include <cstdio>
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
		//TODO: save the processing "this" address too in the sctructure!! (and use on unregistration)
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
	const std::string GetErrorMessage()
	{
		const std::string copy=_errorMessage;
		_errorMessage="";
		return copy;
	}

private:

	void SetAndPrintErrorMessage(const std::string & message)
	{
		_errorMessage=message;
		std::cout<<"LibloSingleton: "<<message<<std::endl;
	}


	std::list<OSCInstance> _OSCInstances;
	std::string _errorMessage;

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

	const lo_server_thread StartServer(const unsigned int & port,const std::string multicastIP="")	
	{
		if (IsPortUsed(port))
		{
			SetAndPrintErrorMessage("server not started (previous instance exists)");
			return NULL;
		}

		char portCString[10];
		sprintf(portCString,"%u",port);

		lo_server_thread serverThread;
		if (multicastIP!="")
		{
// TODO: defined by SConstruct if liblo version > 0.26 (include multicast support)
#ifndef __MULTICAST_LIBLO__
			SetAndPrintErrorMessage("not compiled with multicast support (requires liblo>0.26)");
			return NULL;
#endif
#ifdef __MULTICAST_LIBLO__ //to avoid compiler error...
			serverThread = lo_server_thread_new_multicast(multicastIP.c_str(),portCString, error);
#endif
			std::cout << "LibloSingleton: Starting the multicast server on port " << portCString << " (IP: "<<multicastIP<<")"<< std::endl;
		}
		else
		{
			serverThread = lo_server_thread_new(portCString, error);
			std::cout << "LibloSingleton: Starting the server on port " << portCString << std::endl;
		}
		if (not serverThread)
		{
			SetAndPrintErrorMessage("Error creating OSC server instance");
			return NULL;
		}
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
		return RegisterOscCallback(port,path,typespec,callbackMethod,instanceData,"");
	}

	const bool RegisterOscCallback(const unsigned int & port, const std::string & path,
				 const std::string & typespec, lo_method_handler callbackMethod,void * instanceData, const std::string multicastIP)
	{
		bool newServer=false;
		lo_server_thread thread=GetServerThread(port);
		if (thread==NULL)
		{
			std::cout<<"LibloSingleton: starting server thread for port "<<port<<std::endl;
			std::cout<<" (path: "<<path<<")"<<std::endl;
			thread=StartServer(port,multicastIP);
			if (thread==NULL)
			{
				SetAndPrintErrorMessage("Error starting the server");
				return false;
			}
			newServer=true;
		}

		if (not newServer and IsPathRegistered(port,path,typespec))
		{
			SetAndPrintErrorMessage("Cannot create a method instance of an already registered path");
			return false;
		}

		lo_server_thread_add_method(thread, path.c_str(), typespec.c_str(), callbackMethod, instanceData);

		//TODO: register the processing "this" address too!! (and use on Unregistration)
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
