#ifdef USE_LADSPA

#include "LadspaWrapper.hxx"
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


namespace CLAM
{
	
void LadspaWrapperConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
	SetName("LadspaWrapper");
	SetSampleRate(44100);
	SetSize(512);
	SetLibraryFileName("");
	SetIndex(0);
	SetFactoryKey("");
}

LadspaWrapper::LadspaWrapper()
	: mInstance(0),
	  mDescriptor(0),
	  mSharedObject(0)
{
	LadspaWrapperConfig cfg;
	Configure(cfg);
}

LadspaWrapper::LadspaWrapper( const LadspaWrapperConfig & cfg )
	: mInstance(0),
	  mDescriptor(0),
	  mSharedObject(0)
{
	Configure(cfg);
}

LadspaWrapper::LadspaWrapper( const std::string& libraryFileName, unsigned index, const std::string& key )
	: mInstance(0),
	  mDescriptor(0),
	  mSharedObject(0)
{
	LadspaWrapperConfig cfg;
	cfg.SetLibraryFileName(libraryFileName);
	cfg.SetIndex(index);
	cfg.SetFactoryKey(key);
	Configure(cfg);
}

bool LadspaWrapper::Do()
{
	UpdatePointers();
	
//	for(unsigned int i=0;i<mInputControlValues.size();i++)
//		mInputControlValues[i]=1;//GetInControls().GetByNumber(i).GetLastValue();
		

	mDescriptor->run(mInstance, mConfig.GetSize());

	for(unsigned int i=0;i<mOutputControlValues.size();i++)
		GetOutControls().GetByNumber(i).SendControl(mOutputControlValues[i]);

	for(unsigned int i=0;i<mInputPorts.size();i++)
		 mInputPorts[i]->Consume();
	for(unsigned int i=0;i<mOutputPorts.size();i++)
		mOutputPorts[i]->Produce();
	return true;
}

bool LadspaWrapper::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	mSharedObject = dlopen(mConfig.GetLibraryFileName().c_str(), RTLD_LAZY);
	
	LADSPA_Descriptor_Function function = (LADSPA_Descriptor_Function)dlsym(mSharedObject, "ladspa_descriptor");

	mDescriptor = function(mConfig.GetIndex());

	mInstance = mDescriptor->instantiate(mDescriptor, mConfig.GetSampleRate());

	ConfigurePortsAndControls();

	return true;
}

void LadspaWrapper::RemovePortsAndControls()
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

void LadspaWrapper::ConfigurePortsAndControls()
{
	RemovePortsAndControls();
	for(unsigned int i=0;i<mDescriptor->PortCount;i++)
	{
		const LADSPA_PortDescriptor portDescriptor = mDescriptor->PortDescriptors[i];
		// in port
		if(LADSPA_IS_PORT_INPUT(portDescriptor) && LADSPA_IS_PORT_AUDIO(portDescriptor)) 
		{
			AudioInPort * port = new AudioInPort(mDescriptor->PortNames[i],this );
			port->SetSize( mConfig.GetSize());
			mInputPorts.push_back(port);
		}
		// out port
		if(LADSPA_IS_PORT_OUTPUT(portDescriptor) && LADSPA_IS_PORT_AUDIO(portDescriptor)) 
		{
			AudioOutPort * port = new AudioOutPort(mDescriptor->PortNames[i],this );
			port->SetSize( mConfig.GetSize() );
			mOutputPorts.push_back(port);
		}

		// in control
		if(LADSPA_IS_PORT_INPUT(portDescriptor) && LADSPA_IS_PORT_CONTROL(portDescriptor)) 
		{
			InControl * control = new InControl(mDescriptor->PortNames[i], this);
			mInputControlValues.push_back(LADSPA_Data());
			mInputControls.push_back(control);
		}			
		// out control
		if(LADSPA_IS_PORT_OUTPUT(portDescriptor) && LADSPA_IS_PORT_CONTROL(portDescriptor)) 
		{
			OutControl * control = new OutControl(mDescriptor->PortNames[i], this);
			mOutputControlValues.push_back(LADSPA_Data());
			mOutputControls.push_back(control);
		}				
	}
}

LADSPA_Data LadspaWrapper::DefautLadspaInControlValue( unsigned portId )
{
	const LADSPA_PortRangeHint & hint = mDescriptor->PortRangeHints[portId];
	if ( !LADSPA_IS_HINT_BOUNDED_ABOVE(hint.HintDescriptor) ||
		!LADSPA_IS_HINT_BOUNDED_BELOW(hint.HintDescriptor) )
	{
		std::cout << "Warning: Ladspa in-control "<< mDescriptor->PortNames[portId] << " is not bouded" << std::endl;
		return 1;
	}
	std::cout << mDescriptor->PortNames[portId]<< std::endl;
	std::cout << "up bound: " << hint.UpperBound << std::endl;
	std::cout << "low bound: " << hint.LowerBound << std::endl;
	return (hint.UpperBound + hint.LowerBound) / 2.;
}

void LadspaWrapper::UpdatePointers()
{
	int inPortIndex = 0;
	int outPortIndex = 0;
	int inControlIndex = 0;
	int outControlIndex = 0;
	for(unsigned int i=0;i<mDescriptor->PortCount;i++)
	{
		const LADSPA_PortDescriptor portDescriptor = mDescriptor->PortDescriptors[i];
		if (LADSPA_IS_PORT_CONTROL(portDescriptor))
		{
			if (LADSPA_IS_PORT_INPUT(portDescriptor))
			{
				mDescriptor->connect_port(mInstance, i, & mInputControlValues[inControlIndex]);
				mInputControlValues[inControlIndex] = DefautLadspaInControlValue(i);
				inControlIndex++;
			}
			else
				mDescriptor->connect_port(mInstance, i, & mOutputControlValues[outControlIndex++]);
		}
		else // AUDIO
		{
			if (LADSPA_IS_PORT_INPUT(portDescriptor)) 
				mDescriptor->connect_port(mInstance, i, mInputPorts[inPortIndex++]->GetAudio().GetBuffer().GetPtr());
			else
				mDescriptor->connect_port(mInstance, i, mOutputPorts[outPortIndex++]->GetAudio().GetBuffer().GetPtr());
		}
	}

}
const char * LadspaWrapper::GetClassName() const
{
	return mConfig.GetFactoryKey().c_str();
}

} // namespace CLAM

#endif // USE_LADSPA

