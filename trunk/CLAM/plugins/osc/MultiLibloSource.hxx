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
		: _registered(false)
		, _typespec("")
		, _oscPath(config.GetOscPath())
		, _port(0)
		, _libloSingleton(LibloSingleton::GetInstance())
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
			delete *it;
		_outControls.clear();
	}


	OutControlBase * createControl(const std::string & type, const std::string & name)
	{
		if (type=="s") return new OutControl<std::string> (name,this);
		if (type=="f") return new OutControl<float> (name,this);
		if (type=="d") return new OutControl<double> (name,this);
		if (type=="i") return new OutControl<int> (name,this);
		if (type=="h") return new OutControl<int> (name,this); // TODO: Should it be long?
		return 0;
	}

	void sendControl(unsigned i, lo_arg * valueToSend)
	{
		if (i>=GetNOutControls()) return;
		OutControlBase * control = &GetOutControl(i);
		const std::string typeName=control->GetTypeId().name();
		if (typeName=="s")
			dynamic_cast<OutControl<std::string> *> (control)->SendControl(&(valueToSend->s));
		if(typeName=="f")
			dynamic_cast<OutControl<float> *>(control)->SendControl(valueToSend->f);
		if(typeName=="d")
			dynamic_cast<OutControl<double> *> (control)->SendControl(valueToSend->f32);
		if(typeName=="i")
			dynamic_cast<OutControl<int> *> (control)->SendControl(valueToSend->i);
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		std::string previousTypes = _config.GetOSCTypeSpec();
		CLAM_ASSERT(_outControls.size()<=previousTypes.size(),
			"MultiLibloSource: Allocated controls should be less or equal to the types spec");

		// if exists delete previous method (with previous _port, _oscPath and _typespec)
		if (_registered)
		{
			_libloSingleton.UnregisterOscCallback(_port,_oscPath,_typespec);
			_registered=false;
		}

		CopyAsConcreteConfig(_config, config);

		//set outputs:
		const std::string & newTypes = _config.GetOSCTypeSpec();
		unsigned commonSize = 0;
		for (; commonSize<newTypes.size(); commonSize++)
		{
			if (commonSize==_outControls.size()) break;
			if (commonSize==previousTypes.size()) break; // Should be redundant (see previous assert)
			if (newTypes[commonSize]!=previousTypes[commonSize]) break;
		}
		for (unsigned i=commonSize; i<_outControls.size(); i++)
			delete _outControls[i];
		_outControls.resize(commonSize);
		for (unsigned i=commonSize; i<newTypes.size(); i++)
		{
			std::ostringstream controlName;
			controlName<<i;
			OutControlBase * control = createControl(
				newTypes.substr(i,1), controlName.str());
			if (not control) return AddConfigErrorMessage(
				"No proper OSCTypeSpec setup. Use: "
				"'f' for float,\n"
				"'d' for double,\n"
				"'i' for integer,\n"
				"'h' for long integer,\n"
				"'s' for string.");
			_outControls.push_back(control);
		}

		unsigned port = _config.GetServerPort();
		if (port==0) return AddConfigErrorMessage("No port defined");

		_typespec=_config.GetOSCTypeSpec();
		_oscPath=_config.GetOscPath();
		_port=port;

		std::string multiCastIP = _config.GetEnableMulticast()?
			_config.GetMultiCastIP() : "";
		_registered=_libloSingleton.RegisterOscCallback(
			_port, _oscPath, _typespec,
			&controls_handler,this,
			multiCastIP);

		if (not _registered) return AddConfigErrorMessage(_libloSingleton.GetErrorMessage());

		return true; // Configuration ok
	}

protected:

	static int controls_handler(
		const char *path, const char *types,
		lo_arg **argv, int argc,
		void *data, void *user_data);

	// server management related structs, methods, and attributes
	bool _registered;
	std::string _typespec;
	std::string _oscPath;
	unsigned _port;

	Config _config;
	std::vector <OutControlBase *> _outControls;
	CLAM::LibloSingleton & _libloSingleton;
};

} //namespace

#endif
