#ifndef _MyProcessingWithControlArray_hxx_
#define _MyProcessingWithControlArray_hxx_

#include <vector>
#include "TransformationDummy.hxx"
#include "InControlTmplArray.hxx"
#include "Processing.hxx"

namespace CLAM
{

class MyProcessingWithControlArray : public Processing
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

	bool Do(){return 0;}
	const ProcessingConfig& GetConfig() const {throw 0;}
	const char* GetClassName() const {return "MyProcessingWithControlArray";}
	bool ConcreteConfigure(const ProcessingConfig& c) {return true;}

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
