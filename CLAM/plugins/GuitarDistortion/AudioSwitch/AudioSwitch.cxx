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

#include "AudioSwitch.hxx"
#include <CLAM/ProcessingFactory.hxx>


namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "AudioSwitch",
		"category", "Plugins",
		"description", "AudioSwitch",
		0
	};
	static FactoryRegistrator<ProcessingFactory, AudioSwitch> reg = metadata;
}

AudioSwitch::AudioSwitch() : mOutputPort("Output Audio",this), mSwitch("Switch", this)
{
	Configure( mConfig );
}

void AudioSwitch::CreatePortsAndControls()
{
	for( int i=0; i<mConfig.GetNumberOfInPorts(); i++ )
	{
		std::stringstream number("");
		number << i;
		AudioInPort * inPort = new AudioInPort( "Input " + number.str(), this );
		inPort->SetSize( mConfig.GetFrameSize() );
		inPort->SetHop( mConfig.GetFrameSize() );
		mInputPorts.push_back( inPort );
	}

	mSwitch.SetBounds(0,mConfig.GetNumberOfInPorts()-1);
	mSwitch.SetDefaultValue(0);
	mSwitch.DoControl(0);	

	mOutputPort.SetSize( mConfig.GetFrameSize());
	mOutputPort.SetHop( mConfig.GetFrameSize());
}

void AudioSwitch::RemovePortsAndControls()
{
	std::vector< AudioInPort* >::iterator itInPort;
	for(itInPort=mInputPorts.begin(); itInPort!=mInputPorts.end(); itInPort++)
		delete *itInPort;
	mInputPorts.clear();
	
	GetInPorts().Clear();
}

bool AudioSwitch::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	RemovePortsAndControls();
	CreatePortsAndControls();
	return true;
}

bool AudioSwitch::Do()
{
	unsigned int frameSize = mConfig.GetFrameSize();
	unsigned int numInPorts = mConfig.GetNumberOfInPorts();

	TData normConstant = (TData)1.0 /TData(numInPorts);
	TData * output = mOutputPort.GetAudio().GetBuffer().GetPtr();
	TData * inputs[numInPorts];
	TControlData controls[numInPorts];
	for (unsigned int i = 0; i<numInPorts; i++)
	{
		inputs[i]=mInputPorts[i]->GetAudio().GetBuffer().GetPtr();
		controls[i] = 0.;
	}
	unsigned int val = int( mSwitch.GetLastValue() );
	CLAM_ASSERT( val >= 0., "Error: bad index calculation in AudioSwitch class" );
	controls[ val ] = 1.;

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

