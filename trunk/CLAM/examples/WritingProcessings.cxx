
#ifndef MyProcessing_hxx
#define MyProcessing_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/NullProcessingConfig.hxx>

// Those two configuration types could be the ones you choose.
// Your own ones or any type already in CLAM.
//#include "MyInputDataType.hxx"
//#include "MyOutputDataType.hxx"
typedef float MyInputDataType;
typedef float MyOutputDataType;

class MyProcessing : public CLAM::Processing
{ 
	CLAM::InPort<MyInputDataType> mIn;
	CLAM::OutPort<MyOutputDataType> mOut;
public:
	const char* GetClassName() const { return "MyProcessing"; }
	MyProcessing() 
		: mIn("My Input", this)
		, mOut("My Output", this) 
	{
		Configure( Config() );
	}
 
	bool ConcreteConfigure(const CLAM::ProcessingConfig&)
	{
		return true; // Always gets properly configurated
	}
	
	bool Do()
	{
		bool result = Do(mIn.GetData(), mOut.GetData()); 
		// Tell the ports this is done
		mIn.Consume();
		mOut.Produce();
		return result;
	}

	bool Do(const MyInputDataType& in, MyOutputDataType& out)
	{
		// Your implementation
	}
};

int main()
{
//	CLAM_DEBUG_ASSERT(false, "debug---");
	MyProcessing proc;
	std::cout << "State: "<< proc.GetExecStateString() << std::endl;
	std::cout << "Config error message: " << proc.GetConfigErrorMessage() << std::endl;
//	proc.Do();
	std::cout << "Done." << std::endl;
	return 0;
}
#endif // MyProcessing_hxx
