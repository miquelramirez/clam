#include "MyProcessingWithControlArray.hxx"

namespace CLAM 
{

		//InControlTmplArray(int size, const std::string &name, Processing* parent=0,
		//TPtrMemberFuncId f, const bool publish);
MyProcessingWithControlArray::MyProcessingWithControlArray() :
	_in(12, "my_name", this, CallbackId),
	_in2("my_name", this, Callback),
	_inArray(10, "array_control_name", this, CallbackId),
	_voiceControl(NVoices, "VoiceControl", this, VoiceControlCallbackId),
	_subPOsTransformation( NVoices )
{
	for(int i=0; i<NVoices; i++)
		_subPOsTransformation.at(i) = new TransformationDummy;
}

MyProcessingWithControlArray::~MyProcessingWithControlArray()
{
	for(int i=0; i<NVoices; i++)
		delete _subPOsTransformation.at(i);
}



int MyProcessingWithControlArray::CallbackId(int id, TControlData val) {
	std::cout << "MyProcessingWithControlArray::CallbackId(id: " << id 
		<< " val: " << val << " )" << std::endl;
	return 0; // TODO: depracate result
}

int MyProcessingWithControlArray::VoiceControlCallbackId(int id, TControlData val)
{
	CLAM_DEBUG_ASSERT(id < _subPOsTransformation.size(), "id is larger than array size");
	std::cout << "VoiceControlCallbackId with Id: " << id << std::endl;
	_subPOsTransformation.at(id)->voiceInControl.DoControl(val);
	return 0;
}

}//namespace