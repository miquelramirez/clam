#ifndef MultiLibloSource_hxx
#define MultiLibloSource_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControlArray.hxx>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>

#include "lo/lo.h"
#ifdef GetClassName // windows.h introduces that
#undef GetClassName
#endif

namespace CLAM
{

class MultiLibloSourceConfig : public CLAM::ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE( MultiLibloSourceConfig, 3, ProcessingConfig );
	DYN_ATTRIBUTE( 0, public, std::string, OscPath);
	DYN_ATTRIBUTE( 1, public, std::string, ServerPort);
	DYN_ATTRIBUTE( 2, public, CLAM::TData, NumberOfArguments);
	//TODO number of arguments/ports
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
		SetNumberOfArguments(3);
		SetOscPath("/clam/target");
		SetServerPort("");
	};
};

class MultiLibloSource : public CLAM::Processing
{
	typedef MultiLibloSourceConfig Config;

public:
	MultiLibloSource(const Config& config = Config()) 
		: _serverThread(0)
		, _typespec("fff")
		, _oscPath(config.GetOscPath())
		, _port("")
		, _methodSetted(false)
		
	{
		Configure( config );
	}
	
	~MultiLibloSource()
	{
		DeleteMethod();
		RemoveOldControls();
	}

	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}
	bool Do()
	{
		return true;
	}
	const char* GetClassName() const
	{
		return "MultiLibloSource";
	}
protected:

        void RemoveOldControls()
        {
		_outControls.Clear();
		GetOutControls().Clear();
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		RemoveOldControls();
		CopyAsConcreteConfig(_config, config);
//		_config.AddAll();
//		_config.UpdateData();
//set outputs:
		int nOutputs = int(_config.GetNumberOfArguments());
		if (nOutputs < 1)
		{
			_config.SetNumberOfArguments(1);
			nOutputs = 1;
		}
		// multi-port names share user-configured identifier
		std::string oscPath=_config.GetOscPath();
		std::replace(oscPath.begin(),oscPath.end(),'/','_');
		_outControls.Resize(nOutputs,oscPath, this);

		std::string port = _config.GetServerPort();
		/* not defined port */
		if (port=="")
		{
			std::cout << "MultiLibloSource::ConcreteConfigure server NOT started -- default config" << std::endl;
			return true;
		}
		if (IsPortUsed(port.c_str())) // if exists server on port
		{ 
			_serverThread = (*ServersInstances().find(port)).second.thread;	//uses existing thread
			DeleteMethod();
		}
		// if there are no servers on port. IMPORTANT: the conditional need to be explicit, since the server could be removed by the last conditional
		if (not IsPortUsed(port.c_str())) 
		{ 
			_serverThread = ServerStart(port.c_str()); // start new server
		}

		// define processing callback catcher (floats, for now)
		std::string typespecMask="";
		for (int i=0;i<nOutputs;i++)
			typespecMask+="f"; //add "f"s as arguments
		_typespec=typespecMask;
		_oscPath=_config.GetOscPath();
		_port=port;
		if (AddMethod()==false)
			return false;
		_methodSetted=true;
		// add instance on map
		return true; // Configuration ok
	}

	void DeleteMethod()
	{
		// if exists a previous method handler
		if (_methodSetted)
		{
			if (EraseInstance(_port.c_str(), _oscPath.c_str(),_typespec.c_str()))
				lo_server_thread_del_method(_serverThread,_oscPath.c_str(),_typespec.c_str()); //delete it
		}
		if (IsPortUsed(_port.c_str()) && GetInstances(_port.c_str())==0) // if was the last instance
		{
			std::cout <<"MultiLibloSource: Shutting down the server..."<<std::endl;
			lo_server_thread_free(_serverThread);
			RemoveServer(_port.c_str());
		}
	}
	bool AddMethod()
	{
		if (InsertInstance(_port.c_str(), _oscPath.c_str(), _typespec.c_str()))
		{
			lo_server_thread_add_method(_serverThread, _oscPath.c_str(), _typespec.c_str(), controls_handler, this);
			return true;
		}
		return false;
	}

private:
	static void error(int num, const char *m, const char *path);

	static int generic_handler(const char *path, const char *types, lo_arg **argv,
			 int argc, void *data, void *user_data);

	static int controls_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);

	static int quit_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);

	static bool EraseInstance(const char* port, const char * path, const char * typespec);
	static bool InsertInstance(const char* port, const char * path, const char * typespec);

	// server management related structs, methods, and attributes
	// key: path, value: typespec
	typedef std::multimap<std::string,std::string> MethodsMultiMap;
	typedef struct
	{
		lo_server_thread thread;
		//unsigned int nProcessings;
		MethodsMultiMap methods;
	} ServerInstance;

	// map key: port
	typedef std::map<std::string,ServerInstance> OscServersMap;
	static OscServersMap & ServersInstances()
	{
		static OscServersMap sOscServersMap;
		return sOscServersMap;
	}
	static bool IsPortUsed(const char* port)
	{
		return ( ServersInstances().find(port) != ServersInstances().end() );
	}
	static lo_server_thread ServerStart(const char* port);
	static bool RemoveServer(const char* port);
	static bool IncInstance(const char* port);
	static bool DecInstance(const char* port);
	static const unsigned int GetInstances (const char* port);
	lo_server_thread _serverThread;
	std::string _typespec;
	std::string _oscPath;
	std::string _port;
	bool _methodSetted;
	Config _config;
	OutControlArray _outControls;
};

} //namespace

#endif
