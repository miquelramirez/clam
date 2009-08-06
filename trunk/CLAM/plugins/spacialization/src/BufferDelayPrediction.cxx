/*
 * Copyright (c) 2009 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "BufferDelayPrediction.hxx"
#include <CLAM/ProcessingFactory.hxx>

#include <algorithm>
#include <functional>
#include <cmath>

namespace CLAM
{

namespace
{
	static const char * metadata[] = {
		"key", "BufferDelayPrediction",
		"category", "[plugin] New Spectral Processing",
		"description", "BufferDelayPrediction",
		0
	};
	static FactoryRegistrator<ProcessingFactory, BufferDelayPrediction> reg = metadata;
}

BufferDelayPrediction::BufferDelayPrediction()
{
	Configure( mConfig );
}

void BufferDelayPrediction::CreatePortsAndControls()
{
	unsigned portSize = mConfig.GetBufferSize();
	//std::cout << "portsize=" << portSize << std::endl;
	
	for( int i = 0; i < mConfig.GetNumberOfInPorts(); ++i)
	{
		std::stringstream number("");
		number << i;
		AudioInBuffer* inBuf = new AudioInBuffer( "Input " + number.str(), this );
		inBuf->SetSize( portSize );
		inBuf->SetHop( portSize );
		mInputBufs.push_back( inBuf );
		
		mOutputControls.push_back( new OutControl("Predicted Delay " + number.str(), this) );
		mOutputControls.push_back( new OutControl("Quality " + number.str(), this) );
		
		mInputControls.push_back( new InControl("Mix RMS " + number.str(), this) );
		mInputControls.push_back( new InControl("Uncompressed RMS " + number.str(), this) );
	}
}

void BufferDelayPrediction::RemovePortsAndControls()
{
	std::vector< AudioInBuffer* >::iterator itInBuf;
	for(itInBuf=mInputBufs.begin(); itInBuf!=mInputBufs.end(); itInBuf++)
		delete *itInBuf;
	mInputBufs.clear();

	std::vector< OutControl* >::iterator itOutControl;
	for(itOutControl=mOutputControls.begin(); itOutControl!=mOutputControls.end(); itOutControl++)
		delete *itOutControl;
	mOutputControls.clear();
	
	std::vector< InControl* >::iterator itInControl;
	for(itInControl=mInputControls.begin(); itInControl!=mInputControls.end(); itInControl++)
		delete *itInControl;
	mInputControls.clear();
		
	GetInPorts().Clear();
	GetInControls().Clear();
	GetOutControls().Clear();
}

bool BufferDelayPrediction::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	RemovePortsAndControls();
	CreatePortsAndControls();
	return true;
}

namespace {

} // anonymous

bool BufferDelayPrediction::Do()
{
	unsigned int numInPorts = mConfig.GetNumberOfInPorts();

	for (unsigned int i = 0; i < numInPorts; ++i)
	{
		TControlData mix_rms = mInputControls[i]->GetLastValue();
		TControlData uncompressed_rms = mInputControls[i+1]->GetLastValue();

		TControlData divider = uncompressed_rms * mix_rms;

		TData* first = mInputBufs[i]->GetData().GetBuffer().GetPtr();
		unsigned buffer_size = mInputBufs[i]->GetData().GetSize();
		TData* last = first + buffer_size;

		TData* max_location_ptr = std::max_element(first, last);
	    
		TData max_location = buffer_size - (last - max_location_ptr);

		TData max_value = *max_location_ptr;
		TData max_value_normalized = (max_value / divider) * 100;

		TData predicted_delay = (buffer_size + 1) + 1 - max_location;
		TData predicted_delay_in_seconds = predicted_delay / 48000.0;

		//std::cout << "max_location=" << max_location << " max_value=" << max_value << std::endl; 

		mOutputControls[i]->SendControl(static_cast<TControlData>(predicted_delay_in_seconds));
		mOutputControls[i+1]->SendControl(static_cast<TControlData>(max_value_normalized));
	
	}

	for (unsigned int i = 0; i < numInPorts; ++i)
		mInputBufs[i]->Consume();

	return true;
}

} // namespace CLAM

