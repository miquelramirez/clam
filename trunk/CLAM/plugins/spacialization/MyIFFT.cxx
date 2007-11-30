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


#include "MyIFFT.hxx"
#include <CLAM/SpecTypeFlags.hxx>

#include <CLAM/ProcessingFactory.hxx>

#include <fftw3.h>


namespace CLAM
{

namespace Hidden
{
	static const char* metadata[] = {
		"key", "MyIFFT",
		"category", "New Spectral Processing",
		"description", "MyIFFT",
		0
	};
	static FactoryRegistrator<ProcessingFactory, MyIFFT> reg = metadata;
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
	: mInput("Complex Spectrum", this)
	, mOutput("Audio Buffer", this)
	, _fftw3(0)
{ 
	Configure(c);
};

bool MyIFFT::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	if (mConfig.GetAudioSize()<=0)
	{
		AddConfigErrorMessage("IFFT size should be greater than 0");
		return false;
	}
	mSize = mConfig.GetAudioSize();	
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


bool MyIFFT::Do()
{
	bool toReturn = Do(mInput.GetData(),mOutput.GetData());
	mInput.Consume();
	mOutput.Produce();
	return toReturn;
};


bool MyIFFT::Do( const ComplexSpectrum& in, Audio &out) const
{
	CLAM_ASSERT(IsRunning() ,"MyIFFT: Do(): Not in execution mode");
	const std::vector<std::complex<TData> > & inbuffer = in.bins;
	CLAM_ASSERT(inbuffer.size() == mSize/2+1, "MyIFFT: input data size doesn't match configuration.");
	for (int i=0; i< inbuffer.size(); i++)
	{
		_fftw3->_complexInput[i][0] = std::real(inbuffer[i])/mSize;
		_fftw3->_complexInput[i][1] = std::imag(inbuffer[i])/mSize;
	}

	fftw_execute(_fftw3->_plan);

	out.SetSampleRate(TData(in.spectralRange*2));
	out.SetSize(mSize);
	TData * outbuffer = out.GetBuffer().GetPtr();
	for (int i=0; i<mSize; i++)
		outbuffer[i] = _fftw3->_realOutput[i];
	return true;
}

}

