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

namespace CLAM
{

static int sPortSize = 512;

LadspaWrapper::LadspaWrapper()
	: mInstance(0),
	  mDescriptor(0),
	  mSharedObject(0)
{
	Config cfg;
	Configure(cfg);
}

LadspaWrapper::LadspaWrapper( const Config & cfg = Config())
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
	Config cfg;
	ConcreteConfigure( libraryFileName, index, key);
	Configure(cfg);
}

bool LadspaWrapper::Do()
{
	UpdatePortsPointers();
	
	mDescriptor->run(mInstance, sPortSize );

	for(unsigned int i=0;i<mOutputControlValues.size();i++)
		GetOutControls().GetByNumber(i).SendControl(mOutputControlValues[i]);

	for(unsigned int i=0;i<mInputPorts.size();i++)
		 mInputPorts[i]->Consume();
	for(unsigned int i=0;i<mOutputPorts.size();i++)
		mOutputPorts[i]->Produce();
	return true;
}
bool LadspaWrapper::ConcreteConfigure(const std::string& libraryFileName, unsigned index, const std::string& factoryKey)
{
	mSharedObject = dlopen(libraryFileName.c_str(), RTLD_LAZY);
	LADSPA_Descriptor_Function function = (LADSPA_Descriptor_Function)dlsym(mSharedObject, "ladspa_descriptor");
	if(!function)
	{
		std::string error = "[LADSPA] can't open library: " + libraryFileName;
		throw ErrFactory(error.c_str());
	}
	mDescriptor = function(index);
	mInstance = mDescriptor->instantiate(mDescriptor, 44100);
	mFactoryKey = factoryKey;

	ConfigurePortsAndControls();
	UpdateControlsPointers();

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
			port->SetSize( sPortSize );
			mInputPorts.push_back(port);
		}
		// out port
		if(LADSPA_IS_PORT_OUTPUT(portDescriptor) && LADSPA_IS_PORT_AUDIO(portDescriptor)) 
		{
			AudioOutPort * port = new AudioOutPort(mDescriptor->PortNames[i],this );
			port->SetSize( sPortSize );
			mOutputPorts.push_back(port);
		}

		// in control
		if(LADSPA_IS_PORT_INPUT(portDescriptor) && LADSPA_IS_PORT_CONTROL(portDescriptor)) 
		{
			InControl * control = new InControl(mDescriptor->PortNames[i], this);

			const LADSPA_PortRangeHint & hint = mDescriptor->PortRangeHints[i];
			bool isBounded = (
				LADSPA_IS_HINT_BOUNDED_ABOVE(hint.HintDescriptor) &&
				LADSPA_IS_HINT_BOUNDED_BELOW(hint.HintDescriptor) 
				);
			if (isBounded)
			{
				control->SetBounds( hint.LowerBound, hint.UpperBound ); 
				control->DoControl( control->DefaultValue() );
//				std::cout << mDescriptor->PortNames[i] << " is bounded "<<std::endl;
//				std::cout << "(" << control->LowerBound()<<", "<< control->UpperBound() << ")" << std::endl;
			}
			mInputControls.push_back(control);
		}			
		// out control
		if (LADSPA_IS_PORT_OUTPUT(portDescriptor) && LADSPA_IS_PORT_CONTROL(portDescriptor)) 
		{
			OutControl * control = new OutControl(mDescriptor->PortNames[i], this);
			mOutputControlValues.push_back(LADSPA_Data());
			mOutputControls.push_back(control);
		}				
	}
}

void LadspaWrapper::UpdateControlsPointers()
{
	int inControlIndex = 0;
	int outControlIndex = 0;
	for(unsigned int i=0;i<mDescriptor->PortCount;i++)
	{
		const LADSPA_PortDescriptor portDescriptor = mDescriptor->PortDescriptors[i];
		if (LADSPA_IS_PORT_CONTROL(portDescriptor))
		{
			if (LADSPA_IS_PORT_INPUT(portDescriptor))
			{
				LADSPA_Data* inControlValue = const_cast<LADSPA_Data*>( &(mInputControls[inControlIndex]->GetLastValue()) );
				mDescriptor->connect_port(mInstance, i, inControlValue);
				inControlIndex++;
			}
			else
				mDescriptor->connect_port(mInstance, i, & mOutputControlValues[outControlIndex++]);
		}
	}

}

void LadspaWrapper::UpdatePortsPointers()
{
	int inPortIndex = 0;
	int outPortIndex = 0;
	for(unsigned int i=0;i<mDescriptor->PortCount;i++)
	{
		const LADSPA_PortDescriptor portDescriptor = mDescriptor->PortDescriptors[i];
		if (!LADSPA_IS_PORT_CONTROL(portDescriptor)) // is audio port
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
	return mFactoryKey.c_str();
}

} // namespace CLAM

#endif // USE_LADSPA

