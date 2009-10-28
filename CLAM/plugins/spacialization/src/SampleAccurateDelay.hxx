/*
 * Copyright (c) 2009 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef SAMPLEACCURATEDELAY_INCLUDED
#define SAMPLEACCURATEDELAY_INCLUDED

#include <CLAM/InPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/AudioWindowingConfig.hxx>
#include <CLAM/InControl.hxx>

#include <functional>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>

namespace CLAM
{

class SampleAccurateDelayConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (SampleAccurateDelayConfig , 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, int, MaxDelayInSamples);
	DYN_ATTRIBUTE (1, public, int, SampleRate);

protected:
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetMaxDelayInSamples(65536);
		SetSampleRate(48000);
	}
};


/**
*/
class SampleAccurateDelay : public Processing
{
	InPort<Audio> _in1;
	OutPort<Audio> _out1;
	
	InControl _delayControl;
	
	SampleAccurateDelayConfig _config;
	
	typedef std::vector<TData> buffer_t;
	buffer_t delay_buffer_;
	
	unsigned sample_rate_, delay_in_samples_, delay_buffer_size_, read_index, write_index;
	
	void setdelay(float delay_in_sec) 
	{
		unsigned delay_in_samples = delay_in_sec * sample_rate_;
		
		/*std::cout << "delay_in_samples=" << delay_in_samples
				<< " delay_in_sec=" << delay_in_sec
				<< " delay_buffer_size=" << delay_buffer_size_
				<< " write_index=" << write_index 
				<< " read_index=" << read_index 
				;*/
		
		if (delay_in_samples_ > delay_buffer_size_) 
			return;
		
		read_index = (write_index - delay_in_samples) % delay_buffer_size_;

		/*std::cout << " read_index=" << read_index
				<< std::endl;*/
		
		return;
	}
	
	TData delayline(TData x)
	{	
		delay_buffer_[write_index] = x;
		TData y = delay_buffer_[read_index];
		
		read_index = (read_index+1)%delay_buffer_size_;
		write_index = (write_index+1)%delay_buffer_size_;
		
		return y;
	}
	
public:
	const char* GetClassName() const { return "SampleAccurateDelay"; }
	SampleAccurateDelay() 
		: _in1("InputBuffer", this)
		, _out1("OutputBuffer", this)
		, _delayControl("VaryingDelay", this)
		, delay_in_samples_(0.)
	{
		Configure( _config );
		delay_buffer_.resize(_config.GetMaxDelayInSamples());
		sample_rate_ = _config.GetSampleRate();
		
		read_index = write_index = delay_buffer_size_ = delay_buffer_.size(); 
		
		std::fill(delay_buffer_.begin(), delay_buffer_.end(), 0.);
	}

	bool Do()
	{
		TControlData delay = _delayControl.GetLastValue();
		setdelay(delay);
		
		// copy into output
		const CLAM::Audio& in = _in1.GetData();
		const TData* inpointer = in.GetBuffer().GetPtr();		
		unsigned size = in.GetSize();
		
		CLAM::Audio& out = _out1.GetData();
		out.SetSize(size);
		TData* outpointer = out.GetBuffer().GetPtr();
		
		for (unsigned i = 0; i < size; ++i) 
			outpointer[i] = delayline(inpointer[i]);
		
		// Tell the ports this is done
		_in1.Consume();
		_out1.Produce();
		return true;
	}
};

} // namespace CLAM

#endif 

