#include "MyProcessingWithControlArray.hxx"

namespace CLAM 
{

		//InControlTmplArray(int size, const std::string &name, Processing* parent=0,
		//TPtrMemberFuncId f, const bool publish);
MyProcessingWithControlArray::MyProcessingWithControlArray() :
	mIn(12, "my_name", this, CallbackId),
	mIn2("my_name", this, Callback),
	mInArray(10, "array_control_name", this, CallbackId),
	mVoiceControl(NVoices, "VoiceControl", this, VoiceControlCallbackId),
	mSubPOsTransformation( NVoices )
{
	for(int i=0; i<NVoices; i++)
		mSubPOsTransformation.at(i) = new TransformationDummy;
}

MyProcessingWithControlArray::~MyProcessingWithControlArray()
{
	for(int i=0; i<NVoices; i++)
		delete mSubPOsTransformation.at(i);
}



int MyProcessingWithControlArray::CallbackId(int id, TControlData val) {
	std::cout << "MyProcessingWithControlArray::CallbackId(id: " << id 
		<< " val: " << val << " )" << std::endl;
	return 0; // TODO: depracate result
}

int MyProcessingWithControlArray::VoiceControlCallbackId(int id, TControlData val)
{
	CLAM_DEBUG_ASSERT(id < mSubPOsTransformation.size(), "id is larger than array size");
	std::cout << "VoiceControlCallbackId with Id: " << id << std::endl;
	mSubPOsTransformation.at(id)->voiceInControl.DoControl(val);
	return 0;
}

}//namespace