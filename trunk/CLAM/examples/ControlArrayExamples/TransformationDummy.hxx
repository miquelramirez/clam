#ifndef _TransformationDummy_hxx_
#define _TransformationDummy_hxx_

#include "InControl.hxx"
#include <iostream>

namespace CLAM 
{

class TransformationDummy
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