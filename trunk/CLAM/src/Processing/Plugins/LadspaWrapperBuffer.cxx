#ifdef USE_LADSPA

#include "LadspaWrapperBuffer.hxx"
//#include <ladspa.h>
#include "OSDefines.hxx"
#include "CLAM_Math.hxx"
#include "Factory.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include "Audio.hxx"
#include <ctime>
#include <cstdlib>

namespace CLAM
{


LadspaWrapperBuffer::LadspaWrapperBuffer( const Config & cfg)
	: _instance(0)
	, _descriptor(0)
	, _sharedObject(0)
	, _libraryFileName("")
	, _bufferSize(0)
{
	Configure(cfg);
}

LadspaWrapperBuffer::LadspaWrapperBuffer( const std::string& libraryFileName, unsigned index, const std::string& key )
	: _instance(0)
	, _descriptor(0)
	, _sharedObject(0)
	, _libraryFileName("")
	, _bufferSize(0)
{
	//std::cout<<"LadspaWrapperBuffer()"<<std::endl;
	Config cfg;
	LoadLibraryFunction( libraryFileName, index, key);
	Configure(cfg);
}

LadspaWrapperBuffer::~LadspaWrapperBuffer()
{
	//std::cout<<"~LadspaWrapperBuffer()"<<std::endl;
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

bool LadspaWrapperBuffer::ConcreteStart()
{
	if (_descriptor->activate)
		_descriptor->activate(_instance);
	return true;
}
bool LadspaWrapperBuffer::ConcreteStop()
{
	if (_descriptor->deactivate)
		_descriptor->deactivate(_instance);
	return true;
}

bool LadspaWrapperBuffer::Do()
{
	_bufferSize = _inputPorts[0]->GetData().GetSize();

    bool eachInputPortUsingSameSize=true;
    for(unsigned int i=1;i<_inputPorts.size()&&eachInputPortUsingSameSize;i++)
	    eachInputPortUsingSameSize = eachInputPortUsingSameSize && (_inputPorts[i-1]->GetData().GetSize() == _inputPorts[i]->GetData().GetSize());

    CLAM_ASSERT(eachInputPortUsingSameSize, "Some InputPorts have not the same buffer size.");

    for(unsigned int i=0;i<_outputPorts.size();i++)
	    _outputPorts[i]->GetData().SetSize(_bufferSize);

	DoUpdatePortsPointers();
    _descriptor->run(_instance, _bufferSize);

    for(unsigned int i=0;i<_outputControlValues.size();i++)
		SendFloatToOutControl(*this, i, _outputControlValues[i]);

    for(unsigned int i=0;i<_inputPorts.size();i++)
	{
		 _inputPorts[i]->Consume();
	}
	for(unsigned int i=0;i<_outputPorts.size();i++)
	{
	    _outputPorts[i]->Produce();
	}
	return true;
}
bool LadspaWrapperBuffer::LoadLibraryFunction(const std::string& libraryFileName, unsigned index, const std::string& factoryKey)
{
	//std::cout<<"LadspaWrapperBuffer::LoadLibraryFunction("<<libraryFileName<<")"<<std::endl;
	_sharedObject = RunTimeLibraryLoader::LazyLoadLibrary(libraryFileName);
	LADSPA_Descriptor_Function function = (LADSPA_Descriptor_Function)RunTimeLibraryLoader::GetSymbol(_sharedObject, "ladspa_descriptor");
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
bool LadspaWrapperBuffer::ConcreteConfigure(const ProcessingConfig&)
{
	_bufferSize = BackendBufferSize();
	ConfigurePortsAndControls();
	ConfigureControlsPointers();
	return true;
}
void LadspaWrapperBuffer::RemovePortsAndControls()
{
	std::vector< InPort<Audio>* >::iterator itInPort;
	for(itInPort=_inputPorts.begin(); itInPort!=_inputPorts.end(); itInPort++)
		delete *itInPort;
	_inputPorts.clear();

	std::vector< OutPort<Audio>* >::iterator itOutPort;
	for(itOutPort=_outputPorts.begin(); itOutPort!=_outputPorts.end(); itOutPort++)
		delete *itOutPort;
	_outputPorts.clear();

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

void LadspaWrapperBuffer::ConfigurePortsAndControls()
{
	RemovePortsAndControls();
	for(unsigned int i=0;i<_descriptor->PortCount;i++)
	{
		const LADSPA_PortDescriptor portDescriptor = _descriptor->PortDescriptors[i];
		// in port
		if(LADSPA_IS_PORT_INPUT(portDescriptor) && LADSPA_IS_PORT_AUDIO(portDescriptor))
		{
			InPort<Audio> * port = new InPort<Audio>(_descriptor->PortNames[i],this );
			port->SetSize( 1 );
			port->SetHop( 1 );
			_inputPorts.push_back(port);
		}
		// out port
		if(LADSPA_IS_PORT_OUTPUT(portDescriptor) && LADSPA_IS_PORT_AUDIO(portDescriptor))
		{
			OutPort<Audio> * port = new OutPort<Audio>(_descriptor->PortNames[i],this );
			port->SetSize( 1 );
			port->SetHop( 1 );
			_outputPorts.push_back(port);
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

void LadspaWrapperBuffer::ConfigureControlsPointers()
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

void LadspaWrapperBuffer::DoUpdatePortsPointers()
{
	int inPortIndex = 0;
	int outPortIndex = 0;
	for(unsigned int i=0;i<_descriptor->PortCount;i++)
	{
		const LADSPA_PortDescriptor portDescriptor = _descriptor->PortDescriptors[i];
		if (!LADSPA_IS_PORT_CONTROL(portDescriptor)) // is audio port
		{
			if (LADSPA_IS_PORT_INPUT(portDescriptor))
				_descriptor->connect_port(_instance, i, _inputPorts[inPortIndex++]->GetData().GetBuffer().GetPtr());
			else
				_descriptor->connect_port(_instance, i, _outputPorts[outPortIndex++]->GetData().GetBuffer().GetPtr());
		}
	}
}

const char * LadspaWrapperBuffer::GetClassName() const
{
	return _factoryKey.c_str();
}

} // namespace CLAM

#endif // USE_LADSPA

