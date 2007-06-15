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

#include "IFFT_rfftw.hxx"
#include "SpecTypeFlags.hxx"

#include "Audio.hxx"
#include "Spectrum.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM {

	namespace detail
	{
		static ProcessingFactory::Registrator<IFFT_rfftw> regtIFFT_rfftw( "IFFT_rfftw" );
	}
	
	bool IFFT_rfftw::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		if (mConfig.HasAudioSize()) {
			if (mConfig.GetAudioSize()<=1)
			{
				AddConfigErrorMessage("Wrong Size, should be greater than 1");
				return false;
			}
			mSize = mConfig.GetAudioSize();	
		}
		else
			mSize = CLAM_DEFAULT_IFFT_SIZE;
		mOutput.SetSize( mSize );
		mOutput.SetHop( mSize );

		mState=sOther;
		mComplexflags.bComplex=1;
		mComplexflags.bMagPhase=0;
		if (mSize>0) {
			delete [] ifftbuffer;
			ifftbuffer = new TData[mSize];
			mpPlan = rfftw_create_plan (mSize , FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE);
			return true;
		}
		ifftbuffer = 0;
		mpPlan=0;
		return false;
	}


	IFFT_rfftw::IFFT_rfftw()
		: mpPlan(0),
		  ifftbuffer(0)
	{
		Configure(IFFTConfig());
	}

	IFFT_rfftw::IFFT_rfftw(const IFFTConfig &c) throw(ErrDynamicType)
		: mpPlan(0),
		  ifftbuffer(0)
	{ 
		Configure(c);
	};

	IFFT_rfftw::~IFFT_rfftw()
	{
		if (mpPlan)
			rfftw_destroy_plan(mpPlan);
		delete [] ifftbuffer;
	}

	void IFFT_rfftw::CheckTypes(const Spectrum& in, const Audio &out) const
	{
		CLAM_ASSERT(out.HasBuffer(),"IFFT Do: Float attribute required for Audio object.");
		CLAM_BEGIN_CHECK
			// Input object checking
			if (out.GetSize()!=mSize) { 
				std::stringstream ss;
				ss << "IFFT_rfftw::Do: Wrong size in IFFT Audio output\n"
				   << "  Expected: " << mSize*2+1 << ", used " << out.GetSize();
				CLAM_ASSERT(0,ss.str().c_str());
			}
			if (in.GetSize() < mSize/2+1 ) { // ALGORITHM DEPENDENT CHECKING
				std::stringstream ss;
				ss << "IFFT_rfftw::Do: not enough memory in input Spectrum.\n"
				   << "  Expected: " << mSize/2+1 << ", used " << in.GetSize();
				CLAM_ASSERT(0,ss.str().c_str());

			}
		CLAM_END_CHECK
	}
/*
	void IFFT_rfftw::Attach(Spectrum &in, Audio& out)
	{
		mInput.Attach(in);
		mOutput.Attach(out);
	}
*/
	
	bool IFFT_rfftw::Do()
	{

/*
		switch(mState) {
		case sOther:
		case sComplex:
			CLAM_ASSERT(false,"IFFT_rfftw: Do(IFFTInputOutput&,IFFTInputOutput&): Not implemented");
		default:
			CLAM_ASSERT(false,"IFFT_rfftw: Do(): Inconsistent state");
		}

		
*/
		bool toReturn = Do(mInput.GetData(),mOutput.GetAudio());
		mInput.Consume();
		mOutput.Produce();
		return toReturn;
	};


	bool IFFT_rfftw::SetPrototypes(const Spectrum& in, const Audio &out)
	{
		CheckTypes(in,out);

		SpecTypeFlags flags; 
		in.GetType(flags);

		if (flags.bComplex)
			mState=sComplex;
		else
		{
			CLAM_ASSERT(flags.bPolar || flags.bMagPhase || flags.bMagPhaseBPF,"IFFT_rfftw: SetPrototypes(): No Spectrum Attributes!");
			mState=sOther;
		}
		// TODO: Maybe should update the prototype in the port?

		return true;
	}

	bool IFFT_rfftw::UnsetPrototypes()
	{
		mState=sOther;
		return true;
	}


	bool IFFT_rfftw::Do( const Spectrum& inFoo, Audio &out) const
	{
		CLAM_ASSERT(IsRunning(),
			"IFFT_rfftw: Do(): Not in execution mode");
		CLAM_ASSERT(out.GetSize() == mSize,
			"Not proper IFFT output size");
		// TODO: Avoid copy, this solution is provisional
		Spectrum in = inFoo;
		
		TData *outbuffer;

		switch(mState) {
		case sComplex:
			ComplexToRIFFTW(in);
			outbuffer = out.GetBuffer().GetPtr();
			rfftw_one(mpPlan, ifftbuffer, outbuffer);
			break;
		case sOther:
			CheckTypes(in,out);
			OtherToRIFFTW(in);
			outbuffer = out.GetBuffer().GetPtr();
			rfftw_one(mpPlan, ifftbuffer, outbuffer);
			break;
		default:
			CLAM_ASSERT(false,"IFFT_rfftw: Do(): Inconsistent state");
		}
		out.SetSampleRate(TData(in.GetSpectralRange()*2));
		return true;
	}

	bool IFFT_rfftw::SetPrototypes()
	{
		// @todo Check port prototypes, and set the state (or de
		// backup state if disabled) acordingly.
		CLAM_ASSERT(false,"IFFT_rfftw::SetPrototypes: Not implemented.");
		return false;
	}

	inline void IFFT_rfftw::ComplexToRIFFTW(Spectrum &in) const
	{
		int i;
		Array<Complex>* inbuffer;
		inbuffer = &in.GetComplexArray();		

		ifftbuffer[0]=(*inbuffer)[0].Real()/mSize;   // Real Values
		ifftbuffer[mSize/2]=(*inbuffer)[mSize/2].Real()/mSize;
		for (i=1; i< mSize/2; i++) {
			ifftbuffer[i] = (*inbuffer)[i].Real()/mSize;  
			ifftbuffer[mSize-i] = (*inbuffer)[i].Imag()/mSize;
		}
		if (mSize&1)
			ifftbuffer[mSize/2+1] = (*inbuffer)[mSize/2].Imag()/mSize;
	}


	inline void IFFT_rfftw::OtherToRIFFTW(Spectrum &in) const
	{
		bool hadcomplex=true;
		SpecTypeFlags flags;


		if (!in.HasComplexArray()) {
			hadcomplex=false;
			in.GetType(flags);
			flags.bComplex=1;
			in.SetTypeSynchronize(flags);
		}

		ComplexToRIFFTW(in);

		// @todo: Can we leave the complex attribute just there?
		if (!hadcomplex) {
			in.RemoveComplexArray();
			in.UpdateData();
		}
	}
}

