#ifndef LibloSink_hxx
#define LibloSink_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/TypeInfo.hxx>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include "lo/lo.h"
#include <time.h>
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

class LibloSink : public CLAM::Processing
{
	typedef LibloSinkConfig Config;
	Config _config;
	FloatInControl _portControlChange;
	std::vector <InControlBase *> _inControls;
public:

	LibloSink(const Config& config = Config()) 
		: _portControlChange("OSC port",this)
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


	template <typename T> 
	void InControlCallback (const T & value)
	{
		SendOSCMessage();
	}

	void SendOSCMessage()
	{
		std::ostringstream portString;
		portString << _portControlChange.GetLastValue();
		lo_address oscLibloAddress = lo_address_new(_config.GetIPAddress().c_str(), portString.str().c_str());

		lo_message message=lo_message_new();
		const std::string & typespec = _config.GetOSCTypeSpec();
		for (unsigned i=0;i<typespec.size();i++)
		{
			switch (typespec[i])
			{
				case 's':
					lo_message_add_string(message,dynamic_cast<InControl<std::string> *> (_inControls[i])->GetLastValue().c_str());
					break;
				case 'f':
					lo_message_add_float(message,dynamic_cast<InControl<float>*>(_inControls[i])->GetLastValue());
					break;
				case 'd':
					lo_message_add_double(message,dynamic_cast<InControl<double> *> (_inControls[i])->GetLastValue());
					break;
				case 'i':
					lo_message_add_int32(message,dynamic_cast<InControl<int> *> (_inControls[i])->GetLastValue());
					break;
				case 'h':
					lo_message_add_int64(message,dynamic_cast<InControl<long int> *> (_inControls[i])->GetLastValue());
					break;
			}
		}
		if (lo_send_message(oscLibloAddress,_config.GetOscPath().c_str(),message) == -1)
		{
			printf("OSC error %d: %s\n", lo_address_errno(oscLibloAddress), lo_address_errstr(oscLibloAddress));
		}

	}

	const char* GetClassName() const
	{
		return "LibloSink";
	}
protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		std::string previousTypes = _config.GetOSCTypeSpec();
		CLAM_ASSERT(_inControls.size()<=previousTypes.size(),
			"LibloSink: Allocated controls should be less or equal to the types spec");
		CopyAsConcreteConfig(_config, config);

		const std::string & newTypes = _config.GetOSCTypeSpec();
		unsigned commonSize = 0;
		for (; commonSize<newTypes.size(); commonSize++)
		{
			if (commonSize==_inControls.size()) break;
			if (commonSize==previousTypes.size()) break; // Should be redundant (see previous assert)
			if (newTypes[commonSize]!=previousTypes[commonSize]) break;
		}
		for (unsigned i=commonSize; i<_inControls.size(); i++)
			delete _inControls[i];
		_inControls.resize(commonSize);
		for (unsigned i=commonSize; i<newTypes.size(); i++)
		{
			std::ostringstream controlName;
			controlName<<i;
			InControlBase * control = createControl(newTypes[i], controlName.str());
			if (not control) return AddConfigErrorMessage(
				"No proper OSCTypeSpec setup. Use: "
				"'f' for float,\n"
				"'d' for double,\n"
				"'i' for integer,\n"
				"'h' for long integer,\n"
				"'s' for string.");
			_inControls.push_back(control);
		}

		unsigned port = _config.GetServerPort();
		if (port==0) return AddConfigErrorMessage("No port defined");
		_portControlChange.DoControl(port);
		return true; // Configuration ok
	}

	void RemoveOldControls()
	{
		for (unsigned i=0; i<_inControls.size(); i++)
			delete _inControls[i];
		_inControls.clear();
	}

	InControlBase * createControl(char type, const std::string & name)
	{
		switch (type)
		{
			case 's':
				return new InControl<std::string> (name, this, &LibloSink::InControlCallback<std::string>);
			case 'f':
				return new InControl<float> (name, this, &LibloSink::InControlCallback<float>);
			case 'd':
				return new InControl<double> (name, this, &LibloSink::InControlCallback<double>);
			case 'i':
				return new InControl<int> (name,this, &LibloSink::InControlCallback<int>);
			case 'h':
				return new InControl<long int> (name,this, &LibloSink::InControlCallback<long int>);
			default:
				return 0;
				// TODO: Decide whether ASSERTing (contract) or throw (control) 
		}
	}
};

} //namespace

#endif
