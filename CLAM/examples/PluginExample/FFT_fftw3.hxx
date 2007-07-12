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


#ifndef FFT_fftw3_hxx
#define FFT_fftw3_hxx

#ifndef USE_FFTW3
#error This header should not be used without the USE_FFTW3 macro defined
#endif

#include "FFT_base.hxx"
#include "DataTypes.hxx"
#include "SpecTypeFlags.hxx"
#include "ErrDynamicType.hxx"

namespace CLAM
{

	class FFTConfig;
	class Spectrum;
	class Audio;
	namespace Hidden { struct FFT_fftw3_Implementation; }

	/** Implementation of the FFT using the Fastest Fourier Transform of the West version 3
	 */
	class FFT_fftw3: public FFT_base
	{
	public:
		FFT_fftw3(const FFTConfig &c=FFTConfig());
		~FFT_fftw3();
		const char * GetClassName() const {return "FFT_fftw3";}

		// Execution methods
		bool Do();
		bool Do(const Audio& in, Spectrum &out);
	protected:
		bool ConcreteConfigure(const ProcessingConfig&);

	private:
		Hidden::FFT_fftw3_Implementation * _fftw3;
	private:
		// Memory Management (for work areas and stuff)
		void ReleaseMemory();
		void SetupMemory();

		// Output conversions
		void ToComplex(Spectrum &out);
	};

}//namespace CLAM

#endif//FFT_fftw3_hxx

