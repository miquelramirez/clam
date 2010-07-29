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

#ifndef SAMPLEACCURATESTREAMDELAY_INCLUDED
#define SAMPLEACCURATESTREAMDELAY_INCLUDED

#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include "SampleAccurateDelay.hxx"

namespace CLAM
{

/**
 *
 */
class SampleAccurateStreamDelay : public SampleAccurateDelay
{
	std::vector<AudioInPort*> _inputs;
	std::vector<AudioOutPort*> _outputs;

public:

	SampleAccurateStreamDelay(const Config& config = Config()) 
	{
		Configure( config );
	}

	~SampleAccurateStreamDelay()
	{
		ResizePorts(0);
	}
	
	bool ConcreteConfigure(const ProcessingConfig& config)
	{
		CopyAsConcreteConfig(_config, config);	
		_channels = _config.HasPortsNumber() ? _config.GetPortsNumber() : 1;
		ResizePorts(_channels);
		return SampleAccurateDelay::ConcreteConfigure();
	}

	const char* GetClassName() const { return "SampleAccurateStreamDelay"; }
	
	bool Do()
	{
		TControlData delay = _delayControl.GetLastValue();
		setDelay(delay);

		for (unsigned channel=0; channel<_channels; channel++)
		{
			const CLAM::Audio & in = _inputs[channel]->GetAudio();
			CLAM::Audio & out = _outputs[channel]->GetAudio();
		
			delayLine(in, out, channel);	
				
			_inputs[channel]->Consume();
			_outputs[channel]->Produce();
		}
		
		return true;
	}
	private: 
	void ResizePorts(unsigned newSize)
	{
		unsigned oldSize = _inputs.size();
		CLAM_ASSERT(_inputs.size()==_outputs.size(),
			"SampleAccurateStreamDelay had different number of inputs and outputs");
		for (unsigned i = newSize; i<oldSize; i++)
		{
			delete _inputs[i];
			delete _outputs[i];
		}
		_inputs.resize(newSize);
		_outputs.resize(newSize);
		for (unsigned i = oldSize; i<newSize; i++)
		{
			std::ostringstream number;
			if (i>0) number << " " << i;
			_inputs[i] = new AudioInPort("Audio Input" + number.str(), this );
			_outputs[i] = new AudioOutPort ( "Audio Output" + number.str(), this);
		}
		const unsigned buffersize = BackendBufferSize();

		for (unsigned channel = 0; channel<newSize; channel++)
		{
			_inputs[channel]->SetSize(buffersize);
			_inputs[channel]->SetHop(buffersize);
			_outputs[channel]->SetSize(buffersize);
			_outputs[channel]->SetHop(buffersize);
		}
			
	}
};

} // namespace CLAM

#endif 

