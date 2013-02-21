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
		return true;
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
		return true;
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
		: mIn("Inport1", this)
		, mIn2("Inport2", this)
		, mOut("Outport1", this)
		, mOut2("Outport2", this)
		, mInControl("Incontrol1", this)
		, mInControl2("Incontrol2", this)
		, mInControl3("Incontrol3", this)
		, mOutControl("Outcontrol1", this)
		, mOutControl2("Outcontrol2", this)
	{
		Configure( config );
	}
 
	bool Do()
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
		return true;
	}
};

// Created for compatibility with Dummy Testing
class DummyPortSink : public CLAM::Processing
{
private:
	CLAM::InPort<float> mIn;
public:
	const char* GetClassName() const { return "DummyPortSink"; }
	DummyPortSink(const Config& config = Config()) 
		: mIn("InPort1", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		return true;
	}
};

// Created for compatibility with Dummy Testing
class DummyPortSource : public CLAM::Processing
{
private:
	CLAM::OutPort<float> mOut;
public:
	const char* GetClassName() const { return "DummyPortSource"; }
	DummyPortSource(const Config& config = Config()) 
		: mOut("OutPort1", this)
	{
		Configure( config );
	}
 
	bool Do()
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
		return true;
	}

	bool Do(float in, float out)
	{
		return true;
	}
};

// Created for compatibility with Dummy Testing
class DummyControlSource : public CLAM::Processing
{
private:
	CLAM::OutControl<float> mOutControl;
public:
	const char* GetClassName() const { return "DummyControlSource"; }
	DummyControlSource(const Config& config = Config()) 
		: mOutControl("OutControl1", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		return true;
	}

	bool Do(float in, float out)
	{
		return true;
	}
};

// Created for compatibility with Dummy Testing
class ProcessingWithNameSpacedPorts : public CLAM::Processing
{
private:
	CLAM::InPort<float> mIn;
	CLAM::OutPort<float> mOut;
public:
	const char* GetClassName() const { return "ProcessingWithNameSpacedPorts"; }
	ProcessingWithNameSpacedPorts(const Config& config = Config()) 
		: mIn("An inport", this)
		, mOut("An outport", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		return true;
	}

	bool Do(float in, float out)
	{
		return true;
	}
};

// Created for compatibility with Dummy Testing
class ProcessingWithNameSpacedControls : public CLAM::Processing
{
private:
	CLAM::InControl<float> mInControl;
	CLAM::OutControl<float> mOutControl;
public:
	const char* GetClassName() const { return "ProcessingWithNameSpacedControls"; }
	ProcessingWithNameSpacedControls(const Config& config = Config()) 
		: mInControl("An incontrol", this)
		, mOutControl("An outcontrol", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		return true;
	}

	bool Do(float in, float out)
	{
		return true;
	}
};

// Created for compatibility with Dummy Testing
class ProcessingWithNumericPorts : public CLAM::Processing
{
private:
	CLAM::InPort<float> mIn;
	CLAM::OutPort<float> mOut;
	CLAM::OutPort<float> mOut2;
public:
	const char* GetClassName() const { return "ProcessingWithNumericPorts"; }
	ProcessingWithNumericPorts(const Config& config = Config()) 
		: mIn("1", this)
		, mOut("1", this)
		, mOut2("2", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		return true;
	}

	bool Do(float in, float out)
	{
		return true;
	}
};
class Dummy6IOPorts : public CLAM::Processing
{
	CLAM::InPort<float> mIn1;
	CLAM::InPort<float> mIn2;
	CLAM::InPort<float> mIn3;
	CLAM::InPort<float> mIn4;
	CLAM::InPort<float> mIn5;
	CLAM::InPort<float> mIn6;
	CLAM::OutPort<float> mOut1;
	CLAM::OutPort<float> mOut2;
	CLAM::OutPort<float> mOut3;
	CLAM::OutPort<float> mOut4;
	CLAM::OutPort<float> mOut5;
	CLAM::OutPort<float> mOut6;
public:
	const char* GetClassName() const { return "Dummy6IOPorts"; }
	Dummy6IOPorts(const Config& config = Config()) 
		: mIn1("inport1", this)
		, mIn2("inport2", this)
		, mIn3("inport3", this)
		, mIn4("inport4", this)
		, mIn5("inport5", this)
		, mIn6("inport6", this)
		, mOut1("outport1", this)
		, mOut2("outport2", this)
		, mOut3("outport3", this)
		, mOut4("outport4", this)
		, mOut5("outport5", this)
		, mOut6("outport6", this)
	{
		Configure( config );
	}
 
	bool Do() { return true; }
};

class Dummy6IOControls : public CLAM::Processing
{
	CLAM::InControl<float> mIn1;
	CLAM::InControl<float> mIn2;
	CLAM::InControl<float> mIn3;
	CLAM::InControl<float> mIn4;
	CLAM::InControl<float> mIn5;
	CLAM::InControl<float> mIn6;
	CLAM::OutControl<float> mOut1;
	CLAM::OutControl<float> mOut2;
	CLAM::OutControl<float> mOut3;
	CLAM::OutControl<float> mOut4;
	CLAM::OutControl<float> mOut5;
	CLAM::OutControl<float> mOut6;
public:
	const char* GetClassName() const { return "Dummy6IOControls"; }
	Dummy6IOControls(const Config& config = Config()) 
		: mIn1("incontrol1", this)
		, mIn2("incontrol2", this)
		, mIn3("incontrol3", this)
		, mIn4("incontrol4", this)
		, mIn5("incontrol5", this)
		, mIn6("incontrol6", this)
		, mOut1("outcontrol1", this)
		, mOut2("outcontrol2", this)
		, mOut3("outcontrol3", this)
		, mOut4("outcontrol4", this)
		, mOut5("outcontrol5", this)
		, mOut6("outcontrol6", this)
	{
		Configure( config );
	}
 
	bool Do() { return true; }
};

#endif // DummyProcessings_hxx
