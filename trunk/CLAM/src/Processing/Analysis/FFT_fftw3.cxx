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
#include "ProcessingFactory.hxx"
#include <fftw3.h>

namespace CLAM 
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "FFT_fftw3",
		"category", "Analysis",
		"description", "FFT_fftw3",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, FFT_fftw3> regFFT_fftw3("FFT_fftw3");
	static FactoryRegistrator<ProcessingFactory, FFT_fftw3> reg = metadata;
}

namespace Hidden
{
	struct FFT_fftw3_Implementation
	{
		FFT_fftw3_Implementation(unsigned size)
		{
			// Special malloc which aligns to SIMD segment boundaries
			realInput = (double*) fftw_malloc(sizeof(double) * size);
			complexOutput = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
			fftw_import_system_wisdom();
			plan = fftw_plan_dft_r2c_1d(size, realInput, complexOutput, FFTW_ESTIMATE);
		}
		~FFT_fftw3_Implementation()
		{
			fftw_destroy_plan(plan);
			fftw_free(realInput);
			fftw_free(complexOutput);
		}
		double * realInput;
		fftw_complex * complexOutput;
		fftw_plan plan;
	};
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

void FFT_fftw3::ReleaseMemory()
{
	if (_fftw3) delete _fftw3;
}

void FFT_fftw3::SetupMemory()
{
	ReleaseMemory();
	_fftw3 = new Hidden::FFT_fftw3_Implementation(mSize);
}

FFT_fftw3::FFT_fftw3(const FFTConfig &c)
	: _fftw3(0)
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
	CLAM_BEGIN_DEBUG_CHECK
		CLAM_WARNING(isPowerOfTwo(mSize),
			"FFT_fftw3: Do(): Not a power of two");
	CLAM_END_DEBUG_CHECK

	out.SetSpectralRange(in.GetSampleRate()/2);
	if (mState==sOther) CheckTypes(in,out);
	TData * inbuffer = in.GetBuffer().GetPtr();
	for (int i=0; i<mSize; i++)
		_fftw3->realInput[i] = inbuffer[i];
	fftw_execute(_fftw3->plan);
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
	for (int i=0; i<mSize/2+1; i++)
	{
		outbuffer[i].SetReal(_fftw3->complexOutput[i][0]);
		outbuffer[i].SetImag(_fftw3->complexOutput[i][1]);
	}
}

}; // CLAM

