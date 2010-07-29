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
	std::vector<InPort<Audio>*> _inputs;
	std::vector<OutPort<Audio>*> _outputs;

public:
	SampleAccurateBufferDelay(const Config& config = Config()) 
	{
		Configure( config );
	}
	~SampleAccurateBufferDelay()
	{
		ResizePorts(0);
	}

	bool ConcreteConfigure(const ProcessingConfig& config)
	{
		CopyAsConcreteConfig(_config, config);	
		_sampleRate = _config.GetSampleRate();
		unsigned channels = _config.HasPortsNumber()?_config.GetPortsNumber():1;
		
		_crossFadeBuffer.resize(channels);
		_delayBuffer.resize(channels);
		for (unsigned channel=0; channel<channels;channel++)
		{
			_crossFadeBuffer[channel].resize(CROSSFADESIZE);
			std::fill(_crossFadeBuffer[channel].begin(), _crossFadeBuffer[channel].end(), 0.);
		
			_delayBuffer[channel].resize(_config.GetMaxDelayInSeconds() * _sampleRate);
			std::fill(_delayBuffer[channel].begin(), _delayBuffer[channel].end(), 0.);

			_delayBufferSize = _delayBuffer[channel].size(); ////// TODO: remove on refactor (Nael)
			_readIndex = _writeIndex = (_delayBufferSize-1); ////// TODO: remove on refactor (Nael)
		}
		if ( not _config.HasInitialDelayInSamples() )
		{
			_config.AddInitialDelayInSamples();
			_config.UpdateData();
			_config.SetInitialDelayInSamples(0);
		}
		_delayControl.DoControl( (float)_config.GetInitialDelayInSamples() );

		ResizePorts(channels);

		return true;
	}
	
	const char* GetClassName() const { return "SampleAccurateBufferDelay"; }
	
	bool Do()
	{
		unsigned channels = _config.HasPortsNumber()?_config.GetPortsNumber():1;

		TControlData delay = _delayControl.GetLastValue();
		setDelay(delay);

		std::vector<const TData*> inpointers(channels);
		std::vector<TData*> outpointers(channels);
		std::vector<unsigned> inSizes(channels);

		for (unsigned channel=0; channel<channels; channel++)
		{
			const CLAM::Audio & in = _inputs[channel]->GetData();
			inSizes[channel]=in.GetSize();
			inpointers[channel]=in.GetBuffer().GetPtr();
			CLAM::Audio & out = _outputs[channel]->GetData();
			out.SetSize(inSizes[channel]);
			outpointers[channel]=out.GetBuffer().GetPtr();
		}

		for (unsigned i = 0; i < inSizes[0]; ++i)  //// TODO: remove on refactor (Nael)
		{
			for (unsigned channel=0; channel<channels;channel++)
			{
				outpointers[channel][i] = delayLine(inpointers[channel][i], channel);
			}
			_writeIndex++;
			_readIndex++;
			if (_crossFadeIndex > 0) _crossFadeIndex--;
		}
	
		for (unsigned channel=0; channel<channels; channel++)
		{
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
			"SampleAccurateBufferDelay had different number of inputs and outputs");
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
			_inputs[i] = new InPort<Audio>("Audio Input" + number.str(), this );
			_outputs[i] = new OutPort<Audio> ( "Audio Output" + number.str(), this);
		}
	}
//		Config _config;
		
};

} // namespace CLAM

#endif 

