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


#include "FFT.hxx"
#include "ErrProcessingObj.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"

namespace CLAM {

	SpecTypeFlags FFT_base::mComplexflags;

	void FFTConfig::DefaultInit(void)
	{
		AddAudioSize();
		UpdateData();
		SetAudioSize(0);
	}


	FFT_base::FFT_base() :
		mSize(0),
		mInput("Input",this,1),
		mOutput("Output",this,1),
		fftbuffer( NULL )
	{
	};

	FFT_base::~FFT_base()
	{
	}

	void FFT_base::ChangeSize(int n)
	{
		CLAM_ASSERT(n>=0,"Wrong (negative) Size in control input.");

		CLAM_ASSERT(false, "Controls not yet implemented.");

	}

	void FFT_base::CheckTypes(const Audio& in, const Spectrum &out) const
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
		if (out.GetSize() != mSize/2+1 ) { // ALGORITHM DEPENDENT CHECKING
			std::stringstream ss;
			ss << "FFT_rfftw::Do: wrong size  Spectrum.\n"
			   << "  Expected: " << mSize/2+1 << ", used " << out.GetSize();
			CLAM_ASSERT(0,ss.str().c_str());
		}

		CLAM_END_CHECK
	}

	bool FFT_base::SetPrototypes(const Audio& in,const Spectrum &out)
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

	bool FFT_base::ConcreteConfigure(const ProcessingConfig& c)
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
			return false;
		}
		if (mSize == oldSize)
			return true;

		delete [] fftbuffer;
		fftbuffer = new TData[mSize];	

		SpectrumConfig cfg;
		SpecTypeFlags fl;
		fl.bMagPhase=0;
		fl.bComplex=1;
		cfg.SetType(fl);
		cfg.SetSize(mSize);
		mComplexSpectrum.Configure(cfg);		

		return true;
	}

	bool FFT_base::UnsetPrototypes()
	{
		mState=sOther;
		return true;
	}

	void FFT_base::Attach(Audio& in, Spectrum &out)
	{
		mInput.Attach(in);
		mOutput.Attach(out);
	}
	
	void FFT_base::ToOther(Spectrum &out)
	{
		if(out.HasComplexArray()) {
			ToComplex(out);
			out.SynchronizeTo(mComplexflags);
		}
		else {
			ToComplex(mComplexSpectrum);
			out.SynchronizeTo(mComplexSpectrum);
		}

	}	
		

};//namespace CLAM
