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


#include "FFT_fftw3.hxx"

#include "Assert.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "CLAM_Math.hxx"
#include "Factory.hxx"

namespace CLAM {

 
	namespace detail
	{
		static Factory<Processing>::Registrator<FFT_fftw3> regtFFT_fftw3( "FFT_fftw3" );
	}

bool FFT_fftw3::ConcreteConfigure(const ProcessingConfig& c)
{
	int oldSize=mSize;
	FFT_base::ConcreteConfigure(c);
	if (mSize<=0) 
	{
		AddConfigErrorMessage("Invalid zero or negative input size");
		return false;
	}
	if (mSize == oldSize) return true;
	SetupMemory();
	return true;
}

void FFT_fftw3::ReleaseMemory() {
	if (_plan)
	{
		fftw_destroy_plan(*_plan);
		delete _plan;
		_plan=0;
	}
	if (_realInput)
	{
		fftw_free(_realInput);
		_realInput=0;
	}
	if (_complexOutput)
	{
		fftw_free(_complexOutput);
		_complexOutput=0;
	}
}

void FFT_fftw3::SetupMemory() {
	ReleaseMemory();
	// Special malloc which aligns to SIMD segment boundaries
	_realInput = (double*) fftw_malloc(sizeof(double) * mSize);
	_complexOutput = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * mSize);
	_plan = new fftw_plan;
	*_plan = fftw_plan_dft_r2c_1d(mSize, _realInput, _complexOutput, FFTW_ESTIMATE);
	}

FFT_fftw3::FFT_fftw3()
	: _realInput(0)
	, _complexOutput(0)
	, _plan(0)
{
	Configure(FFTConfig());
}

FFT_fftw3::FFT_fftw3(const FFTConfig &c) throw(ErrDynamicType)
	: _realInput(0)
	, _complexOutput(0)
	, _plan(0)
{ 
	Configure(c);
}

FFT_fftw3::~FFT_fftw3()
{
	ReleaseMemory();
}

bool FFT_fftw3::Do() 
{
	mOutput.GetData().SetSize( mInput.GetSize()/2+1);
	bool toReturn = Do(mInput.GetAudio(), mOutput.GetData());
	mInput.Consume();
	mOutput.Produce();
	return toReturn;
}

bool FFT_fftw3::Do(const Audio& in, Spectrum &out)
{
	CLAM_DEBUG_ASSERT(IsRunning(),
		"FFT_fftw3: Do(): Not in execution mode");
	CLAM_DEBUG_ASSERT(isPowerOfTwo(mSize),
		"FFT_fftw3: Do(): Not a power of two");

	out.SetSpectralRange(in.GetSampleRate()/2);
	if (mState==sOther) CheckTypes(in,out);
	TData * inbuffer = in.GetBuffer().GetPtr();
	for (int i=0; i<mSize; i++)
		_realInput[i] = inbuffer[i];
	fftw_execute(*_plan);
	if (mState!=sOther)
		ToComplex(out);
	else
		ToOther(out);
	if (mState==sComplexSync)
		out.SynchronizeTo(mComplexflags);

	return true;
}


void FFT_fftw3::ToComplex(Spectrum &out)
{
	Array<Complex>& outbuffer = out.GetComplexArray();
	outbuffer.Resize(mSize/2+1); // TODO: Any sense?
	outbuffer.SetSize(mSize/2+1); // TODO: Any sense?
	for (int i=0; i<mSize/2+1; i++) {
		outbuffer[i].SetReal(_complexOutput[i][0]);
		outbuffer[i].SetImag(_complexOutput[i][1]);
	}
}

}; // CLAM

