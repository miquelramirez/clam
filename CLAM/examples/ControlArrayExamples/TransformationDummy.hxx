#ifndef _TransformationDummy_hxx_
#define _TransformationDummy_hxx_

#include <iostream>

#include "InControl.hxx"
#include "Processing.hxx"

namespace CLAM 
{

class TransformationDummy : public Processing
{
public:
	InControlTmpl<TransformationDummy> voiceInControl;

	TransformationDummy() :
		voiceInControl(
			"voice_in_control", 
			this, 
			&TransformationDummy::StupidCallback	) 
	{}
	
	virtual ~TransformationDummy() {}

	bool Do(){return 0;}
	const ProcessingConfig& GetConfig() const {throw 0;}
	const char* GetClassName() const {return "TransformationDummy";}
	bool ConcreteConfigure(const ProcessingConfig& c) {return true;}

	// this is only because we are pretending that this class is a Processing
	void PublishInControl(InControl* i) {};

private:
	int StupidCallback(TControlData data) {
		std::cout << "\tTransformationDummy::StupidCallback(" << 
			data << ") with this==" << this << std::endl;
		return 0;
	}

};

}; //namespace


#endif

