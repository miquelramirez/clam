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

namespace CLAM
{

AudioMixer::AudioMixer() : mOutputPort("Output Audio",this)
{
	AudioMixerConfig cfg;
	Configure( cfg );
}


void AudioMixer::CreatePortsAndControls()
{
	for( int i=0; i<mConfig.GetNumberOfInPorts(); i++ )
	{
		std::stringstream number("");
		number << i;
		AudioInPort * inPort = new AudioInPort( "Input " + number.str(), this );
		inPort->SetSize( mConfig.GetFrameSize() );
		inPort->SetHop( mConfig.GetFrameSize() );
		mInputPorts.push_back( inPort );
		
		mInputControls.push_back( new InControl("Gain " + number.str(), this) );
	}
	for( int i=0; i<mConfig.GetNumberOfInPorts(); i++ )
	{
		/* Set gain = 1 by default */
		mInputControls[i]->DoControl(1.);
	}
	
	mOutputPort.SetSize( mConfig.GetFrameSize());
	mOutputPort.SetHop( mConfig.GetFrameSize());
}

void AudioMixer::RemovePortsAndControls()
{
	std::vector< AudioInPort* >::iterator itInPort;
	for(itInPort=mInputPorts.begin(); itInPort!=mInputPorts.end(); itInPort++)
		delete *itInPort;
	mInputPorts.clear();

	std::vector< InControl* >::iterator itInControl;
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
	unsigned int frameSize = mConfig.GetFrameSize();
	unsigned int numInPorts = mConfig.GetNumberOfInPorts();

	TData normConstant = (TData)1.0 /TData(numInPorts);
	
	for (unsigned int sample=0; sample < frameSize; sample++) 
	{
		TData sum=0.0;
		
		for (unsigned int inPort=0; inPort< numInPorts; inPort++)
		{
			TData valueModified = mInputPorts[inPort]->GetData(sample) * mInputControls[inPort]->GetLastValue();
			sum += valueModified;
		}
		mOutputPort.GetData(sample) = sum * normConstant;
	}

	// execute consume/produce methods	
	for (unsigned int inPort=0; inPort<numInPorts; inPort++)
		mInputPorts[inPort]->Consume();
	mOutputPort.Produce();
	
	return true;
}

} // namespace CLAM

