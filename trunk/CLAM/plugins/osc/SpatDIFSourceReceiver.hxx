#ifndef SpatDIFSourceReceiver_hxx
#define SpatDIFSourceReceiver_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/TypedOutControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/ControlArray.hxx>
#include <CLAM/Enum.hxx>
#include <CLAM/Text.hxx>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>

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
			{eString,"s"},
			{eFloat,"f"},
			{eDouble,"d"},
			{eInt,"i"},
			{eInt64,"h"},
                        {0,NULL}
                };
                return sValueTable;
        }
};


class SpatDIFSourceReceiverConfig : public CLAM::ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE( SpatDIFSourceReceiverConfig, 3, ProcessingConfig );
	DYN_ATTRIBUTE( 0, public, std::string, SourceName);
	DYN_ATTRIBUTE( 1, public, unsigned int, VoicesNumber);
	DYN_ATTRIBUTE( 2, public, std::string, ServerPort);
	//TODO number of arguments/ports
	
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
		SetSourceName("");
		SetVoicesNumber(4);
		SetServerPort("");
	};
};

class SpatDIFSourceReceiver : public CLAM::Processing
{
	typedef SpatDIFSourceReceiverConfig Config;

public:
	SpatDIFSourceReceiver(const Config& config = Config()) 
		: _serverThread(0)
		, _baseOSCPath("")
		, _port("")
		, _x("X position",this)
		, _y("Y position",this)
		, _z("Z position",this)
		, _azimuth("azimuth",this)
		, _elevation("elevation",this)
		, _roll("roll",this)
		, _voice("voice number", this)
		, _play("voice play toggle",this)
		, _loop("voice loop toggle",this)
	{
		Configure( config );
	}
	
	~SpatDIFSourceReceiver()
	{
		UnregisterOscCallbacks();
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
		return "SpatDIFSourceReceiver";
	}

	const std::vector<std::string> GetSourcesNames() const;

protected:

	static std::string SplitPath(std::string input, unsigned tokenNumber)
	{
	unsigned i=0;
	char* inputcharp=(char*)input.c_str();
	inputcharp=strtok(inputcharp,"/");

	while (i!=tokenNumber and inputcharp!=NULL)
	{
		inputcharp=strtok(NULL,"/");
		i++;
	}
		if (inputcharp==NULL)
			return "";
		else
			return std::string(inputcharp);
	}


	static void sendControl(OutControlBase * control, lo_arg * valueToSend)
	{
		const std::string typeName=control->GetTypeId().name();
		if (typeName=="Ss")
			dynamic_cast<TypedOutControl<std::string> *> (control)->SendControl(&(valueToSend->s));
		if(typeName=="f")
			dynamic_cast<FloatOutControl *>(control)->SendControl(valueToSend->f);
		if(typeName=="d")
			dynamic_cast<TypedOutControl<double> *> (control)->SendControl(valueToSend->f32);
		if(typeName=="i")
			dynamic_cast<TypedOutControl<int> *> (control)->SendControl(valueToSend->i);
		return;
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		if (_config.GetSourceName()=="")
		{
			AddConfigErrorMessage("Source name not configured.");
			return false;
		}
		_baseOSCPath="/SpatDIF/source/"+_config.GetSourceName();


		if (_config.GetVoicesNumber()==0)
		{
			AddConfigErrorMessage("There is no voice");
			return false;
		}

		std::string port = _config.GetServerPort();
		/* not defined port */
		if (port=="")
		{
			AddConfigErrorMessage("No port defined");
			return false;
		}

		// if exists previous callback methods clear them
		if (_configuredPaths.size()!=0)
			UnregisterOscCallbacks();

		_port=port;

		if (IsPortUsed(port.c_str())) // if exist server on port
		{
			_serverThread = (*ServersInstances().find(_port.c_str())).second.thread;	//uses existing thread
		}
		else
		{ 
			_serverThread = ServerStart(port.c_str()); // start new server
		}
		if (RegisterOscCallbacks()==false)
		{
			AddConfigErrorMessage("Error creating OSC server instance");
			return false;
		}
		// add instance on map
		return true; // Configuration ok
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

		if (EraseInstance(port.c_str(), oscPath.c_str(),typespec.c_str()))
			lo_server_thread_del_method(_serverThread,oscPath.c_str(),typespec.c_str()); //delete it
		_configuredPaths.erase(itConfiguredPaths); //remove from the list
		if (IsPortUsed(port.c_str()) && GetInstances(port.c_str())==0) // if was the last instance
		{
			std::cout <<"SpatDIFSourceReceiver: Shutting down the server..."<<std::endl;
			lo_server_thread_free(_serverThread);
			RemoveServer(port.c_str());
			_serverThread=0;
		}
	}

	bool RegisterOscCallback(std::string port, std::string oscPath, std::string typespec)
	{
		if (InsertInstance(port.c_str(), oscPath.c_str(), typespec.c_str()))
		{
			lo_server_thread_add_method(_serverThread, oscPath.c_str(), typespec.c_str(), controls_handler, this);
			PathHandler configuredPath;
			configuredPath.oscPath=oscPath;
			configuredPath.typespec=typespec;
			_configuredPaths.push_back(configuredPath);
			return true;
		}
		return false;
	}


	bool RegisterOscCallbacks()
	{
		bool allOK=true;
		std::string oscPath;
		oscPath=_baseOSCPath+"/xyz";
		allOK &= RegisterOscCallback(_port,oscPath,"fff");
		oscPath=_baseOSCPath+"/ypr";
		allOK &= RegisterOscCallback(_port,oscPath,"fff");
		for (unsigned i=0;i<_config.GetVoicesNumber();i++)
		{
			std::ostringstream voice;
			voice<<i;
			oscPath=_baseOSCPath+"/sampler/"+voice.str()+"/play";

			allOK &= RegisterOscCallback(_port,oscPath,"ii"); // play with loop setting
			allOK &= RegisterOscCallback(_port,oscPath,"i"); //play without loop setting
			oscPath=_baseOSCPath+"/sampler/"+voice.str()+"/setLoop";
			allOK &= RegisterOscCallback(_port,oscPath,"i");
		}
		return allOK;
	}

	void UnregisterOscCallbacks()
	{
		while (_configuredPaths.size()!=0)
		{
			PathHandler firstPath=_configuredPaths.front();
			UnregisterOscCallback(_port, firstPath.oscPath, firstPath.typespec);
		}


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

	std::string _baseOSCPath;
	std::string _port;

	Config _config;

	FloatOutControl _x;
	FloatOutControl _y;
	FloatOutControl _z;
	FloatOutControl _azimuth;
	FloatOutControl _elevation;
	FloatOutControl _roll;
	TypedOutControl <int> _voice;
//	TypedOutControl <std::string> _voiceControlMessage;
	TypedOutControl <int> _play;
	TypedOutControl <int> _loop;

	typedef struct
	{
		std::string oscPath;
		std::string typespec;
	} PathHandler;

	std::vector<PathHandler> _configuredPaths;




};

} //namespace

#endif
