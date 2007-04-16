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


#ifndef _FFT_fftw3
#define _FFT_fftw3

#ifndef USE_FFTW3
#error This header should not be used without the USE_FFTW3 macro defined
#endif

#include "FFT_base.hxx"
#include "DataTypes.hxx"
#include "SpecTypeFlags.hxx"
#include "ErrDynamicType.hxx"
#include <fftw3.h>

namespace CLAM {

	struct FFTConfig;
	class Spectrum;
	class Audio;
	class ProcessingConfig;
	class IFFT_fftw3;

	/** Implementation of the FFT using the Fastest Fourier Transform of the West version 3
	 */
	class FFT_fftw3: public FFT_base
	{
		double * _realInput;
		fftw_complex * _complexOutput;
		fftw_plan * _plan;

//		bool FFTConfigure();

		/** Configuration change method
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

		// Memory Management (for work areas and stuff)
		void ReleaseMemory();
		void SetupMemory();

		// Output conversions

		void ToComplex(Spectrum &out);

	public:

		FFT_fftw3();
		FFT_fftw3(const FFTConfig &c) throw(ErrDynamicType);
		~FFT_fftw3();
		const char * GetClassName() const {return "FFT_fftw3";}

		// Execution methods
		bool Do();
		bool Do(const Audio& in, Spectrum &out);
		bool MayDisableExecution() const {return true;}

	};

};//namespace CLAM

#endif // _FFT_numrec_

