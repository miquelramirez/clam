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

#include "AudioMixer.hxx"
#include "ProcessingFactory.hxx"


namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "AudioMixer",
		"category", "Arithmetic Operations",
		"description", "AudioMixer",
		0
	};
	static FactoryRegistrator<ProcessingFactory, AudioMixer> reg = metadata;
}

AudioMixer::AudioMixer()
	: mOutputPort("Output Audio",this)
{
	Configure( mConfig );
}

void AudioMixer::CreatePortsAndControls()
{
	unsigned portSize = BackendBufferSize();
	
	for( int i=0; i<mConfig.GetNumberOfInPorts(); i++ )
	{
		std::stringstream number("");
		number << i;
		AudioInPort * inPort = new AudioInPort( "Input " + number.str(), this );
		inPort->SetSize( portSize );
		inPort->SetHop( portSize );
		mInputPorts.push_back( inPort );
		
		mInputControls.push_back( new FloatInControl("Gain " + number.str(), this) );
	}
	unsigned int inPortsNumber=mConfig.GetNumberOfInPorts();
	CLAM::Array<TControlData> gainsArray;
	bool useConfigGains = mConfig.HasDefaultGains();
	if (useConfigGains)
	{
		gainsArray=mConfig.GetDefaultGains();
		unsigned numberofConfiguredGains=gainsArray.Size();
		gainsArray.Resize(inPortsNumber);
		gainsArray.SetSize(inPortsNumber);
		for (unsigned i=numberofConfiguredGains;i<gainsArray.Size();i++)
		{
			gainsArray[i]=1;
		}
		mConfig.SetDefaultGains(gainsArray);
	}
	for( unsigned int i=0; i<inPortsNumber; i++ )
	{
		if (useConfigGains) 
			mInputControls[i]->DoControl(gainsArray[i]);
		else
			/* Set gain = 1 by default */
			mInputControls[i]->DoControl(1.);
	}
	
	mOutputPort.SetSize( portSize );
	mOutputPort.SetHop( portSize );
}

void AudioMixer::RemovePortsAndControls()
{
	std::vector< AudioInPort* >::iterator itInPort;
	for(itInPort=mInputPorts.begin(); itInPort!=mInputPorts.end(); itInPort++)
		delete *itInPort;
	mInputPorts.clear();

	std::vector< FloatInControl* >::iterator itInControl;
	for(itInControl=mInputControls.begin(); itInControl!=mInputControls.end(); itInControl++)
		delete *itInControl;
	mInputControls.clear();
			
	GetInPorts().Clear();
	GetInControls().Clear();
}

bool AudioMixer::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	RemovePortsAndControls();
	CreatePortsAndControls();
	return true;
}

bool AudioMixer::Do()
{

	unsigned int frameSize = BackendBufferSize(); 
	unsigned int numInPorts = mConfig.GetNumberOfInPorts();

	TData normConstant = (TData)1.0 /TData(numInPorts);
	TData * output = mOutputPort.GetAudio().GetBuffer().GetPtr();
	TData * inputs[numInPorts];
	TControlData controls[numInPorts];
	for (unsigned int i = 0; i<numInPorts; i++)
	{
		inputs[i]=mInputPorts[i]->GetAudio().GetBuffer().GetPtr();
		controls[i]=mInputControls[i]->GetLastValue();
	}

	for (unsigned int sample=0; sample < frameSize; sample++) 
	{
		TData sum=0.0;
		for (unsigned int inPort=0; inPort< numInPorts; inPort++)
		{
			sum += inputs[inPort][sample] * controls[inPort];
		}
		output[sample] = sum * normConstant;
	}

	// execute consume/produce methods	
	for (unsigned int inPort=0; inPort<numInPorts; inPort++)
		mInputPorts[inPort]->Consume();
	mOutputPort.Produce();
	
	return true;
}

} // namespace CLAM

