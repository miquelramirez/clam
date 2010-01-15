#ifndef SpatDIFSourceReceiver_hxx
#define SpatDIFSourceReceiver_hxx

#include <CLAM/Processing.hxx>
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
	DYNAMIC_TYPE_USING_INTERFACE( SpatDIFSourceReceiverConfig, 5, ProcessingConfig );
	DYN_ATTRIBUTE( 0, public, std::string, SourceName);
	DYN_ATTRIBUTE( 1, public, unsigned int, VoicesNumber);
	DYN_ATTRIBUTE( 2, public, unsigned int, ServerPort);
	DYN_ATTRIBUTE( 3, public, bool, EnableMulticast);
	DYN_ATTRIBUTE( 4, public, std::string, MultiCastIP);
	//TODO number of arguments/ports
	
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
		SetSourceName("");
		SetVoicesNumber(4);
		SetServerPort(0);
		SetEnableMulticast(false);
		SetMultiCastIP("224.0.0.1");
	};
};

class SpatDIFSourceReceiver : public CLAM::Processing
{
	typedef SpatDIFSourceReceiverConfig Config;

public:
	SpatDIFSourceReceiver(const Config& config = Config()) 
		: _baseOSCPath("")
		, _port(0)
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
			dynamic_cast<OutControl<std::string> *> (control)->SendControl(&(valueToSend->s));
		if(typeName=="f")
			dynamic_cast<FloatOutControl *>(control)->SendControl(valueToSend->f);
		if(typeName=="d")
			dynamic_cast<OutControl<double> *> (control)->SendControl(valueToSend->f32);
		if(typeName=="i")
			dynamic_cast<OutControl<int> *> (control)->SendControl(valueToSend->i);
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

		unsigned int port = _config.GetServerPort();
		/* not defined port */
		if (port==0)
		{
			AddConfigErrorMessage("No port defined");
			return false;
		}

		_port=port;

		// if exists previous callback methods clear them
//		if (_libloSingleton.GetConfiguredPaths().size()!=0)
		UnregisterOscCallbacks();

		if (RegisterOscCallbacks()==false)
		{
			AddConfigErrorMessage(_libloSingleton.GetErrorMessage());
			return false;
		}
		// add instance on map
		return true; // Configuration ok
	}

	const bool RegisterCallback(const unsigned int & port, const std::string & path,
				 const std::string & typespec, lo_method_handler callbackMethod)
	{
		bool registered=false;
		if (_config.GetEnableMulticast() and _config.GetMultiCastIP()!="")
			registered=_libloSingleton.RegisterOscCallback(port,path,typespec,callbackMethod,this,_config.GetMultiCastIP());
		else
			registered=_libloSingleton.RegisterOscCallback(port,path,typespec,callbackMethod,this);

		if (registered)
		{
			CLAM::LibloSingleton::OSCInstance instance;
			instance.path=path;
			instance.port=port;
			instance.typespec=typespec;
			instance.thread=NULL;
			_OSCInstances.push_back(instance);
		}
		return registered;
	}

	const bool UnregisterCallback(const unsigned int & port, const std::string & path,
				const std::string & typespec)
	{
		std::list<CLAM::LibloSingleton::OSCInstance>::iterator it;
		for (it=_OSCInstances.begin();it!=_OSCInstances.end();it++)
		{
			CLAM::LibloSingleton::OSCInstance instance=(*it);
			if (instance.port==port and instance.path==path and instance.typespec==typespec)
			{
				_OSCInstances.erase(it);
				_libloSingleton.UnregisterOscCallback(port,path,typespec);
				return true;
			}
		}
		return false;
	}

	bool RegisterOscCallbacks()
	{
		bool allOK=true;
		std::string oscPath;
		oscPath=_baseOSCPath+"/xyz";
		allOK &= RegisterCallback(_port,oscPath,"fff",&controls_handler);
		oscPath=_baseOSCPath+"/ypr";
		allOK &= RegisterCallback(_port,oscPath,"fff",&controls_handler);
		oscPath=_baseOSCPath+"/velocity";
		allOK &= RegisterCallback(_port,oscPath,"fff",&controls_handler);
		for (unsigned i=0;i<_config.GetVoicesNumber();i++)
		{
			std::ostringstream voice;
			voice<<i;
			oscPath=_baseOSCPath+"/sampler/"+voice.str()+"/play";

			allOK &= RegisterCallback(_port,oscPath,"ii",&controls_handler); // play with loop setting
			allOK &= RegisterCallback(_port,oscPath,"i",&controls_handler); //play without loop setting
			oscPath=_baseOSCPath+"/sampler/"+voice.str()+"/setLoop";
			allOK &= RegisterCallback(_port,oscPath,"i",&controls_handler);
		}
		return allOK;
	}

	void UnregisterOscCallbacks()
	{
		while (_OSCInstances.size()!=0)
		{
			CLAM::LibloSingleton::OSCInstance instance=_OSCInstances.front();
			UnregisterCallback(instance.port, instance.path, instance.typespec);
		}
	}

private:

	std::string _baseOSCPath;
	unsigned int _port;

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

	OutControl <CLAM::MultiSampler::SamplerMessage> _outputSamplerMessage;
	CLAM::LibloSingleton & _libloSingleton;
	std::list<CLAM::LibloSingleton::OSCInstance> _OSCInstances;

};

} //namespace

#endif
