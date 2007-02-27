
#include "DummyLadspa.hxx"
//#include <ladspa.h>
#include <CLAM/OSDefines.hxx>
#include <CLAM/CLAM_Math.hxx>
#include <CLAM/Factory.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <ctime>
#include <cstdlib>
#include <dlfcn.h>

#define DEFAULTDELAYFACTOR 10


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<DummyLadspa> regtDummyLadspa( "DummyLadspa" );
	}
	
	
void DummyLadspaConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
	SetName("DummyLadspa");
	SetIndex(0);
	SetSampleRate(44100);
	SetSize(512);
}

DummyLadspa::DummyLadspa()
	: mInstance(0),
	  mDescriptor(0),
	  mSharedObject(0)
{
	DummyLadspaConfig cfg;
	Configure(cfg);
}

DummyLadspa::DummyLadspa( const DummyLadspaConfig & cfg)
	: mInstance(0),
	  mDescriptor(0),
	  mSharedObject(0)
{
	Configure(cfg);
}


bool DummyLadspa::Do()
{
	UpdatePointers();
	
	for(unsigned int i=0;i<mInputControlValues.size();i++)
		mInputControlValues[i]=GetInControls().GetByNumber(i).GetLastValue();

	mDescriptor->run(mInstance, mConfig.GetSize());

	for(unsigned int i=0;i<mOutputControlValues.size();i++)
		GetOutControls().GetByNumber(i).SendControl(mOutputControlValues[i]);

	for(unsigned int i=0;i<mInputPorts.size();i++)
		 mInputPorts[i]->Consume();
	for(unsigned int i=0;i<mOutputPorts.size();i++)
		mOutputPorts[i]->Produce();
	return true;
}

bool DummyLadspa::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	//mSharedObject = dlopen(mConfig.GetSharedObjectName().c_str(), RTLD_LAZY);
	mSharedObject = dlopen("/usr/lib/ladspa/sine.so", RTLD_LAZY);
	std::cout << "loaded ladspa lugin" << std::endl;
	
	LADSPA_Descriptor_Function function = (LADSPA_Descriptor_Function)dlsym(mSharedObject, "ladspa_descriptor");

	mDescriptor = function(mConfig.GetIndex());

	mInstance = mDescriptor->instantiate(mDescriptor, mConfig.GetSampleRate());

	ConfigurePortsAndControls();

	return true;
}

void DummyLadspa::RemovePortsAndControls()
{
	std::vector< AudioInPort* >::iterator itInPort;
	for(itInPort=mInputPorts.begin(); itInPort!=mInputPorts.end(); itInPort++)
		delete *itInPort;
	mInputPorts.clear();

	std::vector< AudioOutPort* >::iterator itOutPort;
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
}

void DummyLadspa::ConfigurePortsAndControls()
{
	for(unsigned int i=0;i<mDescriptor->PortCount;i++)
	{
		// in port
		if(LADSPA_IS_PORT_INPUT(mDescriptor->PortDescriptors[i]) && LADSPA_IS_PORT_AUDIO(mDescriptor->PortDescriptors[i])) 
		{
			AudioInPort * port = new AudioInPort(mDescriptor->PortNames[i],this );
			port->SetSize( mConfig.GetSize());
			mInputPorts.push_back(port);
		}
		// out port
		if(LADSPA_IS_PORT_OUTPUT(mDescriptor->PortDescriptors[i]) && LADSPA_IS_PORT_AUDIO(mDescriptor->PortDescriptors[i])) 
		{
			AudioOutPort * port = new AudioOutPort(mDescriptor->PortNames[i],this );
			port->SetSize( mConfig.GetSize() );
			mOutputPorts.push_back(port);
			
		}

		// in control
		if(LADSPA_IS_PORT_INPUT(mDescriptor->PortDescriptors[i]) && LADSPA_IS_PORT_CONTROL(mDescriptor->PortDescriptors[i])) 
		{
			InControl * control = new InControl(mDescriptor->PortNames[i], this);
			mInputControlValues.push_back(LADSPA_Data());
			mDescriptor->connect_port(mInstance, i, &mInputControlValues[mInputControlValues.size()-1]);
			mInputControls.push_back(control);
		}			
		// out control
		if(LADSPA_IS_PORT_OUTPUT(mDescriptor->PortDescriptors[i]) && LADSPA_IS_PORT_CONTROL(mDescriptor->PortDescriptors[i])) 
		{
			OutControl * control = new OutControl(mDescriptor->PortNames[i], this);
			mOutputControlValues.push_back(LADSPA_Data());
			mDescriptor->connect_port(mInstance, i, &mOutputControlValues[mOutputControlValues.size()-1]);
			mOutputControls.push_back(control);
		}				
	}
}

void DummyLadspa::UpdatePointers()
{
	for(unsigned int i=0;i<mDescriptor->PortCount;i++)
	{
		int inPortIndex = 0;
		int outPortIndex = 0;
		if( LADSPA_IS_PORT_INPUT(mDescriptor->PortDescriptors[i]) && 
		    LADSPA_IS_PORT_AUDIO(mDescriptor->PortDescriptors[i])) // in port
			mDescriptor->connect_port(mInstance, i, mInputPorts[inPortIndex]->GetAudio().GetBuffer().GetPtr());
		else if( LADSPA_IS_PORT_OUTPUT(mDescriptor->PortDescriptors[i]) && 
		    LADSPA_IS_PORT_AUDIO(mDescriptor->PortDescriptors[i])) // in port
			mDescriptor->connect_port(mInstance, i, mOutputPorts[outPortIndex]->GetAudio().GetBuffer().GetPtr());
	}

}

} // namespace CLAM

