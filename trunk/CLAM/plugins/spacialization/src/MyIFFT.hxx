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


#ifndef _MyIFFT_
#define _MyIFFT_

#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include "ComplexSpectrum.hxx"

namespace CLAM {

/**
 Fast Fourier Transform that takes a complex spectrum (n/2+1 bins)
 and returns an audio buffer  of n samples.
 Implementation uses the Fastest Fourier Tranform in the West version 3
 to perfrom a complex to real IDFT.
 @see @link http://www.fftw.org/ FFTW Home Page @endlink
 
 @param[in] "Complex Spectrum" [Port] A complex spectrum
 @param[out] "Audio Buffer" [Port] An audio buffer
 @param[in] "AudioSize" [Config] The outgoing audio size (fftsize 
 @todo Document MyIFFT configuration parameters

 @see MyFFT, AudioBuffer2Stream, NewSpectralProcessing
 @ingroup SpectralProcessingAnalysis
*/
	class MyIFFT: public Processing
	{
		InPort<ComplexSpectrum> mInput;
		OutPort<Audio> mOutput;
	public:
		class Config : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE (Config, 1, ProcessingConfig);
			DYN_ATTRIBUTE (0, public, int, AudioSize);
			void DefaultInit()
			{
				AddAll();
				UpdateData();
				SetAudioSize(1024);
			}
			~Config();	
		};
	private:
		Config mConfig;
		unsigned _size;

		/* IFFT possible execution states.
		 */
		typedef enum {
			sComplex, // We just need to read the complex array.
			sOther // The complex array is not present.
		} IFFTState;

		/** Execution state of the IFFT object. It includes I/O
		    prototypes state */
		IFFTState mState;

		void DefaultInit();

		void ReleaseMemory();
		void SetupMemory();

		bool ConcreteConfigure(const ProcessingConfig&);
		const ProcessingConfig & GetConfig() const { return mConfig; }

	public:

		MyIFFT(const Config &c=Config());
		~MyIFFT();
		const char * GetClassName() const {return "MyIFFT";}

		bool Do();
		bool Do(const ComplexSpectrum& in, Audio &out) const;
	private:
		struct Implementation;
		Implementation * _fftw3;

	};

}

#endif // _MyIFFT_

