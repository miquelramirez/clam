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
	DYNAMIC_TYPE_USING_INTERFACE (SampleAccurateDelayConfig , 1, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, int, MaxDelayInSamples);

protected:
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetMaxDelayInSamples(48000);
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
	
	// http://ccrma.stanford.edu/~jos/pasp/Variable_Delay_Lines.html#sec:dllvar
	std::vector<TData> A;
	signed N;
	TData *rptr; // read ptr
	TData *wptr; // write ptr

	TData setdelay(int M) 
	{
		rptr = wptr - M;
		while (rptr < &A[0]) { rptr += N; }
		return 0;
	}

	TData delayline(TData x)
	{
		TData y;
		*wptr++ = x; 
		y = *rptr++;
		if (wptr-&A[0] >= N) { wptr -= N; }
		if (rptr-&A[0] >= N) { rptr -= N; }
		return y;
	}

public:
	const char* GetClassName() const { return "SampleAccurateDelay"; }
	SampleAccurateDelay() 
		: _in1("InputBuffer", this)
		, _out1("OutputBuffer", this)
		, _delayControl("VaryingDelay", this)
	{
		Configure( _config );
		N = _config.GetMaxDelayInSamples();
		A.resize(N);

		rptr = &A[0]; // read ptr
		wptr = &A[0]; // write ptr

		std::fill(A.begin(), A.end(), 0.);
	}

	bool Do()
	{
		
		TControlData delay = _delayControl.GetLastValue();
		setdelay(static_cast<int>(delay));
		
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

