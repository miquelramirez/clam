#ifndef MultiLibloSourceTyped_hxx
#define MultiLibloSourceTyped_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/TypedOutControl.hxx>
#include <CLAM/ControlArray.hxx>
#include <CLAM/Enum.hxx>
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

class EnumOSCTypes : public CLAM::Enum {
public:
        EnumOSCTypes() : CLAM::Enum(ValueTable(), eFloat) {}
        EnumOSCTypes(tValue v) : CLAM::Enum(ValueTable(), v) {};
        EnumOSCTypes(const std::string & s) : CLAM::Enum(ValueTable(), s) {};

        virtual CLAM::Component * Species() const {return new EnumOSCTypes();}

        typedef enum {
                eString=0,
                eFloat=1,
                eDouble=2,
		eInt=3,
		eInt64=4
        } tEnum;
        static tEnumValue * ValueTable()
        {
                static tEnumValue sValueTable[] =
                {
			{eString,"string"},
			{eFloat,"float"},
			{eDouble,"double"},
			{eInt,"integer"},
			{eInt64,"integer64"},
                        {0,NULL}
                };
                return sValueTable;
        }
};


class MultiLibloSourceTypedConfig : public CLAM::ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE( MultiLibloSourceTypedConfig, 4, ProcessingConfig );
	DYN_ATTRIBUTE( 0, public, std::string, OscPath);
	DYN_ATTRIBUTE( 1, public, std::string, ServerPort);
	DYN_ATTRIBUTE( 2, public, CLAM::TData, NumberOfArguments);
	DYN_ATTRIBUTE( 3, public, EnumOSCTypes, OSCTypesMask);
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

class MultiLibloSourceTyped : public CLAM::Processing
{
	typedef ControlArray<TypedOutControl <std::string> > OutTypedControlArray;
	typedef MultiLibloSourceTypedConfig Config;

public:
	MultiLibloSourceTyped(const Config& config = Config()) 
		: _serverThread(0)
		, _typespec("fff")
		, _oscPath(config.GetOscPath())
		, _port("")
		, _methodSetted(false)
		
	{
		Configure( config );
	}
	
	~MultiLibloSourceTyped()
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
		return "MultiLibloSourceTyped";
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
			std::cout << "MultiLibloSourceTyped::ConcreteConfigure server NOT started -- default config" << std::endl;
			return true;
		}
		if (_methodSetted) // if exists delete previous method (with previous _port, _oscPath and _typespec)
		{ 
			DeleteMethod();
		}
		// define processing callback catcher (strings)
		std::string typespecMask="";
		std::string typeChar="";

		unsigned int type=EnumOSCTypes::eFloat;

		if (_config.HasOSCTypesMask())
			type=EnumOSCTypes(_config.GetOSCTypesMask());
		switch (type)
		{
			case EnumOSCTypes::eString:
				typeChar="s";
				break;
			case EnumOSCTypes::eFloat:
				typeChar="f";
				break;
			case EnumOSCTypes::eDouble:
				typeChar="d";
				break;
			case EnumOSCTypes::eInt:
				typeChar="i";
				break;
			case EnumOSCTypes::eInt64:
				typeChar="h";
				break;
			default:
				typeChar="f";
		}
std::cout<<EnumOSCTypes(type);
		for (int i=0;i<nOutputs;i++)
		{
			typespecMask+=typeChar; //if type is not defined, add "f"s as arguments
		}
		_typespec=typespecMask;
		_oscPath=_config.GetOscPath();
		_port=port;

		if (IsPortUsed(port.c_str())) // if exist server on port
		{
			_serverThread = (*ServersInstances().find(_port.c_str())).second.thread;	//uses existing thread
		}
		// if there are no servers on port. IMPORTANT: the conditional need to be explicit, since the server could be removed by the last conditional
		if (not IsPortUsed(port.c_str())) 
		{ 
			_serverThread = ServerStart(port.c_str()); // start new server
		}
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
			_methodSetted=false;
		}
		if (IsPortUsed(_port.c_str()) && GetInstances(_port.c_str())==0) // if was the last instance
		{
			std::cout <<"MultiLibloSourceTyped: Shutting down the server..."<<std::endl;
			lo_server_thread_free(_serverThread);
			RemoveServer(_port.c_str());
			_serverThread=0;
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
	OutTypedControlArray _outControls;
};

} //namespace

#endif
