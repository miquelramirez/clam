/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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


#include "FFT_rfftw.hxx"

#include <string>
#include "mtgsstream.h" // An alias for <sstream>

#include "ErrProcessingObj.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"

namespace CLAM {


	bool FFT_rfftw::ConcreteConfigure(const ProcessingConfig& c)
	{
		FFT_base::ConcreteConfigure(c);
		if(mSize==0)
		{
			mpPlan=0;		
			return false;
		}
		mpPlan = rfftw_create_plan (mSize , FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
		return true;
	}

	FFT_rfftw::FFT_rfftw()
		: mpPlan(0)
	{
		Configure(FFTConfig());
	}

	FFT_rfftw::FFT_rfftw(const FFTConfig &c)
		: mpPlan(0)
	{ 
		Configure(c);
	};

	FFT_rfftw::~FFT_rfftw()
	{
		if ( mpPlan )
			rfftw_destroy_plan(mpPlan);
		delete [] fftbuffer;
	}

	bool FFT_rfftw::Do()
	{
		return Do(mInput.GetData(),mOutput.GetData());
	};

	bool FFT_rfftw::Do(const Audio& in, Spectrum &out)
	{
		TData *inbuffer;

		if (!AbleToExecute()) return true; // object was disabled

		switch(mState) {
		case sComplex:
			inbuffer = in.GetBuffer().GetPtr();
			rfftw_one(mpPlan, inbuffer, fftbuffer);
			ToComplex(out);
			break;
		case sComplexSync:
			inbuffer = in.GetBuffer().GetPtr();
			rfftw_one(mpPlan, inbuffer, fftbuffer);
			ToComplex(out);
			out.SynchronizeTo(mComplexflags);
			break;
		case sOther:
			CheckTypes(in,out);
			inbuffer = in.GetBuffer().GetPtr();
			rfftw_one(mpPlan, inbuffer, fftbuffer);
			ToOther(out);
			break;
		default:
			CLAM_ASSERT(false, "FFT_rfftw: Do(): Inconsistent state");
		}
		out.SetSpectralRange(in.GetSampleRate()/2);
		return true;
	}

	inline void FFT_rfftw::ToComplex(Spectrum &out)
	{
		Array<Complex>& outbuffer = out.GetComplexArray();
		int i;

		outbuffer[0].SetReal(fftbuffer[0]);   // Real Values
		outbuffer[mSize/2].SetReal(fftbuffer[mSize/2]);
		outbuffer[0].SetImag(0);   // Real Values
		for (i=1; i< mSize/2; i++) {
			outbuffer[i].SetReal(fftbuffer[i]);
			outbuffer[i].SetImag(fftbuffer[mSize-i]);
		}
		if (mSize&1)
			outbuffer[mSize/2].SetImag(fftbuffer[mSize/2+1]);
		else
			outbuffer[mSize/2].SetImag(0);

		outbuffer.SetSize(mSize/2+1);
	}

	
};//namespace CLAM
