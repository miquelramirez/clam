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


#ifndef MyFFT_hxx
#define MyFFT_hxx

#ifndef USE_FFTW3
#error This header should not be used without the USE_FFTW3 macro defined
#endif

#include <CLAM/Processing.hxx>
#include <CLAM/FFTConfig.hxx>
#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Audio.hxx>
#include "ComplexSpectrum.hxx"

namespace CLAM
{

	class FFTConfig;
	class ComplexSpectrum;
	class Audio;

/**
 Implementation of the FFT using the Fastest Fourier Transform in the West version 3.
 @see @link http://www.fftw.org/ FFTW Home Page @endlink

 @param[in] "Audio Buffer" [Port] An audio buffer
 @param[out] "Complex Spectrum" [Port] A complex spectrum

 @todo Document MyFFT configuration parameters
 @see MyIFFT, NewSpectralProcessing
 @ingroup SpectralProcessingAnalysis
*/
	class MyFFT: public Processing
	{
	public:
		typedef FFTConfig Config;

		MyFFT(const FFTConfig &c=FFTConfig());
		~MyFFT();
		const char * GetClassName() const {return "MyFFT";}

		// Execution methods
		bool Do();
		bool Do(const Audio& in, ComplexSpectrum &out);
		const ProcessingConfig & GetConfig() const;
	protected:
		bool ConcreteConfigure(const ProcessingConfig&);

	private:
		void SetupMemory();
		void ReleaseMemory();
	
	private:
		InPort<Audio> mInput;
		OutPort<ComplexSpectrum> mOutput;
		Config mConfig;
		unsigned mSize;
	private:
		struct Implementation;
		Implementation * _fftw3;
	};

}//namespace CLAM

#endif//MyFFT_hxx

