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


#ifndef _MyIFFT_
#define _MyIFFT_

#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/Spectrum.hxx>
#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/IFFTConfig.hxx>
#include "ComplexSpectrum.hxx"

namespace CLAM {

	/** Implementation of the IFFT using the Fastest Fourier in the West version 3
	* @see <a HREF="http://www.fftw.org/"> FFTW Home Page</a>
	*/
	class MyIFFT: public Processing
	{
		InPort<ComplexSpectrum> mInput;
		OutPort<Audio> mOutput;
		typedef IFFTConfig Config;
		Config mConfig;
		unsigned mSize;

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

		MyIFFT(const IFFTConfig &c=IFFTConfig());
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

