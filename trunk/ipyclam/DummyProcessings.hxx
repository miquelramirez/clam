#ifndef DummyProcessings_hxx
#define DummyProcessings_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/Processing.hxx>

#include "DummyConfigurations.hxx"

class DummyProcessingWithInAndOutPorts : public CLAM::Processing
{ 
	CLAM::InPort<float> mIn;
	CLAM::OutPort<float> mOut;
public:
	const char* GetClassName() const { return "DummyProcessingWithInAndOutPorts"; }
	DummyProcessingWithInAndOutPorts(const Config& config = Config()) 
		: mIn("Inport1", this)
		, mOut("Outport1", this) 
	{
		Configure( config );
	}
 
	bool Do()
	{
		bool result = Do(mIn.GetData(), mOut.GetData()); 
		// Tell the ports this is done
		mIn.Consume();
		mOut.Produce();
		return result;
	}

	bool Do(float in, float out)
	{
		return true;
	}
};

class DummyProcessingWithInAndOutControls : public CLAM::Processing
{
	CLAM::InControl<float> mInControl;
	CLAM::OutControl<float> mOutControl;
public:
	const char* GetClassName() const { return "DummyProcessingWithInAndOutControls"; }
	DummyProcessingWithInAndOutControls(const Config& config = Config()) 
		: mInControl("Incontrol1", this)
		, mOutControl("Outcontrol1", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		return 1;
	}

	bool Do(float in, float out)
	{
		return true;
	}
};

class DummyProcessingWithMultiplePortsAndControls : public CLAM::Processing
{
	CLAM::InPort<float> mIn;
	CLAM::InPort<float> mIn2;
	CLAM::OutPort<float> mOut;
	CLAM::OutPort<float> mOut2;
	CLAM::InControl<float> mInControl;
	CLAM::InControl<float> mInControl2;
	CLAM::OutControl<float> mOutControl;
	CLAM::OutControl<float> mOutControl2;
public:
	const char* GetClassName() const { return "DummyProcessingWithMultiplePortsAndControls"; }
	DummyProcessingWithMultiplePortsAndControls(const Config& config = Config()) 
		: mIn("Inport1", this)
		, mIn2("Inport2", this)
		, mOut("Outport1", this)
		, mOut2("Outport2", this)
		, mInControl("Incontrol1", this)
		, mInControl2("Incontrol2", this)
		, mOutControl("Outcontrol1", this)
		, mOutControl2("Outcontrol2", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		return 1;
	}

	bool Do(float in, float out)
	{
		return true;
	}
};

class DummyProcessingWithStringConfiguration : public CLAM::Processing
{
public:
	typedef IpyclamTests::DummyDTWithJustStrings Config;
private:
	Config mConfig;

public:
	const char* GetClassName() const { return "DummyProcessingWithStringConfiguration"; }
	DummyProcessingWithStringConfiguration(const Config& config = Config()) 
	{
		Configure( config );
	}
 
	bool Do()
	{
		return 1;
	}

	bool Do(float in, float out)
	{
		return true;
	}
	
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return mConfig;
	}

protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(mConfig, config);
		// Use here the config to setup you object
		return true; // Configuration ok
	}
};

class DummyProcessingWithIntConfiguration : public CLAM::Processing
{
public:
	typedef IpyclamTests::DummyDTWithJustIntegers Config;
private:
	Config mConfig;

public:
	const char* GetClassName() const { return "DummyProcessingWithIntConfiguration"; }
	DummyProcessingWithIntConfiguration(const Config& config = Config()) 
	{
		Configure( config );
	}
 
	bool Do()
	{
		return 1;
	}

	bool Do(float in, float out)
	{
		return true;
	}
	
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return mConfig;
	}

protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(mConfig, config);
		// Use here the config to setup you object
		return true; // Configuration ok
	}
};

class DummyProcessingWithCompleteConfiguration : public CLAM::Processing
{
public:
	typedef IpyclamTests::DummyDTWithEveryType Config;
private:
	Config mConfig;
public:
	const char* GetClassName() const { return "DummyProcessingWithCompleteConfiguration"; }
	DummyProcessingWithCompleteConfiguration(const Config& config = Config()) 
	{
		Configure( config );
	}
 
	bool Do()
	{
		return 1;
	}

	bool Do(float in, float out)
	{
		return true;
	}
	
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return mConfig;
	}

protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(mConfig, config);
		// Use here the config to setup you object
		return true; // Configuration ok
	}
};

class DummyProcessingWithOptionalAttributes : public CLAM::Processing
{
public:
	typedef IpyclamTests::DummyDTWithOptionalAttributes Config;
private:
	Config mConfig;

public:
	const char* GetClassName() const { return "DummyProcessingWithOptionalAttributes"; }
	DummyProcessingWithOptionalAttributes(const Config& config = Config()) 
	{
		Configure( config );
	}
 
	bool Do()
	{
		return 1;
	}

	bool Do(float in, float out)
	{
		return true;
	}
	
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return mConfig;
	}

protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(mConfig, config);
		// Use here the config to setup you object
		return true; // Configuration ok
	}
};

