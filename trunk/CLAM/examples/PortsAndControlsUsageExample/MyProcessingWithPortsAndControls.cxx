
#include "MyProcessingWithPortsAndControls.hxx"

namespace CLAM
{

MyProcessingWithPortsAndControls::MyProcessingWithPortsAndControls()
	// we specify for each port its name, the processing parent and the number of data token to receive
	// each "Do" (one audio instance, each time).
	: mInput( "Audio Input", this, 1 ), 
	  mOutput( "Audio Output", this, 1 ),
	  // for the control, just the name and the processing
	  mModControl( "Modulation", this )
{
	MyProcessingWithPortsAndControlsConfig cfg;
	Configure( cfg );
}
	
MyProcessingWithPortsAndControls::MyProcessingWithPortsAndControls( const MyProcessingWithPortsAndControlsConfig & cfg )
	: mInput( "Audio Input", this, 1 ),
	  mOutput( "Audio Output", this, 1 ),
	  mModControl( "Modulation", this )
{
	Configure( cfg );
}

MyProcessingWithPortsAndControls::~MyProcessingWithPortsAndControls()
{
}
	
bool MyProcessingWithPortsAndControls::ConcreteConfigure( const ProcessingConfig & cfg )
{	
	CopyAsConcreteConfig(mConfig, cfg);

	return true;
}
	

// this is the do with parameter: just receive a value from control and multiply the input signal with it.
bool MyProcessingWithPortsAndControls::Do(const Audio & in, Audio & out)
{
	if( !AbleToExecute() ) return true;
	
	CLAM::TData value = mModControl.GetLastValue();
	
	for(int i=0; i<in.GetSize(); i++)
	{
		out.GetBuffer()[i] = value * in.GetBuffer()[i];
	}
	return true;
}

// the automatic Do gets the data from the ports and process it. After this, notify the ports that it is finished (with LeaveData method)
bool MyProcessingWithPortsAndControls::Do()
{
	bool res = Do( mInput.GetData(), mOutput.GetData() );
	mInput.LeaveData();
	mOutput.LeaveData();
	return res;
}

} // namespace CLAM
