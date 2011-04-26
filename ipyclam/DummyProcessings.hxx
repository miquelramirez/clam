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

#endif // DummyProcessings_hxx