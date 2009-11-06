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
	AudioInPort _in1;
	AudioOutPort _out1;

public:
	SampleAccurateStreamDelay(const Config& config = Config()) 
		: _in1("InputStream", this)
		, _out1("OutputStream", this)
	{
		Configure( config );
	}
	
	const char* GetClassName() const { return "SampleAccurateStreamDelay"; }
	
	bool Do()
	{
		TControlData delay = _delayControl.GetLastValue();
		setDelay(delay);
		
		const CLAM::Audio& in = _in1.GetAudio();
		const TData* inpointer = in.GetBuffer().GetPtr();		
		unsigned size = in.GetSize();
		
		CLAM::Audio& out = _out1.GetAudio();
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

