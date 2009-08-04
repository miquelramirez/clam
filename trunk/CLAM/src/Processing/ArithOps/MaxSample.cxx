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

#include "MaxSample.hxx"
#include "ProcessingFactory.hxx"
#include <algorithm>

namespace CLAM
{

namespace
{
	static const char * metadata[] = {
		"key", "MaxSample",
		"category", "Arithmetic Operations",
		"description", "MaxSample",
		0
	};
	static FactoryRegistrator<ProcessingFactory, MaxSample> reg = metadata;
}

MaxSample::MaxSample()
{
	Configure( mConfig );
}

void MaxSample::CreatePortsAndControls()
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
		
		mOutputControls.push_back( new OutControl("Value " + number.str(), this) );
	}
}

void MaxSample::RemovePortsAndControls()
{
	std::vector< AudioInPort* >::iterator itInPort;
	for(itInPort=mInputPorts.begin(); itInPort!=mInputPorts.end(); itInPort++)
		delete *itInPort;
	mInputPorts.clear();

	std::vector< OutControl* >::iterator itInControl;
	for(itInControl=mOutputControls.begin(); itInControl!=mOutputControls.end(); itInControl++)
		delete *itInControl;
	mOutputControls.clear();
			
	GetInPorts().Clear();
	GetInControls().Clear();
}

bool MaxSample::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	RemovePortsAndControls();
	CreatePortsAndControls();
	return true;
}

bool MaxSample::Do()
{
	unsigned int frameSize = BackendBufferSize(); 
	unsigned int numInPorts = mConfig.GetNumberOfInPorts();

	for (unsigned int i = 0; i<numInPorts; i++)
	{
		TData* first = mInputPorts[i]->GetAudio().GetBuffer().GetPtr();
		TData* last = first + frameSize;

		TData* max_sample = std::max_element(first, last);

		mOutputControls[i]->SendControl((TControlData) *max_sample);
	}

	for (unsigned int inPort=0; inPort<numInPorts; inPort++)
		mInputPorts[inPort]->Consume();

	return true;
}

} // namespace CLAM

