#ifndef SpatDIFSourceReceiver_hxx
#define SpatDIFSourceReceiver_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/TypedOutControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/ControlArray.hxx>
#include <CLAM/Enum.hxx>
#include <CLAM/Text.hxx>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>


#include "MultiLibloSource.hxx"
#include "MultiSampler.hxx"
#include "LibloSingleton.hxx"

#include "lo/lo.h"
#ifdef GetClassName // windows.h introduces that
#undef GetClassName
#endif


namespace CLAM
{

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
		, _velocityX("X instant velocity",this)
		, _velocityY("Y instant velocity",this)
		, _velocityZ("Z instant velocity",this)
		, _outputSamplerMessage("Sampler typed messages",this)
		, _libloSingleton(LibloSingleton::GetInstance())
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

	static int controls_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);
protected:

	static std::string SplitPath(std::string input, unsigned tokenNumber)
	{
	unsigned i=0;
	char* inputcharp=(char*)input.c_str();
	inputcharp=std::strtok(inputcharp,"/");

	while (i!=tokenNumber and inputcharp!=NULL)
	{
		inputcharp=std::strtok(NULL,"/");
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

		_libloSingleton=_libloSingleton.GetInstance();
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

		_port=port;

std::cout<<"debug - - - - - configuredpaths: " << _libloSingleton.GetConfiguredPaths().size()<<std::endl;


		// if exists previous callback methods clear them
		if (_libloSingleton.GetConfiguredPaths().size()!=0)
			UnregisterOscCallbacks();

		_serverThread=_libloSingleton.GetServerThread(port);

		if (RegisterOscCallbacks()==false)
		{
			AddConfigErrorMessage("Error creating OSC server instance");
			return false;
		}
		// add instance on map
		return true; // Configuration ok
	}

	bool RegisterOscCallbacks()
	{
		bool allOK=true;
		std::string oscPath;
		oscPath=_baseOSCPath+"/xyz";
		allOK &= _libloSingleton.RegisterOscCallback(_port,oscPath,"fff",&controls_handler);
		oscPath=_baseOSCPath+"/ypr";
		allOK &= _libloSingleton.RegisterOscCallback(_port,oscPath,"fff",&controls_handler);
		oscPath=_baseOSCPath+"/velocity";
		allOK &= _libloSingleton.RegisterOscCallback(_port,oscPath,"fff",&controls_handler);
		for (unsigned i=0;i<_config.GetVoicesNumber();i++)
		{
			std::ostringstream voice;
			voice<<i;
			oscPath=_baseOSCPath+"/sampler/"+voice.str()+"/play";

			allOK &= _libloSingleton.RegisterOscCallback(_port,oscPath,"ii",&controls_handler); // play with loop setting
			allOK &= _libloSingleton.RegisterOscCallback(_port,oscPath,"i",&controls_handler); //play without loop setting
			oscPath=_baseOSCPath+"/sampler/"+voice.str()+"/setLoop";
			allOK &= _libloSingleton.RegisterOscCallback(_port,oscPath,"i",&controls_handler);
		}
		return allOK;
	}

	void UnregisterOscCallbacks()
	{

std::cout<<"unregistering callbacks...."<<std::endl;
		while (_libloSingleton.GetConfiguredPaths().size()!=0)
		{
std::cout<<"\tgetconfiguredpaths.size: "<<_libloSingleton.GetConfiguredPaths().size()<<std::endl;
			CLAM::LibloSingleton::PathHandler firstPath=_libloSingleton.GetConfiguredPaths().front();
std::cout<<"\tpath= "<<firstPath.oscPath << " - port: "<<_port<<" - typespec: "<<firstPath.typespec<<std::endl;
			_libloSingleton.UnregisterOscCallback(_port, firstPath.oscPath, firstPath.typespec);
		}
	}

private:

	std::string _baseOSCPath;
	std::string _port;

	Config _config;

	FloatOutControl _x;
	FloatOutControl _y;
	FloatOutControl _z;
	FloatOutControl _azimuth;
	FloatOutControl _elevation;
	FloatOutControl _roll;
	FloatOutControl _velocityX;
	FloatOutControl _velocityY;
	FloatOutControl _velocityZ;

	TypedOutControl <CLAM::MultiSampler::SamplerMessage> _outputSamplerMessage;

	lo_server_thread _serverThread;
	CLAM::LibloSingleton & _libloSingleton;


};

} //namespace

#endif
