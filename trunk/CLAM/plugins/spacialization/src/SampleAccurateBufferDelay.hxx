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

#ifndef SAMPLEACCURATEBUFFERDELAY_INCLUDED
#define SAMPLEACCURATEBUFFERDELAY_INCLUDED

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include "SampleAccurateDelay.hxx"

namespace CLAM
{

/**
 *
 */
class SampleAccurateBufferDelay : public SampleAccurateDelay
{
	InPort<Audio> _in1;
	OutPort<Audio> _out1;

public:
	SampleAccurateBufferDelay(const Config& config = Config()) 
		: _in1("InputBuffer", this)
		, _out1("OutputBuffer", this)
	{
		Configure( config );
	}

	bool ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(_config, c);	
		_sampleRate = _config.GetSampleRate();
		
		_crossFadeBuffer.resize(CROSSFADESIZE);
		std::fill(_crossFadeBuffer.begin(), _crossFadeBuffer.end(), 0.);
		
		_delayBuffer.resize(_config.GetMaxDelayInSeconds() * _sampleRate);
		_delayBufferSize = _delayBuffer.size(); 
		_readIndex = _writeIndex = (_delayBufferSize-1); 
		std::fill(_delayBuffer.begin(), _delayBuffer.end(), 0.);
		_delayControl.DoControl(0.);

		return true;
	}
	
	const char* GetClassName() const { return "SampleAccurateBufferDelay"; }
	
	bool Do()
	{
		TControlData delay = _delayControl.GetLastValue();
		setDelay(delay);
		
		const CLAM::Audio& in = _in1.GetData();
		const TData* inpointer = in.GetBuffer().GetPtr();		
		unsigned size = in.GetSize();
		
		CLAM::Audio& out = _out1.GetData();
		out.SetSize(size);
		TData* outpointer = out.GetBuffer().GetPtr();
		
		for (unsigned i = 0; i < size; ++i) 
			outpointer[i] = delayLine(inpointer[i]);
		
		_in1.Consume();
		_out1.Produce();
		return true;
	}
};

} // namespace CLAM

#endif 

