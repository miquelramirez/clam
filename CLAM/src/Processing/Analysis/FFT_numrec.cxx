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


#include "FFT_numrec.hxx"
#include <string>

#include "mtgsstream.h" // An alias for <sstream>

#include "ErrProcessingObj.hxx"
#include "Assert.hxx"

extern "C" {
#include "numrecipes_fft.h"
}

namespace CLAM {

	SpecTypeFlags FFT_numrec::mComplexflags;

	bool FFT_numrec::ConcreteConfigure(const ProcessingConfig& c)
	{
		int oldSize = mSize;

		CopyAsConcreteConfig(mConfig, c);

		if (mConfig.HasAudioSize()) {
			if (mConfig.GetAudioSize()<0)
				throw(ErrProcessingObj("Wrong (negative) Size in FFT Configuration.",this));
			mSize = mConfig.GetAudioSize();
		}

		mState=sOther;
		mComplexflags.bComplex=1;
		mComplexflags.bMagPhase=0;

		if (mSize>0) {
			if (mSize != oldSize)
				fftbuffer = new TData[mSize];
			return true;
		}
		fftbuffer = 0;
		return false;
	}

	FFT_numrec::FFT_numrec()
	{
		Configure(FFTConfig());
	}

	FFT_numrec::FFT_numrec(const FFTConfig &c) throw(ErrDynamicType)
	{ 
		Configure(c);
	};

	FFT_numrec::~FFT_numrec()
	{
		delete fftbuffer;
	}

	void FFT_numrec::CheckTypes(const Audio& in, const Spectrum &out) const
	{
		CLAM_BEGIN_CHECK
		// Input object checking
		if (in.GetSize()!=mSize) { 
			std::stringstream ss;
			ss << "FFT_numrec::Do: Wrong size in FFT Audio input\n"
			   << "  Expected: " << mSize << ", used " << in.GetSize();
			throw(ErrProcessingObj(ss.str().c_str(),this));
		}
		if (!in.HasBuffer())
			throw ErrProcessingObj("FFT Do: Float attribute required for Audio object.",this);
		if (out.GetSize() < mSize/2+1 ) { // ALGORITHM DEPENDENT CHECKING
			std::stringstream ss;
			ss << "FFT_numrec::Do: not enough memory in out Spectrum.\n"
			   << "  Expected: " << mSize/2+1 << ", used " << out.GetSize();
			throw(ErrProcessingObj(ss.str().c_str(),this));
		}
		CLAM_END_CHECK
	}


	bool FFT_numrec::SetPrototypes(const Audio& in,const Spectrum &out)
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
				throw(ErrProcessingObj("FFT_numrec: SetPrototypes(): Spectrum with no attributes!",this));

		return true;
	}

	bool FFT_numrec::UnsetPrototypes()
	{
		mState=sOther;
		return true;
	}

	void FFT_numrec::Attach(Audio& in, Spectrum &out)
	{
		mInput.Attach(in);
		mOutput.Attach(out);
	}

	bool FFT_numrec::Do()
	{
		return Do(mInput.GetData(),mOutput.GetData());
	};

	bool FFT_numrec::Do(const Audio& in, Spectrum &out) const
	{
		TData *inbuffer;
		int i;

		CLAM_DEBUG_ASSERT(GetExecState() != Unconfigured &&
		                  GetExecState() != Ready,
		                  "FFT_numrec: Do(): Not in execution mode");

		if (GetExecState() == Disabled)
			return true;

		switch(mState) {
		case sComplex:
			inbuffer = in.GetBuffer().GetPtr();
 			// Buffer dump. This is a kludge; the right way to do this
 			// is using a non-inplace version of realft (which would
 			// not reduce performance).
 			for (i=0; i<mSize; i++)
 				fftbuffer[i]=inbuffer[i];
			realft(fftbuffer-1, mSize, 1);
			NumrecToComplex(out);
			break;
		case sComplexSync:
			inbuffer = in.GetBuffer().GetPtr();
 			// Buffer dump. This is a kludge; the right way to do this
 			// is using a non-inplace version of realft (which would
 			// not reduce performance).
 			for (i=0; i<mSize; i++)
 				fftbuffer[i]=inbuffer[i];
			realft(fftbuffer-1, mSize, 1);
			NumrecToComplex(out);
			out.SynchronizeTo(mComplexflags);
			break;
		case sOther:
			CheckTypes(in,out);
			inbuffer = in.GetBuffer().GetPtr();
 			// Buffer dump. This is a kludge; the right way to do this
 			// is using a non-inplace version of realft (which would
 			// not reduce performance).
 			for (i=0; i<mSize; i++)
 				fftbuffer[i]=inbuffer[i];
			realft(fftbuffer-1, mSize, 1);
			NumrecToOther(out);
			break;
		default:
			throw(ErrProcessingObj("FFT_numrec: Do(): Inconsistent state",this));
		}
		out.SetSpectralRange(in.GetSampleRate()/2);
		return true;

	}

	bool FFT_numrec::SetPrototypes()
	{
		// @todo Check port prototypes, and set the state (or de
		// backup state if disabled) acordingly.
		throw(ErrProcessingObj("FFT_numrec::SetPrototypes: Not implemented.",this));
	}

	inline void FFT_numrec::NumrecToComplex(Spectrum &out) const
	{
		int i;
		Array<Complex>* outbuffer;

		outbuffer = &out.GetComplexArray();		
		(*outbuffer)[0].SetReal(fftbuffer[0]);   // Real Values
		(*outbuffer)[0].SetImag(0);   // Real Values
		(*outbuffer)[mSize/2].SetReal(fftbuffer[1]);
		(*outbuffer)[mSize/2].SetImag(0);
		
		for (i=1; i< mSize/2; i++) {
			(*outbuffer)[i].SetReal(fftbuffer[2*i]);  
			(*outbuffer)[i].SetImag(-fftbuffer[2*i+1]);
		}
		
		outbuffer->SetSize(mSize/2+1);
	}


	inline void FFT_numrec::NumrecToOther(Spectrum &out) const
	{
		bool hadcomplex=true;
		SpecTypeFlags flags;

		if (!out.HasComplexArray()) {
			hadcomplex=false;
			out.GetType(flags);
			flags.bComplex=1;
			out.SetType(flags);
		}

		NumrecToComplex(out);
		out.SynchronizeTo(mComplexflags);

		// @todo Can we leave the complex attribute just there?
		if (!hadcomplex) {
			out.RemoveComplexArray();
			out.UpdateData();
		}
	}
};//namespace CLAM
