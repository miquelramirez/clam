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

#include "IFFT_fftw3.hxx"
#include "SpecTypeFlags.hxx"

#include "Audio.hxx"
#include "Spectrum.hxx"
#include "ProcessingFactory.hxx"


namespace CLAM {

namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, IFFT_fftw3> regIFFT_fftw3("IFFT_fftw3");
}
	
void IFFT_fftw3::ReleaseMemory()
{
	if (_plan)
	{
		fftw_destroy_plan(*_plan);
		delete _plan;
		_plan=0;
	}
	if (_realOutput)
	{
		fftw_free(_realOutput);
		_realOutput=0;
	}
	if (_complexInput)
	{
		fftw_free(_complexInput);
		_complexInput=0;
	}
	fftw_forget_wisdom();
}
void IFFT_fftw3::SetupMemory()
{
	ReleaseMemory();
	// Special malloc which aligns to SIMD segment boundaries
	_complexInput = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * mSize);
	_realOutput = (double*) fftw_malloc(sizeof(double) * mSize);
	fftw_import_system_wisdom();
	_plan = new fftw_plan;
	*_plan = fftw_plan_dft_c2r_1d(mSize, _complexInput, _realOutput, FFTW_ESTIMATE);
}

bool IFFT_fftw3::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	mSize = CLAM_DEFAULT_IFFT_SIZE;
	if (mConfig.HasAudioSize()) {
		CLAM_ASSERT (mConfig.GetAudioSize()>=0,"Wrong (negative) Size in IFFT Configuration.");
		mSize = mConfig.GetAudioSize();	
	}
	mOutput.SetSize( mSize );
	mOutput.SetHop( mSize );

	mState=sOther;
	mComplexflags.bComplex=1;
	mComplexflags.bMagPhase=0;
	SetupMemory();
	return true;
}


IFFT_fftw3::IFFT_fftw3()
	: _plan(0)
	, _complexInput(0)
	, _realOutput(0)
{
	Configure(IFFTConfig());
}

IFFT_fftw3::IFFT_fftw3(const IFFTConfig &c) throw(ErrDynamicType)
	: _plan(0)
	, _complexInput(0)
	, _realOutput(0)
{ 
	Configure(c);
};

IFFT_fftw3::~IFFT_fftw3()
{
	ReleaseMemory();
}

void IFFT_fftw3::CheckTypes(const Spectrum& in, const Audio &out) const
{
	CLAM_ASSERT(out.HasBuffer(),"IFFT Do: Float attribute required for Audio object.");
	CLAM_BEGIN_CHECK
		// Input object checking
		if (out.GetSize()!=mSize) { 
			std::stringstream ss;
			ss << "IFFT_fftw3::Do: Wrong size in IFFT Audio output\n"
			   << "  Expected: " << mSize*2+1 << ", used " << out.GetSize();
			CLAM_ASSERT(0,ss.str().c_str());
		}
		if (in.GetSize() < mSize/2+1 ) { // ALGORITHM DEPENDENT CHECKING
			std::stringstream ss;
			ss << "IFFT_fftw3::Do: not enough memory in input Spectrum.\n"
			   << "  Expected: " << mSize/2+1 << ", used " << in.GetSize();
			CLAM_ASSERT(0,ss.str().c_str());

		}
	CLAM_END_CHECK
}
/*
void IFFT_fftw3::Attach(Spectrum &in, Audio& out)
{
	mInput.Attach(in);
	mOutput.Attach(out);
}
*/

bool IFFT_fftw3::Do()
{
	bool toReturn = Do(mInput.GetData(),mOutput.GetAudio());
	mInput.Consume();
	mOutput.Produce();
	return toReturn;
};


bool IFFT_fftw3::SetPrototypes(const Spectrum& in, const Audio &out)
{
	CheckTypes(in,out);

	SpecTypeFlags flags; 
	in.GetType(flags);

	if (flags.bComplex)
		mState=sComplex;
	else
	{
		CLAM_ASSERT(flags.bPolar || flags.bMagPhase || flags.bMagPhaseBPF,"IFFT_fftw3: SetPrototypes(): No Spectrum Attributes!");
		mState=sOther;
	}
	// TODO: Maybe should update the prototype in the port?

	return true;
}

bool IFFT_fftw3::UnsetPrototypes()
{
	mState=sOther;
	return true;
}


bool IFFT_fftw3::Do( const Spectrum& in, Audio &out) const
{
	CLAM_ASSERT(IsRunning() ,"IFFT_fftw3: Do(): Not in execution mode");
	CLAM_ASSERT(out.GetSize() == mSize,
		"Not proper IFFT output size");

	if (mState==sComplex)
		ComplexToRIFFTW(in);
	else
	{
		CheckTypes(in,out);
		OtherToRIFFTW(in);
	}
	fftw_execute(*_plan);
	TData * outbuffer = out.GetBuffer().GetPtr();
	for (int i=0; i<mSize; i++)
		outbuffer[i] = _realOutput[i];

	out.SetSampleRate(TData(in.GetSpectralRange()*2));
	return true;
}

bool IFFT_fftw3::SetPrototypes()
{
	// @todo Check port prototypes, and set the state (or de
	// backup state if disabled) acordingly.
	CLAM_ASSERT(false,"IFFT_fftw3::SetPrototypes: Not implemented.");
	return false;
}

inline void IFFT_fftw3::ComplexToRIFFTW(const Spectrum &in) const
{
	CLAM_ASSERT(in.HasComplexArray(), "Input spectrum has no complex array");
	const Array<Complex> & inbuffer = in.GetComplexArray();
	CLAM_ASSERT(inbuffer.Size() == mSize/2+1, "IFFT_fftw3::ComplexToRIFFTW: sizes doesn't match.");
	for (int i=0; i< inbuffer.Size(); i++) {
		_complexInput[i][0] = inbuffer[i].Real()/mSize;
		_complexInput[i][1] = inbuffer[i].Imag()/mSize;
	}
/*
 	// TODO: Use this code for the r2r format
	ifftbuffer[0]=(*inbuffer)[0].Real()/mSize;   // Real Values
	ifftbuffer[mSize/2]=(*inbuffer)[mSize/2].Real()/mSize;
	for (int i=1; i< mSize/2; i++) {
		ifftbuffer[i] = (*inbuffer)[i].Real()/mSize;  
		ifftbuffer[mSize-i] = (*inbuffer)[i].Imag()/mSize;
	}
	if (mSize&1)
		ifftbuffer[mSize/2+1] = (*inbuffer)[mSize/2].Imag()/mSize;
*/
}


inline void IFFT_fftw3::OtherToRIFFTW(const Spectrum &in) const
{
	if (in.HasComplexArray())
	{
		ComplexToRIFFTW(in);
		return;
	}
	SpecTypeFlags flags;
	Spectrum spec = in;
	spec.GetType(flags);
	flags.bComplex=1;
	spec.SetTypeSynchronize(flags);
	ComplexToRIFFTW(spec);
}
}

