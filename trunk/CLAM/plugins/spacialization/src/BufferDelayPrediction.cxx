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
#include <iterator>

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
	: mSampleRate(48000.)
	, mBufferNr(0)
{
	Configure( mConfig );
	mSampleRate = mConfig.GetSampleRate();
}

void BufferDelayPrediction::CreatePortsAndControls()
{
	//unsigned bufferSize = mConfig.GetBufferSize();
	//std::cout << "buffersize=" << bufferSize << std::endl;
	
	for( int i = 0; i < mConfig.GetNumberOfInPorts(); ++i)
	{
		std::stringstream number("");
		number << i;
		AudioInBuffer* inBuf = new AudioInBuffer( "Input " + number.str(), this );
		//inBuf->SetSize( bufferSize );
		//inBuf->SetHop( bufferSize );
		mInputBufs.push_back( inBuf );
				
		mInputControls.push_back( new FloatInControl("Mix RMS " + number.str(), this) );
		mInputControls.push_back( new FloatInControl("Uncompressed RMS " + number.str(), this) );
		
		mOutputControls.push_back( new FloatOutControl("Predicted Delay " + number.str(), this) );
		mOutputControls.push_back( new FloatOutControl("Quality " + number.str(), this) );
	}
}

void BufferDelayPrediction::RemovePortsAndControls()
{
	std::vector< AudioInBuffer* >::iterator itInBuf;
	for(itInBuf=mInputBufs.begin(); itInBuf!=mInputBufs.end(); itInBuf++)
		delete *itInBuf;
	mInputBufs.clear();

	std::vector< FloatOutControl* >::iterator itOutControl;
	for(itOutControl=mOutputControls.begin(); itOutControl!=mOutputControls.end(); itOutControl++)
		delete *itOutControl;
	mOutputControls.clear();
	
	std::vector< FloatInControl* >::iterator itInControl;
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

bool BufferDelayPrediction::Do()
{
	unsigned numInPorts = mConfig.GetNumberOfInPorts();

	for (unsigned i = 0; i < numInPorts; ++i)
	{
		unsigned buffer_size = mInputBufs[i]->GetData().GetSize();
		std::vector<TData> out(buffer_size);
		
		TData* first = mInputBufs[i]->GetData().GetBuffer().GetPtr();
		
		std::copy(first, first + buffer_size, out.begin());

		// c= (the fft/complexconjugate buffer produced by the clamnetwork)
		first = &out[0];
		TData* last = first + buffer_size;

		// c/= sqrt (dont need to normalize the whole buffer)
		TControlData mix_rms = mInputControls[i]->GetLastValue();
		TControlData uncompressed_rms = mInputControls[i+1]->GetLastValue();

		TData buffer_sqrt_sum = uncompressed_rms * mix_rms;

		// cmax=
		TData* max_location_ptr = std::max_element(first, last);
		TData max_location = buffer_size - (last - max_location_ptr);
		TData max_value = *max_location_ptr;

		// just divide the one max to get a normalized one 
		TData max_value_normalized = 
			(max_value / (buffer_sqrt_sum == 0 ? 1 : buffer_sqrt_sum)) * 100;
		TData quality = round(max_value_normalized * 10); // why * 10 ??

		// delayPrediction=
		TData predicted_delay = (buffer_size  /*+ 1*/) - max_location;

		if (predicted_delay == buffer_size)
			predicted_delay = 0;

		TData predicted_delay_in_msec = (predicted_delay / mSampleRate);
/*
		std::cout << "max_location=" << max_location 
				  << " max_value=" << max_value 
				  << " max_value_normalized=" << max_value_normalized 
				  << " predicted_delay=" << predicted_delay 
				  << " in_msec=" << predicted_delay_in_msec
				  << " quality=" << quality 
				  << " mix_rms=" << mix_rms 
				  << " uncompressed_rms=" << uncompressed_rms 
				  << " buffer_sqrt_sum=" << buffer_sqrt_sum 
				  << " buffer_size=" << buffer_size
				  << " buffer_nr=" << mBufferNr
				  //<< " max=" << out[1012]
				  << " buffer="
				  << std::endl; 

		//std::copy(first, last, std::ostream_iterator<TData>(std::cout, " "));
		//std::cout << std::endl;
*/	
		if (quality > mConfig.GetQualityThreshold())
		{
			mOutputControls[i]->SendControl(static_cast<TControlData>(predicted_delay));
			mOutputControls[i+1]->SendControl(static_cast<TControlData>(quality));
		}

	}

	for (unsigned i = 0; i < numInPorts; ++i)
		mInputBufs[i]->Consume();

	++mBufferNr;

	return true;
}

} // namespace CLAM

