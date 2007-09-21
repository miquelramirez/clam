/*
 * Copyright (c) 2007 Fundació Barcelona Media Universitat Pompeu Fabra
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


#include "MyFFT.hxx"

#include <CLAM/CLAM_Math.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include <fftw3.h>

namespace CLAM 
{

namespace Hidden
{
	static const char* metadata[] = {
		"key", "MyFFT",
		"category", "Spectral Processing",
		"description", "MyFFT",
		0
	};
	static FactoryRegistrator<ProcessingFactory, MyFFT> reg = metadata;
}

struct MyFFT::Implementation
{
	Implementation(unsigned size)
	{
		// Special malloc which aligns to SIMD segment boundaries
		realInput = (double*) fftw_malloc(sizeof(double) * size);
		complexOutput = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
		fftw_import_system_wisdom();
		plan = fftw_plan_dft_r2c_1d(size, realInput, complexOutput, FFTW_ESTIMATE);
	}
	~Implementation()
	{
		fftw_destroy_plan(plan);
		fftw_free(realInput);
		fftw_free(complexOutput);
		fftw_forget_wisdom();	
	}
	double * realInput;
	fftw_complex * complexOutput;
	fftw_plan plan;
};

bool MyFFT::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig,c);
	unsigned int configSize = mConfig.GetAudioSize();
	if (configSize<=0) 
	{
		AddConfigErrorMessage("Invalid zero or negative input size");
		return false;
	}
	if (!isPowerOfTwo(configSize))
	{
		AddConfigErrorMessage("Size is not a power of two");
		return false;
	}
	if (mSize == configSize) return true;
	mSize=configSize;
	SetupMemory();
	return true;
}
const ProcessingConfig & MyFFT::GetConfig() const
{
	return mConfig;
}

void MyFFT::ReleaseMemory()
{
	if (_fftw3) delete _fftw3;
}

void MyFFT::SetupMemory()
{
	ReleaseMemory();
	_fftw3 = new MyFFT::Implementation(mSize);
}

MyFFT::MyFFT(const FFTConfig &c)
	: mInput("Audio Buffer", this)
	, mOutput("Complex Spectrum", this)
	, mSize(0)
	, _fftw3(0)
{ 
	Configure(c);
}

MyFFT::~MyFFT()
{
	ReleaseMemory();
}

bool MyFFT::Do() 
{
	bool toReturn = Do(mInput.GetData(), mOutput.GetData());
	mInput.Consume();
	mOutput.Produce();
	return toReturn;
}

bool MyFFT::Do(const Audio& in, ComplexSpectrum &out)
{
	CLAM_DEBUG_ASSERT(IsRunning(),
		"MyFFT: Do(): Not in execution mode");

	TData * inbuffer = in.GetBuffer().GetPtr();
	for (int i=0; i<mSize; i++)
		_fftw3->realInput[i] = inbuffer[i];

	fftw_execute(_fftw3->plan);

	out.spectralRange = in.GetSampleRate()/2;
	const unsigned spectrumSize = mSize/2+1;
	out.bins.resize(spectrumSize);
	for (int i=0; i<spectrumSize; i++)
	{
		out.bins[i]=std::complex<TData>(
			_fftw3->complexOutput[i][0],
			_fftw3->complexOutput[i][1]);
	}
	return true;
}


}; // CLAM

