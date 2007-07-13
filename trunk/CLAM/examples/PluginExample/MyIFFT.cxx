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

#include "MyIFFT.hxx"
#include "SpecTypeFlags.hxx"

#include "Audio.hxx"
#include "Spectrum.hxx"
#include "ProcessingFactory.hxx"

#include <fftw3.h>


namespace CLAM {

namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, MyIFFT> regMyIFFT("MyIFFT");
}
	
struct MyIFFT::Implementation
{
	Implementation(unsigned size)
	{
		// Special malloc which aligns to SIMD segment boundaries
		_complexInput = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
		_realOutput = (double*) fftw_malloc(sizeof(double) * size);
		fftw_import_system_wisdom();
		_plan = fftw_plan_dft_c2r_1d(size, _complexInput, _realOutput, FFTW_ESTIMATE);
	}
	~Implementation()
	{
		fftw_destroy_plan(_plan);
		fftw_free(_realOutput);
		fftw_free(_complexInput);
	}
	fftw_plan _plan;
	fftw_complex * _complexInput;
	double * _realOutput;
};

MyIFFT::MyIFFT(const IFFTConfig &c)
	: _fftw3(0)
{ 
	Configure(c);
};

bool MyIFFT::ConcreteConfigure(const ProcessingConfig& c)
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
	SetupMemory();
	return true;
}

MyIFFT::~MyIFFT()
{
	ReleaseMemory();
}

void MyIFFT::ReleaseMemory()
{
	if (_fftw3) delete _fftw3;
}
void MyIFFT::SetupMemory()
{
	ReleaseMemory();
	_fftw3 = new Implementation(mSize);
}

void MyIFFT::CheckTypes(const Spectrum& in, const Audio &out) const
{
	CLAM_ASSERT(out.HasBuffer(),"IFFT Do: Float attribute required for Audio object.");
	CLAM_BEGIN_CHECK
		// Input object checking
		if (out.GetSize()!=mSize) { 
			std::stringstream ss;
			ss << "MyIFFT::Do: Wrong size in IFFT Audio output\n"
			   << "  Expected: " << mSize*2+1 << ", used " << out.GetSize();
			CLAM_ASSERT(0,ss.str().c_str());
		}
		if (in.GetSize() < mSize/2+1 ) { // ALGORITHM DEPENDENT CHECKING
			std::stringstream ss;
			ss << "MyIFFT::Do: not enough memory in input Spectrum.\n"
			   << "  Expected: " << mSize/2+1 << ", used " << in.GetSize();
			CLAM_ASSERT(0,ss.str().c_str());

		}
	CLAM_END_CHECK
}
/*
void MyIFFT::Attach(Spectrum &in, Audio& out)
{
	mInput.Attach(in);
	mOutput.Attach(out);
}
*/

bool MyIFFT::Do()
{
	bool toReturn = Do(mInput.GetData(),mOutput.GetAudio());
	mInput.Consume();
	mOutput.Produce();
	return toReturn;
};


bool MyIFFT::SetPrototypes(const Spectrum& in, const Audio &out)
{
	CheckTypes(in,out);

	SpecTypeFlags flags; 
	in.GetType(flags);

	if (flags.bComplex)
		mState=sComplex;
	else
	{
		CLAM_ASSERT(flags.bPolar || flags.bMagPhase || flags.bMagPhaseBPF,"MyIFFT: SetPrototypes(): No Spectrum Attributes!");
		mState=sOther;
	}
	// TODO: Maybe should update the prototype in the port?

	return true;
}

bool MyIFFT::UnsetPrototypes()
{
	mState=sOther;
	return true;
}


bool MyIFFT::Do( const Spectrum& in, Audio &out) const
{
	CLAM_ASSERT(IsRunning() ,"MyIFFT: Do(): Not in execution mode");
	CLAM_ASSERT(out.GetSize() == mSize,
		"Not proper IFFT output size");

	if (mState==sComplex)
		ComplexToRIFFTW(in);
	else
	{
		CheckTypes(in,out);
		OtherToRIFFTW(in);
	}
	fftw_execute(_fftw3->_plan);
	TData * outbuffer = out.GetBuffer().GetPtr();
	for (int i=0; i<mSize; i++)
		outbuffer[i] = _fftw3->_realOutput[i];

	out.SetSampleRate(TData(in.GetSpectralRange()*2));
	return true;
}

bool MyIFFT::SetPrototypes()
{
	// @todo Check port prototypes, and set the state (or de
	// backup state if disabled) acordingly.
	CLAM_ASSERT(false,"MyIFFT::SetPrototypes: Not implemented.");
	return false;
}

inline void MyIFFT::ComplexToRIFFTW(const Spectrum &in) const
{
	CLAM_ASSERT(in.HasComplexArray(), "Input spectrum has no complex array");
	const Array<Complex> & inbuffer = in.GetComplexArray();
	CLAM_ASSERT(inbuffer.Size() == mSize/2+1, "MyIFFT::ComplexToRIFFTW: sizes doesn't match.");
	for (int i=0; i< inbuffer.Size(); i++)
	{
		_fftw3->_complexInput[i][0] = inbuffer[i].Real()/mSize;
		_fftw3->_complexInput[i][1] = inbuffer[i].Imag()/mSize;
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


inline void MyIFFT::OtherToRIFFTW(const Spectrum &in) const
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

