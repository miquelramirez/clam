#ifndef MyProcessing_hxx
#define MyProcessing_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Spectrum.hxx>

class MyProcessing : public CLAM::Processing
{
	CLAM::InPort<CLAM::Spectrum> mIn;
	CLAM::OutPort<CLAM::Spectrum> mOut;
public:
	const char* GetClassName() const { return "MyProcessing"; }
	MyProcessing(const Config& config = Config()) 
		: mIn("My Input", this)
		, mOut("My Output", this) 
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

	bool Do(const CLAM::Spectrum& in, CLAM::Spectrum& out)
	{
		// Your implementation
		return true;
	}
};

#endif // MyProcessing_hxx