// Created for compatibility with Dummy Testing
class DummyTypeProcessing1 : public CLAM::Processing
{
public:
	typedef IpyclamTests::DummyTestsConfig Config;
private:
	Config mConfig;
	CLAM::InPort<float> mIn;
	CLAM::InPort<float> mIn2;
	CLAM::InPort<float> mIn3;
	CLAM::InPort<float> mIn4;
	CLAM::OutPort<float> mOut;
	CLAM::OutPort<float> mOut2;
	CLAM::InControl<float> mInControl;
	CLAM::InControl<float> mInControl2;
	CLAM::OutControl<float> mOutControl;
	CLAM::OutControl<float> mOutControl2;
public:
	const char* GetClassName() const { return "DummyTypeProcessing1"; }
	DummyTypeProcessing1(const Config& config = Config()) 
		: mIn("InPort1", this)
		, mIn2("InPort2", this)
		, mIn3("InPort3", this)
		, mIn4("InPort4", this)
		, mOut("OutPort1", this)
		, mOut2("OutPort2", this)
		, mInControl("InControl1", this)
		, mInControl2("InControl2", this)
		, mOutControl("OutControl1", this)
		, mOutControl2("OutControl2", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		return 1;
	}

	bool Do(float in, float out)
	{
		return true;
	}
	
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return mConfig;
	}

protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(mConfig, config);
		// Use here the config to setup you object
		return true; // Configuration ok
	}
};

// Created for compatibility with Dummy Testing
class DummyTypeProcessing2 : public CLAM::Processing
{
public:
	typedef IpyclamTests::DummyTestsConfig Config;
private:
	Config mConfig;
	CLAM::InPort<float> mIn;
	CLAM::InPort<float> mIn2;
	CLAM::OutPort<float> mOut;
	CLAM::OutPort<float> mOut2;
	CLAM::InControl<float> mInControl;
	CLAM::InControl<float> mInControl2;
	CLAM::InControl<int> mInControl3;
	CLAM::OutControl<float> mOutControl;
	CLAM::OutControl<float> mOutControl2;
public:
	const char* GetClassName() const { return "DummyTypeProcessing2"; }
	DummyTypeProcessing2(const Config& config = Config())
		: mIn("InPort1", this)
		, mIn2("InPort2", this)
		, mOut("OutPort1", this)
		, mOut2("OutPort2", this)
		, mInControl("InControl1", this)
		, mInControl2("InControl2", this)
		, mInControl3("InControl3", this)
		, mOutControl("OutControl1", this)
		, mOutControl2("OutControl2", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		return 1;
	}

	bool Do(float in, float out)
	{
		return true;
	}
	
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return mConfig;
	}

protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(mConfig, config);
		// Use here the config to setup you object
		return true; // Configuration ok
	}
};

// Created for compatibility with Dummy Testing
class ProcessingWithPortsAndControls : public CLAM::Processing
{
private:
	CLAM::InPort<float> mIn;
	CLAM::InPort<float> mIn2;
	CLAM::InPort<float> mIn3;
	CLAM::OutPort<float> mOut;
	CLAM::OutPort<float> mOut2;
	CLAM::OutPort<float> mOut3;
	CLAM::OutPort<float> mOut4;
	CLAM::InControl<float> mInControl;
	CLAM::InControl<float> mInControl2;
	CLAM::InControl<float> mInControl3;
	CLAM::InControl<float> mInControl4;
	CLAM::OutControl<float> mOutControl;
	CLAM::OutControl<float> mOutControl2;
	CLAM::OutControl<float> mOutControl3;
public:
	const char* GetClassName() const { return "ProcessingWithPortsAndControls"; }
	ProcessingWithPortsAndControls(const Config& config = Config()) 
		: mIn("InPort1", this)
		, mIn2("InPort2", this)
		, mIn3("InPort3", this)
		, mOut("OutPort1", this)
		, mOut2("OutPort2", this)
		, mOut3("OutPort3", this)
		, mOut4("OutPort4", this)
		, mInControl("InControl1", this)
		, mInControl2("InControl2", this)
		, mInControl3("InControl3", this)
		, mInControl4("InControl4", this)
		, mOutControl("OutControl1", this)
		, mOutControl2("OutControl2", this)
		, mOutControl3("OutControl3", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		return 1;
	}

	bool Do(float in, float out)
	{
		return true;
	}
};

// Created for compatibility with Dummy Testing
class PortSink : public CLAM::Processing
{
private:
	CLAM::InPort<float> mIn;
public:
	const char* GetClassName() const { return "PortSink"; }
	PortSink(const Config& config = Config()) 
		: mIn("InPort1", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		return 1;
	}

	bool Do(float in, float out)
	{
		return true;
	}
};

// Created for compatibility with Dummy Testing
class DummyControlSink : public CLAM::Processing
{
private:
	CLAM::InControl<float> mInControl;
public:
	const char* GetClassName() const { return "DummyControlSink"; }
	DummyControlSink(const Config& config = Config()) 
		: mInControl("InControl1", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		return 1;
	}

	bool Do(float in, float out)
	{
		return true;
	}
};

#endif // DummyProcessings_hxx