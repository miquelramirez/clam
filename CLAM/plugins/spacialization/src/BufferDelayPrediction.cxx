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
				
		mInputControls.push_back( new InControl("Mix RMS " + number.str(), this) );
		mInputControls.push_back( new InControl("Uncompressed RMS " + number.str(), this) );
		
		mOutputControls.push_back( new OutControl("Predicted Delay " + number.str(), this) );
		mOutputControls.push_back( new OutControl("Quality " + number.str(), this) );
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

// std::transform operates on 2 ranges, not a range and a value
void normalize(TData* first, TData* last, TData div)
{
	for (; first != last; ++first)
		*first = *first / (div == 0 ? 1 : div);
}

// if we need the above generalized
template<typename InputIterator1, typename InputIterator2, typename Value, 
		 typename OutputIterator, typename BinaryOperation>
OutputIterator
_transform(InputIterator1 first1, InputIterator1 last1, Value value, 
		  OutputIterator result, BinaryOperation binary_op)
{
	for (; first1 != last1; ++first1, ++result)
		result = binary_op(*first1, value);
	return result;
}


}

bool BufferDelayPrediction::Do()
{
	unsigned numInPorts = mConfig.GetNumberOfInPorts();

	for (unsigned i = 0; i < numInPorts; ++i)
	{
		// c= (the fft/complexconjugate buffer produced by the clamnetwork)
		TData* first = mInputBufs[i]->GetData().GetBuffer().GetPtr();
		unsigned buffer_size = mInputBufs[i]->GetData().GetSize();
		TData* last = first + buffer_size;

		// c/= sqrt 
		TControlData mix_rms = mInputControls[i]->GetLastValue();
		TControlData uncompressed_rms = mInputControls[i+1]->GetLastValue();

		TData buffer_sqrt_sum(uncompressed_rms * mix_rms);
		normalize(first, last, buffer_sqrt_sum);

		// cmax=
		TData* max_location_ptr = std::max_element(first, last);
		TData max_location = buffer_size - (last - max_location_ptr);

		TData max_value = *max_location_ptr;
		TData max_value_normalized = (max_value / buffer_sqrt_sum) * 100;

		// delayPrediction=
		TData predicted_delay = (buffer_size  /*+ 1*/) - max_location;
		TData predicted_delay_in_seconds = predicted_delay / mSampleRate;

		//std::cout << "max_location=" << max_location << " max_value=" << max_value 
		//		  << " predicted_delay=" << predicted_delay << " in_seconds=" << predicted_delay_in_seconds
		//		  << " max_value_normalized=" << max_value_normalized 
		//		  << " buffer_sqrt_sum=" << buffer_sqrt_sum 
		//		  << " mix_rms=" << mix_rms 
		//		  << " uncompressed_rms=" << uncompressed_rms 
		//		  //<< " buffer="
		//		  << std::endl; 

		//std::copy(first, last, std::ostream_iterator<TData>(std::cout, " "));
		//std::cout << std::endl;

		mOutputControls[i]->SendControl(static_cast<TControlData>(predicted_delay));
		mOutputControls[i+1]->SendControl(static_cast<TControlData>(max_value_normalized));
	}

	for (unsigned i = 0; i < numInPorts; ++i)
		mInputBufs[i]->Consume();

	return true;
}

} // namespace CLAM

