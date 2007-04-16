/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Audio.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM {
	
	namespace detail
	{
		static ProcessingFactory::Registrator<FFT_rfftw> regtFFT_rfftw( "FFT_rfftw" );
	}

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
	}

	bool FFT_rfftw::Do()
	{
		mOutput.GetData().SetSize( mInput.GetSize()/2+1);
		bool toReturn = Do(mInput.GetAudio(), mOutput.GetData());
		mInput.Consume();
		mOutput.Produce();
		return toReturn;
	}

	bool FFT_rfftw::Do(const Audio& in, Spectrum &out)
	{
		TData *inbuffer;

		if (!AbleToExecute()) return true; // object was disabled
		
		out.SetSpectralRange(in.GetSampleRate()/2);
		
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
		return true;
	}

	inline void FFT_rfftw::ToComplex(Spectrum &out)
	{
		Array<Complex>& outbuffer = out.GetComplexArray();

		outbuffer[0].SetReal(fftbuffer[0]);   // Real Values
		outbuffer[mSize/2].SetReal(fftbuffer[mSize/2]);
		outbuffer[0].SetImag(0);   // Real Values
		for (int i=1; i< mSize/2; i++) {
			outbuffer[i].SetReal(fftbuffer[i]);
			outbuffer[i].SetImag(fftbuffer[mSize-i]);
		}
		if (mSize&1)
			outbuffer[mSize/2].SetImag(fftbuffer[mSize/2+1]);
		else
			outbuffer[mSize/2].SetImag(0);

		outbuffer.SetSize(mSize/2+1); // TOOD: Any sense?
	}

	
};//namespace CLAM

