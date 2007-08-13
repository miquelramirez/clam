/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "LadspaLoader.hxx"
#include "AudioInPort.hxx"
#include "AudioOutPort.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include "Audio.hxx"
#include "ProcessingFactory.hxx"


namespace CLAM
{

namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, LadspaLoader> regLadspaLoader("LadspaLoader");
}

LadspaPluginExaminer::LadspaPluginExaminer( const std::string & library )
{
	void * sharedObject = dlopen( library.c_str(), RTLD_LAZY );	
	if(!sharedObject) return;
	
	LADSPA_Descriptor_Function function = (LADSPA_Descriptor_Function)dlsym(sharedObject, "ladspa_descriptor");
	if(!function)
		return;
	
	int i = 0;
	const LADSPA_Descriptor * descriptor = function(i);
	
	while(descriptor)
	{
		mDescriptorsList.push_back( std::string(descriptor->Name) );
		i++;
		descriptor = function(i);
	}
}

int LadspaPluginExaminer::GetIndex( const std::string & descriptor )
{
	int i = 0;
	NamesList::iterator it;
	for( it=mDescriptorsList.begin(); it!=mDescriptorsList.end(); it++, i++ )
		if( (*it) == descriptor ) return i;
	CLAM_ASSERT( false, "Wrong ladspa descriptor name" );
	return -1;
}

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
	if(mSharedObject)				
		dlclose(mSharedObject);
	if(mInstance && mDescriptor)
		mDescriptor->cleanup(mInstance);

	RemovePortsAndControls();
}
void LadspaLoader::UpdatePointers()
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

bool LadspaLoader::Do()
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

void LadspaLoader::RemovePortsAndControls()
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

	
bool LadspaLoader::ConcreteConfigure( const ProcessingConfig & cfg)
{
	RemovePortsAndControls();

	CopyAsConcreteConfig(mConfig, cfg);

	mSharedObject = dlopen(mConfig.GetSharedObjectName().c_str(), RTLD_LAZY);
	CLAM_ASSERT( mSharedObject, dlerror() );
	
	LADSPA_Descriptor_Function function = (LADSPA_Descriptor_Function)dlsym(mSharedObject, "ladspa_descriptor");
	CLAM_ASSERT( function, dlerror() );

	mDescriptor = function(mConfig.GetIndex());
	CLAM_ASSERT( mDescriptor, "Couldn't load ladspa descriptor" );

	mInstance = mDescriptor->instantiate(mDescriptor, mConfig.GetSampleRate());
	CLAM_ASSERT( mInstance, "Couldn't create an instance of the plugin" );
	
	ConfigurePortsAndControls();
	
	return true;
}

void LadspaLoader::ConfigurePortsAndControls()
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

} // namespace CLAM

