/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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


#ifndef _FFT_rfftw_
#define _FFT_rfftw_

#include "FFT.hxx"
#include "DataTypes.hxx"
#include "SpecTypeFlags.hxx"
#include "ErrDynamicType.hxx"

extern "C" {
#include FFTW_HEADER
}

namespace CLAM {

	struct FFTConfig;
	class Spectrum;
	class Audio;
	class ProcessingConfig;

	/** Implementation of the FFT using the Fastest Fourier in the West 
	 * @see <a HREF="http://www.fftw.org/"> FFTW Home Page</a>
	 */
	class FFT_rfftw: public FFT_base
	{
		rfftw_plan	mpPlan;
		/** Internal output buffer */
		TData* fftbuffer;
		/** Auxiliary flags structure, used to add the complex attribute. */
		static SpecTypeFlags mComplexflags;

		/* FFT possible execution states.
		 */
		typedef enum {
			sComplex, // We just need to write the complex array.
			sComplexSync, // We write the complex array and synchronize.
			sOther // The complex array is not present.
		} FFTState;

		/** I/O Prototype state of the FFT object. */
		FFTState mState;

		/** Configuration change method
		 * @throw
		 * bad_cast exception when the argument is not an FFTConfig
		 * object.  
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

		inline void CheckTypes(const Audio& in, const Spectrum &out) const;

		// Output conversions

		/** Converts the RFFTW output format to a Complex array
		 * @param The spectrum object from which the complex array is taken. It 
		 *  must have its ComplexArray attribute instantiated.
		 * @see the <a HREF="http://www.fftw.org/doc/fftw_2.html#SEC5"> RFFTW docs </a>
		 * for a description of this format.
		 */
		inline void RFFTWToComplex(Spectrum &out) const;

		inline void RFFTWToOther(Spectrum &out) const;

	public:

		FFT_rfftw();

		FFT_rfftw(const FFTConfig &c)
			throw(ErrDynamicType);

		~FFT_rfftw();

		bool Do(void);

		void Attach(Audio& in, Spectrum &out);

		bool Do(const Audio& in, Spectrum &out) const;

		// Port interfaces.

		bool SetPrototypes(const Audio& in,const Spectrum &out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		const char* GetClassName() const
		{ 
			return "FFT_rfftw"; 
		}

	};

};//namespace CLAM

#endif // _FFT_rfftw_
