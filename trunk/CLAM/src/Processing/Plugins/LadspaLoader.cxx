
#include "LadspaLoader.hxx"

namespace CLAM
{
	
LadspaLoader::LadspaLoader()
	:mInstance(0),
	mDescriptor(0),
	mSharedObject(0)
{
}
	
LadspaLoader::LadspaLoader( const LadspaLoaderConfig & cfg)
	:mInstance(0),
	mDescriptor(0),
	mSharedObject(0)
{
	Configure(cfg);	
}
	
LadspaLoader::~LadspaLoader()
{
	mDescriptor = 0;
	// TODO:cleanup
	if(mSharedObject)				
		dlclose(mSharedObject);
		if(mInstance && mDescriptor)
		mDescriptor->cleanup(mInstance);
}

bool LadspaLoader::Do()
{
	for(int i=0;i<mInputControlValues.size();i++)
		mInputControlValues[i]=GetInControls().GetByNumber(i).GetLastValue();
	for(int i=0;i<mInputAudio.size();i++)
		mInputAudio[i].GetBuffer() = mInputPorts[i]->GetData().GetBuffer();
		
	mDescriptor->run(mInstance, mConfig.GetSize());

	for(int i=0;i<mOutputControlValues.size();i++)
		GetOutControls().GetByNumber(i).SendControl(mOutputControlValues[i]);

	for(int i=0;i<mOutputAudio.size();i++)
		mOutputPorts[i]->GetData().GetBuffer() = mOutputAudio[i].GetBuffer() ;

	for(int i=0;i<mInputAudio.size();i++)
		 mInputPorts[i]->LeaveData();
	for(int i=0;i<mOutputAudio.size();i++)
		mOutputPorts[i]->LeaveData();
	return true;
}
	
bool LadspaLoader::ConcreteStop()
{
	if(mDescriptor->deactivate)
		mDescriptor->deactivate(mInstance);
	return true;
}
	
bool LadspaLoader::ConcreteStart()		
{
	if(mDescriptor->activate)
		mDescriptor->activate(mInstance);
	return true;
}
	
bool LadspaLoader::ConcreteConfigure( const ProcessingConfig & cfg)
{
	mInputAudio.clear();
	mOutputAudio.clear();
	
	std::vector< InPortTmpl<Audio>* >::iterator itInPort;
	for(itInPort=mInputPorts.begin(); itInPort!=mInputPorts.end(); itInPort++)
		delete *itInPort;
	mInputPorts.clear();

	std::vector< OutPortTmpl<Audio>* >::iterator itOutPort;
	for(itOutPort=mOutputPorts.begin(); itOutPort!=mOutputPorts.end(); itOutPort++)
		delete *itOutPort;
	mOutputPorts.clear();

	std::vector< InControl* >::iterator itInControl;
	for(itInControl=mInputControls.begin(); itInControl!=mInputControls.end(); itInControl++)
		delete *itInControl;
	mInputControls.clear();

	std::vector< OutControl* >::iterator itOutControl;
	for(itOutControl=mOutputControls.begin(); itOutControl!=mOutputControls.end(); itOutControl++)
		delete *itOutControl;
	mOutputControls.clear();
	
	mInputControlValues.clear();
	mOutputControlValues.clear();

	GetInPorts().Clear();
	GetOutPorts().Clear();
	GetInControls().Clear();
	GetOutControls().Clear();
	CopyAsConcreteConfig(mConfig, cfg);

	mSharedObject = dlopen(mConfig.GetSharedObjectName().c_str(), RTLD_LAZY);
	
	if(!mSharedObject)
		throw Err(dlerror());

	LADSPA_Descriptor_Function function = (LADSPA_Descriptor_Function)dlsym(mSharedObject, "ladspa_descriptor");
	if(!function)
		throw Err(dlerror());
		
	mDescriptor = function(mConfig.GetIndex());
	if(!mDescriptor)
		throw Err("Couldn't load ladspa descriptor");

	mInstance = mDescriptor->instantiate(mDescriptor, mConfig.GetSampleRate());
	if(!mInstance)
		throw Err("Couldn't create an instance of the plugin");
		
	ConfigurePortsAndControls();
		return true;
}

void LadspaLoader::ConfigurePortsAndControls()
{
	for(unsigned int i=0;i<mDescriptor->PortCount;i++)
	{
		if(LADSPA_IS_PORT_INPUT(mDescriptor->PortDescriptors[i]) && LADSPA_IS_PORT_AUDIO(mDescriptor->PortDescriptors[i])) // in port
		{
			InPortTmpl<Audio> * port = new InPortTmpl<Audio>(mDescriptor->PortNames[i],this, mConfig.GetSize());
			Audio audio;
			audio.SetSize(mConfig.GetSize());
			audio.SetSampleRate(mConfig.GetSampleRate());
			mInputAudio.push_back(audio);
			mDescriptor->connect_port(mInstance, i, mInputAudio[mInputAudio.size()-1].GetBuffer().GetPtr());
			mInputPorts.push_back(port);
		}
		if(LADSPA_IS_PORT_INPUT(mDescriptor->PortDescriptors[i]) && LADSPA_IS_PORT_CONTROL(mDescriptor->PortDescriptors[i])) // in control
		{
			InControl * control = new InControl(mDescriptor->PortNames[i], this);
			mInputControlValues.push_back(LADSPA_Data());
			mDescriptor->connect_port(mInstance, i, &mInputControlValues[mInputControlValues.size()-1]);
			mInputControls.push_back(control);
		}			
		if(LADSPA_IS_PORT_OUTPUT(mDescriptor->PortDescriptors[i]) && LADSPA_IS_PORT_AUDIO(mDescriptor->PortDescriptors[i])) // out port
		{
			OutPortTmpl<Audio> * port = new OutPortTmpl<Audio>(mDescriptor->PortNames[i],this, mConfig.GetSize());
			Audio audio;
			audio.SetSize(mConfig.GetSize());
			audio.SetSampleRate(mConfig.GetSampleRate());
			mOutputAudio.push_back(audio);
			mDescriptor->connect_port(mInstance, i, mOutputAudio[mOutputAudio.size()-1].GetBuffer().GetPtr());
			mOutputPorts.push_back(port);
			
		}
		if(LADSPA_IS_PORT_OUTPUT(mDescriptor->PortDescriptors[i]) && LADSPA_IS_PORT_CONTROL(mDescriptor->PortDescriptors[i])) // in control
		{
			OutControl * control = new OutControl(mDescriptor->PortNames[i], this);
			mOutputControlValues.push_back(LADSPA_Data());
			mDescriptor->connect_port(mInstance, i, &mOutputControlValues[mOutputControlValues.size()-1]);
			mOutputControls.push_back(control);
		}				
	}
}

} // namespace CLAM

