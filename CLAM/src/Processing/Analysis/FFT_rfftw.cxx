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

	SpecTypeFlags FFT_rfftw::mComplexflags;


	bool FFT_rfftw::ConcreteConfigure(const ProcessingConfig& c)
	{
		int oldSize = mSize;
		
		CopyAsConcreteConfig(mConfig, c);
		if (mConfig.HasAudioSize()) {
			CLAM_ASSERT(mSize>=0, "Negative Size in FFT configuration");
			mSize = mConfig.GetAudioSize();
		}

		CLAM_ASSERT(mSize>=0, "Negative Size in FFT configuration");

		mState=sOther;
		mComplexflags.bComplex=1;
		mComplexflags.bMagPhase=0;
		if (mSize == 0) 
		{
			fftbuffer = 0;
			mpPlan=0;
			return false;
		}
		if (mSize == oldSize)
			return true;

		delete [] fftbuffer;
		fftbuffer = new TData[mSize];
		mpPlan = rfftw_create_plan (mSize , FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
		return true;
	}

	FFT_rfftw::FFT_rfftw()
		: mpPlan(0),
		  fftbuffer(0)
	{
		Configure(FFTConfig());
	}

	FFT_rfftw::FFT_rfftw(const FFTConfig &c) throw(ErrDynamicType)
		: mpPlan(0),
		  fftbuffer(0)
	{ 
		Configure(c);
	};

	FFT_rfftw::~FFT_rfftw()
	{
		if ( mpPlan )
			rfftw_destroy_plan(mpPlan);
		delete [] fftbuffer;
	}

	void FFT_rfftw::CheckTypes(const Audio& in, const Spectrum &out) const
	{

		CLAM_BEGIN_CHECK
		// Input object checking
		if (in.GetSize()!=mSize) { 
			std::stringstream ss;
			ss << "FFT_rfftw::Do: Wrong size in FFT Audio input\n"
			   << "  Expected: " << mSize << ", used " << in.GetSize();
			CLAM_ASSERT(0,ss.str().c_str());
		}
		if (!in.HasBuffer())
			CLAM_ASSERT(0,"FFT Do: Float attribute required for Audio object.");
		if (out.GetSize() < mSize/2+1 ) { // ALGORITHM DEPENDENT CHECKING
			std::stringstream ss;
			ss << "FFT_rfftw::Do: not enough memory in out Spectrum.\n"
			   << "  Expected: " << mSize/2+1 << ", used " << out.GetSize();
			CLAM_ASSERT(0,ss.str().c_str());
		}

		CLAM_END_CHECK
	}

	bool FFT_rfftw::SetPrototypes(const Audio& in,const Spectrum &out)
	{
		CheckTypes(in,out);

		SpecTypeFlags flags;
		out.GetType(flags);

		if (flags.bComplex)
			if (flags.bPolar || flags.bMagPhase || flags.bMagPhaseBPF)
				mState=sComplexSync;
			else
				mState=sComplex;
		else
			if (flags.bPolar || flags.bMagPhase || flags.bMagPhaseBPF)
				mState=sOther;
			else
				CLAM_ASSERT(false, "FFT_rfftw: SetPrototypes(...): No Spectrum Attributes!");

		return true;
	}

	bool FFT_rfftw::UnsetPrototypes()
	{
		mState=sOther;
		return true;
	}

	void FFT_rfftw::Attach(Audio& in, Spectrum &out)
	{
		mInput.Attach(in);
		mOutput.Attach(out);
	}


	bool FFT_rfftw::Do()
	{
		return Do(mInput.GetData(),mOutput.GetData());
	};

	bool FFT_rfftw::Do(const Audio& in, Spectrum &out) const
	{
		TData *inbuffer;

		if (!AbleToExecute()) return true; // object was disabled

		switch(mState) {
		case sComplex:
			inbuffer = in.GetBuffer().GetPtr();
			rfftw_one(mpPlan, inbuffer, fftbuffer);
			RFFTWToComplex(out);
			break;
		case sComplexSync:
			inbuffer = in.GetBuffer().GetPtr();
			rfftw_one(mpPlan, inbuffer, fftbuffer);
			RFFTWToComplex(out);
			out.SynchronizeTo(mComplexflags);
			break;
		case sOther:
			CheckTypes(in,out);
			inbuffer = in.GetBuffer().GetPtr();
			rfftw_one(mpPlan, inbuffer, fftbuffer);
			RFFTWToOther(out);
			break;
		default:
			CLAM_ASSERT(false, "FFT_rfftw: Do(): Inconsistent state");
		}
		out.SetSpectralRange(in.GetSampleRate()/2);
		return true;
	}

	bool FFT_rfftw::SetPrototypes()
	{
		// @todo Check port prototypes, and set the state (or de
		// backup state if disabled) acordingly.
		CLAM_ASSERT(false,"FFT_rfftw::SetPrototypes: Not implemented.");
	}


	inline void FFT_rfftw::RFFTWToComplex(Spectrum &out) const
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


	inline void FFT_rfftw::RFFTWToOther(Spectrum &out) const
	{
		bool hadcomplex=true;
		SpecTypeFlags flags;

		if (!out.HasComplexArray()) {
			hadcomplex=false;
			out.GetType(flags);
			flags.bComplex=1;
			out.SetType(flags);
		}

		RFFTWToComplex(out);
		out.SynchronizeTo(mComplexflags);

		// @todo Can we leave the complex attribute just there?
		if (!hadcomplex) {
			out.RemoveComplexArray();
			out.UpdateData();
		}
	}
};//namespace CLAM
