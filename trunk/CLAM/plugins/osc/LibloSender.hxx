#ifndef LibloSender_hxx
#define LibloSender_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <string>
#include <cmath>
#include "lo/lo.h"


namespace CLAM
{

class LibloSenderConfig : public CLAM::ProcessingConfig
{ 
    DYNAMIC_TYPE_USING_INTERFACE( LibloSenderConfig, 1, ProcessingConfig );
    DYN_ATTRIBUTE( 0, public, std::string, OscPath);
    //TODO number of arguments/ports
protected:
    void DefaultInit()
    {
          AddAll();
          UpdateData();
          SetOscPath("/clam/target");
    };
};

class LibloSender : public CLAM::Processing
{
	typedef LibloSenderConfig Config;
	lo_address _oscPort;
public:
	LibloSender(const Config& config = Config()) 
		: _in1("osc argument 1", this)
		, _in2("osc argument 2", this)
		, _in3("osc argument 3", this)
	{
		Configure( config );
		_oscPort = lo_address_new(NULL, "7780");
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}
	void SendOsc(float f1, float f2, float f3)
	{

	//	std::cout << f1 << " " << f2 << " " << f3 << std::endl;
		if (lo_send(_oscPort, "/navigator", "fff", f1, f2, f3) == -1) {
			printf("OSC error %d: %s\n", lo_address_errno(_oscPort), lo_address_errstr(_oscPort));
	      }
	}
	bool Do()
	{

		SendOsc(	
			_in1.GetLastValue(), 
			_in2.GetLastValue(), 
			_in3.GetLastValue()
			);
		return true;
	}
	const char* GetClassName() const
	{
		return "LibloSender";
	}
protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		return true; // Configuration ok
	}

	Config _config;
	InControl _in1;
	InControl _in2;
	InControl _in3;
};

} //namespace

#endif
