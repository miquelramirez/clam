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


#include "FFT_numrec.hxx"

#include "Assert.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "CLAM_Math.hxx"

extern "C" {
#include "numrecipes_fft.h"
}

namespace CLAM {


	bool FFT_numrec::ConcreteConfigure(const ProcessingConfig& c)
	{
		FFT_base::ConcreteConfigure(c);
		if ( !isPowerOfTwo( mSize ) )
		{
			AddConfigErrorMessage("Configure failed: Numerical Recipes FFT algorithm does not"
				"accept non power of two buffers");

			return false;
		}

		return true;
	}

	FFT_numrec::FFT_numrec()
	{
		Configure(FFTConfig());
	}

	FFT_numrec::FFT_numrec(const FFTConfig &c)
	{ 
		Configure(c);
	};

	FFT_numrec::~FFT_numrec()
	{
	}

	bool FFT_numrec::Do()
	{
		mOutput.GetData().SetSize( mInput.GetSize()/2+1);
		bool toReturn = Do(mInput.GetAudio(), mOutput.GetData());
		mInput.Consume();
		mOutput.Produce();
		return toReturn;
	};

	bool FFT_numrec::Do(const Audio& in, Spectrum &out)
	{
		TData *inbuffer;

		CLAM_DEBUG_ASSERT(GetExecState() == Running,
		                  "FFT_numrec: Do(): Not in execution mode");

		out.SetSpectralRange(in.GetSampleRate()/2);

		switch(mState) {
		case sComplex:
			inbuffer = in.GetBuffer().GetPtr();
 			// Buffer dump. This is a kludge; the right way to do this
 			// is using a non-inplace version of realft (which would
 			// not reduce performance).
 			for (int i=0; i<mSize; i++)
 				fftbuffer[i]=inbuffer[i];
			realft(fftbuffer-1, mSize, 1);
			ToComplex(out);
			break;
		case sComplexSync:
			inbuffer = in.GetBuffer().GetPtr();
 			// Buffer dump. This is a kludge; the right way to do this
 			// is using a non-inplace version of realft (which would
 			// not reduce performance).
 			for (int i=0; i<mSize; i++)
 				fftbuffer[i]=inbuffer[i];
			realft(fftbuffer-1, mSize, 1);
			ToComplex(out);
			out.SynchronizeTo(mComplexflags);
			break;
		case sOther:
			CheckTypes(in,out);
			inbuffer = in.GetBuffer().GetPtr();
 			// Buffer dump. This is a kludge; the right way to do this
 			// is using a non-inplace version of realft (which would
 			// not reduce performance).
 			for (int i=0; i<mSize; i++)
 				fftbuffer[i]=inbuffer[i];
			realft(fftbuffer-1, mSize, 1);
			ToOther(out);
			break;
		default:
			CLAM_ASSERT(false, "FFT_numrec: Do(): Inconsistent state");
		}
	
		return true;

	}


	void FFT_numrec::ToComplex(Spectrum &out)
	{
		Array<Complex>* outbuffer;

		outbuffer = &out.GetComplexArray();		
		(*outbuffer)[0].SetReal(fftbuffer[0]);   // Real Values
		(*outbuffer)[0].SetImag(0);   // Real Values
		(*outbuffer)[mSize/2].SetReal(fftbuffer[1]);
		(*outbuffer)[mSize/2].SetImag(0);
		
		for (int i=1; i< mSize/2; i++) {
			(*outbuffer)[i].SetReal(fftbuffer[2*i]);  
			(*outbuffer)[i].SetImag(-fftbuffer[2*i+1]);
		}
		
		outbuffer->SetSize(mSize/2+1);
	}


};//namespace CLAM

