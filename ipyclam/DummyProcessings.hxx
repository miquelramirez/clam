#ifndef DummyProcessings_hxx
#define DummyProcessings_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/Processing.hxx>

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

#endif // DummyProcessings_hxx