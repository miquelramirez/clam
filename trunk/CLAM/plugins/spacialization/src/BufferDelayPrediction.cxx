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
#include <cmath>
#include <functional>

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
	unsigned portSize = BackendBufferSize();
	
	for( int i = 0; i < mConfig.GetNumberOfInPorts(); ++i)
	{
		std::stringstream number("");
		number << i;
		AudioInBuffer* inBuf = new AudioInBuffer( "Input " + number.str(), this );
		inBuf->SetSize( portSize );
		inBuf->SetHop( portSize );
		mInputBufs.push_back( inBuf );
		
		mOutputControls.push_back( new OutControl("Value " + number.str(), this) );
		
		mInputControls.push_back( new InControl("Mix " + number.str(), this) );
		mInputControls.push_back( new InControl("Uncompressed " + number.str(), this) );
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

template<typename T> struct divide {
	T const& t_;
	divide(T const& t) : t_(t) {}
    T const operator()(T const x) { return x / (t_ == 0 ? 1 : t_); }
};

} // anonymous

bool BufferDelayPrediction::Do()
{
	unsigned int numInPorts = mConfig.GetNumberOfInPorts();

	for (unsigned int i = 0; i < numInPorts; ++i)
	{
		TControlData mix = mInputControls[i]->GetLastValue();
		TControlData uncompressed = mInputControls[i+1]->GetLastValue();

		TControlData divider = std::sqrt(uncompressed * mix);

		TData* first = mInputBufs[i]->GetData().GetBuffer().GetPtr();
		TData* last = first + mInputBufs[i]->GetData().GetSize();

		// in place transform
		std::transform(first, last, first, divide<TData>(divider));

		TData* max = std::max_element(first, last);

		mOutputControls[i]->SendControl(static_cast<TControlData>(*max));
	}

	for (unsigned int i = 0; i < numInPorts; ++i)
		mInputBufs[i]->Consume();

	return true;
}

} // namespace CLAM

