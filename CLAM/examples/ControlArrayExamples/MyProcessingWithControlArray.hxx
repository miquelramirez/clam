#ifndef _MyProcessingWithControlArray_hxx_
#define _MyProcessingWithControlArray_hxx_

#include <vector>
#include "TransformationDummy.hxx"
#include "InControlTmplArray.hxx"

namespace CLAM
{

class MyProcessingWithControlArray
{
private:
	//typedef InControlArray ControlsWithoutCallback;

	typedef std::vector< TransformationDummy* > Transformations;
	typedef MyProcessingWithControlArray ThisProc;

public:
	enum {NVoices = 4};
	Transformations mSubPOsTransformation;
	InControlTmpl<ThisProc> mIn;
	InControlTmpl<ThisProc> mIn2;

	InControlTmplArray<ThisProc> mInArray;

	InControlTmplArray<ThisProc> mVoiceControl;

	
	MyProcessingWithControlArray();
	~MyProcessingWithControlArray();
	int CallbackId(int id, TControlData val );
	int Callback(TControlData val ) { 
		std::cout << "Callback( "<<val<<std::endl; 
		return 0;
	};

	int VoiceControlCallbackId(int id, TControlData val);

	/**  this is only because this class is impersonating a Processing*/
	void PublishInControl(InControl* i) {};
};




} //namespace CLAM
#endif
