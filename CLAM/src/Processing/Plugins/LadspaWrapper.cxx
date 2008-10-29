#ifdef USE_LADSPA

#include "LadspaWrapper.hxx"
//#include <ladspa.h>
#include "OSDefines.hxx"
#include "CLAM_Math.hxx"
#include "Factory.hxx"
#include "AudioInPort.hxx"
#include "AudioOutPort.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include <ctime>
#include <cstdlib>
#include <dlfcn.h>

namespace CLAM
{


LadspaWrapper::LadspaWrapper( const Config & cfg)
	: _instance(0)
	, _descriptor(0)
	, _sharedObject(0)
	, _libraryFileName("")
	, _bufferSize(0)
{
	Configure(cfg);
}

LadspaWrapper::LadspaWrapper( const std::string& libraryFileName, unsigned index, const std::string& key )
	: _instance(0)
	, _descriptor(0)
	, _sharedObject(0)
	, _libraryFileName("")
	, _bufferSize(0)
{
	//std::cout<<"LadspaWrapper()"<<std::endl;
	Config cfg;
	LoadLibraryFunction( libraryFileName, index, key);
	Configure(cfg);
}

LadspaWrapper::~LadspaWrapper()
{
	//std::cout<<"~LadspaWrapper()"<<std::endl;
	if (_instance)
	{
		if (_descriptor->cleanup) _descriptor->cleanup(_instance);
		//std::cout<<"_descriptor->cleanup called"<<std::endl;
	}
	RemovePortsAndControls();
	// if a library function was used, a handle is opened: close it
	if (_sharedObject && (_libraryFileName!=""))
	{
		if (RunTimeLibraryLoader::ReleaseLibraryHandler(_sharedObject,_libraryFileName))
		{
				std::cout<<"[LADSPA] error unloading library handle of: "<<_libraryFileName<<std::endl;
				std::cout<<RunTimeLibraryLoader::LibraryLoadError()<<std::endl;
		}
	}
}

bool LadspaWrapper::ConcreteStart()
{
	if (_descriptor->activate)
		_descriptor->activate(_instance);
	return true;
}
bool LadspaWrapper::ConcreteStop()
{
	if (_descriptor->deactivate)
		_descriptor->deactivate(_instance);
	return true;
}

bool LadspaWrapper::Do()
{
	DoUpdatePortsPointers();
	
	_descriptor->run(_instance, _bufferSize );

	for(unsigned int i=0;i<_outputControlValues.size();i++)
		SendFloatToOutControl(*this, i, _outputControlValues[i]);

	for(unsigned int i=0;i<_inputPorts.size();i++)
		 _inputPorts[i]->Consume();
	for(unsigned int i=0;i<outputPorts.size();i++)
		outputPorts[i]->Produce();
	return true;
}
bool LadspaWrapper::LoadLibraryFunction(const std::string& libraryFileName, unsigned index, const std::string& factoryKey)
{
	//std::cout<<"LadspaWrapper::LoadLibraryFunction("<<libraryFileName<<")"<<std::endl;
	_sharedObject = RunTimeLibraryLoader::LazyLoadLibrary(libraryFileName);
	LADSPA_Descriptor_Function function = (LADSPA_Descriptor_Function)dlsym(_sharedObject, "ladspa_descriptor");
	if(!function)
	{
		std::string error = "[LADSPA] can't open library: " + libraryFileName;
		throw ErrFactory(error.c_str());
	}
	_descriptor = function(index);
	_instance = _descriptor->instantiate(_descriptor, 44100);
	_factoryKey = factoryKey;
	_libraryFileName=libraryFileName;
	return true;
}
bool LadspaWrapper::ConcreteConfigure(const ProcessingConfig&)
{
	_bufferSize = BackendBufferSize();
	ConfigurePortsAndControls();
	ConfigureControlsPointers();
	return true;
}
void LadspaWrapper::RemovePortsAndControls()
{
	std::vector< AudioInPort* >::iterator itInPort;
	for(itInPort=_inputPorts.begin(); itInPort!=_inputPorts.end(); itInPort++)
		delete *itInPort;
	_inputPorts.clear();

	std::vector< AudioOutPort* >::iterator itOutPort;
	for(itOutPort=outputPorts.begin(); itOutPort!=outputPorts.end(); itOutPort++)
		delete *itOutPort;
	outputPorts.clear();

	std::vector< FloatInControl* >::iterator itInControl;
	for(itInControl=_inputControls.begin(); itInControl!=_inputControls.end(); itInControl++)
		delete *itInControl;
	_inputControls.clear();

	std::vector< FloatOutControl* >::iterator itOutControl;
	for(itOutControl=_outputControls.begin(); itOutControl!=_outputControls.end(); itOutControl++)
		delete *itOutControl;
	_outputControls.clear();
	
	_outputControlValues.clear();

	GetInPorts().Clear();
	GetOutPorts().Clear();
	GetInControls().Clear();
	GetOutControls().Clear();
}

void LadspaWrapper::ConfigurePortsAndControls()
{
	RemovePortsAndControls();
	for(unsigned int i=0;i<_descriptor->PortCount;i++)
	{
		const LADSPA_PortDescriptor portDescriptor = _descriptor->PortDescriptors[i];
		// in port
		if(LADSPA_IS_PORT_INPUT(portDescriptor) && LADSPA_IS_PORT_AUDIO(portDescriptor)) 
		{
			AudioInPort * port = new AudioInPort(_descriptor->PortNames[i],this );
			port->SetSize( _bufferSize );
			_inputPorts.push_back(port);
		}
		// out port
		if(LADSPA_IS_PORT_OUTPUT(portDescriptor) && LADSPA_IS_PORT_AUDIO(portDescriptor)) 
		{
			AudioOutPort * port = new AudioOutPort(_descriptor->PortNames[i],this );
			port->SetSize( _bufferSize );
			outputPorts.push_back(port);
		}

		// in control
		if(LADSPA_IS_PORT_INPUT(portDescriptor) && LADSPA_IS_PORT_CONTROL(portDescriptor)) 
		{
			FloatInControl * control = new FloatInControl(_descriptor->PortNames[i], this);

			const LADSPA_PortRangeHint & hint = _descriptor->PortRangeHints[i];
			bool isBounded = (
				LADSPA_IS_HINT_BOUNDED_ABOVE(hint.HintDescriptor) &&
				LADSPA_IS_HINT_BOUNDED_BELOW(hint.HintDescriptor) 
				);
			if (isBounded)
			{
				control->SetBounds( hint.LowerBound, hint.UpperBound ); 
				control->DoControl( control->DefaultValue() );
			}
			_inputControls.push_back(control);
		}			
		// out control
		if (LADSPA_IS_PORT_OUTPUT(portDescriptor) && LADSPA_IS_PORT_CONTROL(portDescriptor)) 
		{
			FloatOutControl * control = new FloatOutControl(_descriptor->PortNames[i], this);
			_outputControlValues.push_back(LADSPA_Data());
			_outputControls.push_back(control);
		}				
	}
}

void LadspaWrapper::ConfigureControlsPointers()
{
	int inControlIndex = 0;
	int outControlIndex = 0;
	for(unsigned int i=0;i<_descriptor->PortCount;i++)
	{
		const LADSPA_PortDescriptor portDescriptor = _descriptor->PortDescriptors[i];
		if (LADSPA_IS_PORT_CONTROL(portDescriptor))
		{
			if (LADSPA_IS_PORT_INPUT(portDescriptor))
			{
				LADSPA_Data* inControlValue = const_cast<LADSPA_Data*>( &(_inputControls[inControlIndex]->GetLastValue()) );
				_descriptor->connect_port(_instance, i, inControlValue);
				inControlIndex++;
			}
			else
				_descriptor->connect_port(_instance, i, & _outputControlValues[outControlIndex++]);
		}
	}

}

void LadspaWrapper::DoUpdatePortsPointers()
{
	int inPortIndex = 0;
	int outPortIndex = 0;
	for(unsigned int i=0;i<_descriptor->PortCount;i++)
	{
		const LADSPA_PortDescriptor portDescriptor = _descriptor->PortDescriptors[i];
		if (!LADSPA_IS_PORT_CONTROL(portDescriptor)) // is audio port
		{
			if (LADSPA_IS_PORT_INPUT(portDescriptor)) 
				_descriptor->connect_port(_instance, i, _inputPorts[inPortIndex++]->GetAudio().GetBuffer().GetPtr());
			else
				_descriptor->connect_port(_instance, i, outputPorts[outPortIndex++]->GetAudio().GetBuffer().GetPtr());
		}
	}

	
}

const char * LadspaWrapper::GetClassName() const
{
	return _factoryKey.c_str();
}

} // namespace CLAM

#endif // USE_LADSPA

