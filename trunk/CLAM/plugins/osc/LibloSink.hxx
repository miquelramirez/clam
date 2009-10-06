#ifndef LibloSink_hxx
#define LibloSink_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/TypedInControl.hxx>
#include <CLAM/TypeInfo.hxx>
#include <CLAM/InControl.hxx>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include "lo/lo.h"
#include "lo/lo_lowlevel.h"
#undef GetClassName


namespace CLAM
{

class LibloSinkConfig : public CLAM::ProcessingConfig
{ 
	DYNAMIC_TYPE_USING_INTERFACE( LibloSinkConfig, 4, ProcessingConfig );
	DYN_ATTRIBUTE( 0, public, std::string, OscPath);
	DYN_ATTRIBUTE( 1, public, unsigned int, ServerPort);
	DYN_ATTRIBUTE( 2, public, std::string, OSCTypeSpec);
	DYN_ATTRIBUTE( 3, public, std::string, IPAddress);

    //TODO number of arguments/ports
protected:
    void DefaultInit()
    {
	AddAll();
	UpdateData();
	SetOscPath("/clam/target");
	SetServerPort(0);
	SetOSCTypeSpec("fff");
	SetIPAddress("127.0.0.1");
    };
};
class X
{
  X(const LibloSinkConfig& = LibloSinkConfig())
  {
  }
};

class LibloSink : public CLAM::Processing
{
	typedef LibloSinkConfig Config;
	lo_address _oscPort;
	Config _config;
	std::vector <InControlBase *> _inControls;
public:

	LibloSink(const Config& config = Config()) 
	{
		Configure( config );
	}

	~LibloSink()
	{
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

	void FloatInControlCallback (const TControlData & value)
	{
		SendOSCMessage();
	}
			
	void StringInControlCallback (const std::string & value)
	{
		SendOSCMessage();
	}
	void DoubleInControlCallback (const double & value)
	{
		SendOSCMessage();
	}
	void IntInControlCallback (const int & value)
	{
		SendOSCMessage();
	}

	void SendOSCMessage()
	{
		lo_message message=lo_message_new();
		for (unsigned i=0;i<_config.GetOSCTypeSpec().size();i++)
		{
			std::string typespec;
			typespec=_config.GetOSCTypeSpec()[i];

			if (typespec=="s")
				lo_message_add(message,typespec.c_str(),dynamic_cast<TypedInControl<std::string> *> (_inControls[i])->GetLastValue().c_str());
			if (typespec=="f")
				lo_message_add(message,typespec.c_str(),dynamic_cast<FloatInControl *>(_inControls[i])->GetLastValue());
			if (typespec=="d")
				lo_message_add(message,typespec.c_str(),dynamic_cast<TypedInControl<double> *> (_inControls[i])->GetLastValue());
			if (typespec=="i" or typespec=="h")
				lo_message_add(message,typespec.c_str(),dynamic_cast<TypedInControl<int> *> (_inControls[i])->GetLastValue());
		}
		if (lo_send_message(_oscPort,_config.GetOscPath().c_str(),message) == -1)
		{
			printf("OSC error %d: %s\n", lo_address_errno(_oscPort), lo_address_errstr(_oscPort));
		}

	}

	const char* GetClassName() const
	{
		return "LibloSink";
	}
protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		RemoveOldControls();
		CopyAsConcreteConfig(_config, config);

		char portCString[10];
		sprintf(portCString,"%u",_config.GetServerPort());
	
		_oscPort = lo_address_new(_config.GetIPAddress().c_str(), portCString);
		unsigned nInputs = GetInputsNumber();
		if (nInputs==0)
		{
			AddConfigErrorMessage("No proper OSCTypeSpec setup. Use: 'f' for float, 'd' for double, 'i' for integer, 'h' for integer 64.");
			return false;
		}

		// multi-port names share user-configured identifier
		std::string oscPath=_config.GetOscPath();
		std::replace(oscPath.begin(),oscPath.end(),'/','_');

		for (unsigned i=0;i<nInputs;i++)
		{
			std::ostringstream controlName;
			controlName<<i;
			std::string type;
			type=_config.GetOSCTypeSpec()[i];
			_inControls.push_back(createControl(type,controlName.str()));
		}

		unsigned int port = _config.GetServerPort();
		/* not defined port */
		if (port==0)
		{
			AddConfigErrorMessage("No port defined");
			return false;
		}

		return true; // Configuration ok
	}

        void RemoveOldControls()
        {
		std::vector<InControlBase *>::iterator it;
		for (it=_inControls.begin();it!=_inControls.end();it++)
		{
			delete *it;
		}
		_inControls.clear();
		GetInControls().Clear();
	}

	InControlBase * createControl(const std::string & type, const std::string & name)
	{
		if (type=="s")
			return new TypedInControl<std::string> (name, this, &LibloSink::StringInControlCallback);
		if (type=="f")
			return new FloatInControl (name, this, &LibloSink::FloatInControlCallback);
		if (type=="d")
			return new TypedInControl<double> (name, this, &LibloSink::DoubleInControlCallback);
		if (type=="i" or type=="h")
			return new TypedInControl<int> (name,this, &LibloSink::IntInControlCallback);
		// TODO: Decide whether ASSERTing (contract) or throw (control) 
		return 0;
	}

	const unsigned int GetInputsNumber() const
	{
		unsigned nInputs;
		std::string typespec;
		typespec=_config.GetOSCTypeSpec();
		for (nInputs=0; nInputs<typespec.size();nInputs++)
		{
			const char oscType = typespec[nInputs];
			if (oscType!='s' and oscType!='i' 
				and oscType != 'f' and oscType !='d'
				and oscType != 'h')
				return 0; // return 0 if there is any non-compatible type
		}
		return nInputs;
	}

};

} //namespace

#endif
