
#ifndef __MyProcessingWithPortsAndControls_hxx__
#define __MyProcessingWithPortsAndControls_hxx__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "OutPortTmpl.hxx"
#include "InPortTmpl.hxx"
#include "InControl.hxx"
#include "Audio.hxx"
#include <string>

namespace CLAM
{

// we don't need great configurations, just the name.
class MyProcessingWithPortsAndControlsConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (MyProcessingWithPortsAndControlsConfig, 0, ProcessingConfig);
};

class MyProcessingWithPortsAndControls : public Processing
{
private:
	MyProcessingWithPortsAndControlsConfig mConfig;

	// This processing will have an input control to get the value of modulation
	InControl mModControl;
	// Two ports: one for the input data and one for the output after the processing is done. Note that
	// the instances are templatized by the type of data they receive.
	InPortTmpl<Audio> mInput;
	OutPortTmpl<Audio> mOutput;

public:
	MyProcessingWithPortsAndControls();
	MyProcessingWithPortsAndControls( const MyProcessingWithPortsAndControlsConfig & cfg );		
	virtual ~MyProcessingWithPortsAndControls();
	const char * GetClassName() const {return "MyProcessingWithPortsAndControls";}
	inline const ProcessingConfig &GetConfig() const { return mConfig;}	
	
	bool ConcreteConfigure( const ProcessingConfig & cfg );
	

	bool Do(const Audio & in, Audio & out);
	bool Do();

};

} // namespace CLAM

#endif // __MyProcessingWithPorts_hxx__
