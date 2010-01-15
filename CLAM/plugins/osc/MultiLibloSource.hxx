#ifndef MultiLibloSource_hxx
#define MultiLibloSource_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/Enum.hxx>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>

#include "LibloSingleton.hxx"

#include "lo/lo.h"
#ifdef GetClassName // windows.h introduces that
#undef GetClassName
#endif

namespace CLAM
{

class MultiLibloSourceConfig : public CLAM::ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE( MultiLibloSourceConfig, 5, ProcessingConfig );
	DYN_ATTRIBUTE( 0, public, std::string, OscPath);
	DYN_ATTRIBUTE( 1, public, unsigned int, ServerPort);
	DYN_ATTRIBUTE( 2, public, std::string, OSCTypeSpec);
	DYN_ATTRIBUTE( 3, public, bool, EnableMulticast);
	DYN_ATTRIBUTE( 4, public, std::string, MultiCastIP);
	//TODO number of arguments/ports
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
		SetOscPath("/clam/target");
		SetServerPort(0);
		SetOSCTypeSpec("fff");
		SetEnableMulticast(false);
		SetMultiCastIP("224.0.0.1");
	};
};

class MultiLibloSource : public CLAM::Processing
{
	typedef MultiLibloSourceConfig Config;

public:
	MultiLibloSource(const Config& config = Config()) 
		: _typespec("")
		, _oscPath(config.GetOscPath())
		, _port(0)
		, _libloSingleton(LibloSingleton::GetInstance())
		, _isConfigured(false)
	{
		Configure( config );
#ifdef __MULTICAST_LIBLO__
std::cout<<"liblomulticast"<<std::endl;
#endif
	}
	
	~MultiLibloSource()
	{
		if (IsConfigured())
			_libloSingleton.UnregisterOscCallback(_port,_oscPath,_typespec);
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
		std::vector<OutControlBase *>::iterator it;
		for (it=_outControls.begin();it!=_outControls.end();it++)
		{
			delete *it;
		}
		_outControls.clear();
		GetOutControls().Clear();
	}


//TODO: REFACTOR THE FOLLOWING METHODS!!!!

	OutControlBase * createControl(const std::string & type, const std::string & name)
	{
		if (type=="s")
			return new OutControl<std::string> (name,this);
		if (type=="f")
			return new OutControl<float> (name,this);
		if (type=="d")
			return new OutControl<double> (name,this);
		if (type=="i" or type=="h")
			return new OutControl<int> (name,this);
		// TODO: Decide whether ASSERTing (contract) or throw (control) 
		return 0;
	}

	static void sendControl(OutControlBase * control, lo_arg * valueToSend)
	{
		const std::string typeName=control->GetTypeId().name();
		if (typeName=="Ss")
			dynamic_cast<OutControl<std::string> *> (control)->SendControl(&(valueToSend->s));
		if(typeName=="f")
			dynamic_cast<OutControl<float> *>(control)->SendControl(valueToSend->f);
		if(typeName=="d")
			dynamic_cast<OutControl<double> *> (control)->SendControl(valueToSend->f32);
		if(typeName=="i")
			dynamic_cast<OutControl<int> *> (control)->SendControl(valueToSend->i);
		return;
	}

	const unsigned int GetOutputsNumber() const
	{
		unsigned nOutputs;
		std::string typespec;
		typespec=_config.GetOSCTypeSpec();
		for (nOutputs=0; nOutputs<typespec.size();nOutputs++)
		{
			const char oscType = typespec[nOutputs];
			if (oscType!='s' and oscType!='i' 
				and oscType != 'f' and oscType !='d'
				and oscType != 'h')
				return 0; // return 0 if there is any non-compatible type
		}
		return nOutputs;
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		RemoveOldControls();
		CopyAsConcreteConfig(_config, config);
		//set outputs:
		unsigned nOutputs = GetOutputsNumber();
		if (nOutputs==0)
		{
			AddConfigErrorMessage("No proper OSCTypeSpec setup. Use: 'f' for float, 'd' for double, 'i' for integer, 'h' for integer 64.");
			_isConfigured=false;
			return false;
		}

		// multi-port names share user-configured identifier
		std::string oscPath=_config.GetOscPath();
		std::replace(oscPath.begin(),oscPath.end(),'/','_');

		for (unsigned i=0;i<nOutputs;i++)
		{
			std::ostringstream controlName;
			controlName<<i;
			std::string type;
			type=_config.GetOSCTypeSpec()[i];
			_outControls.push_back(createControl(type,controlName.str()));
		}

		unsigned int port = _config.GetServerPort();
		/* not defined port */
		if (port==0)
		{
			AddConfigErrorMessage("No port defined");
			_isConfigured=false;
			return false;
		}
		if (_libloSingleton.IsPathRegistered(_port,_oscPath,_typespec) and _isConfigured) // if exists delete previous method (with previous _port, _oscPath and _typespec)
		{ 
			_libloSingleton.UnregisterOscCallback(_port,_oscPath,_typespec);
		}

		_typespec=_config.GetOSCTypeSpec();
		_oscPath=_config.GetOscPath();
		_port=port;

		bool registered=false;
		if (_config.GetEnableMulticast() and _config.GetMultiCastIP()!="")
			registered=_libloSingleton.RegisterOscCallback(_port, _oscPath, _typespec,&controls_handler,this,_config.GetMultiCastIP());
		else
			registered=_libloSingleton.RegisterOscCallback(_port, _oscPath, _typespec,&controls_handler,this);

		if (registered==false)
		{
			AddConfigErrorMessage(_libloSingleton.GetErrorMessage());
			_isConfigured=false;
			return false;
		}
		_isConfigured=true;
		return true; // Configuration ok
	}

protected:

	static int controls_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);

	// server management related structs, methods, and attributes

	std::string _typespec;
	std::string _oscPath;
	unsigned int _port;
	Config _config;
	std::vector <OutControlBase *> _outControls;
	CLAM::LibloSingleton & _libloSingleton;
	bool _isConfigured;
};

} //namespace

#endif
