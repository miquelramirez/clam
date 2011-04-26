#ifndef DummyProcessing_hxx
#define DummyProcessing_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>


class DummyProcessing : public CLAM::Processing
{ 
	CLAM::InPort<float> mIn;
	CLAM::OutPort<float> mOut;
public:
	const char* GetClassName() const { return "DummyProcessing"; }
	DummyProcessing(const Config& config = Config()) 
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

#endif // DummyProcessing_hxx