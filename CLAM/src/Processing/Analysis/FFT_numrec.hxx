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


#ifndef _FFT_numrec_
#define _FFT_numrec_

#include "FFT.hxx"
#include "DataTypes.hxx"
#include "SpecTypeFlags.hxx"

namespace CLAM {

	struct FFTConfig;
	class Spectrum;
	class Audio;
	class ProcessingConfig;

	/** Implementation of the FFT using the algorithm in Numerical Recipies
	 * in C.
	 * @see <a HREF="http://www.nr.com/"> Numerical Recipies Home Page</a>
	 */
	class FFT_numrec: public FFT_base
	{
		/** Internal output buffer */
		TData* fftbuffer;
		/** Auxiliary flags structure, used to add the complex attribute. */
		static SpecTypeFlags mComplexflags;

		/* FFT possible prototype states.
		 */
		typedef enum {
			sComplex, // We just need to write the complex array.
			sComplexSync, // We write the complex array and synchronize.
			sOther // The complex array is not present.
		} FFTState;

		FFTState mState;

		/** When the object enters "Disabled" mode, it stores the
		 * previoius state here. It would have been easier to use a
		 * single state variable, and a "Disabled" flag outside of the
		 * state, but this way we can implement Do()s with a single
		 * switch level, which is slightly faster.
		 */
		FFTState mBackupState;

		bool FFTConfigure();

		inline void CheckTypes(const Audio& in, const Spectrum &out) const;

		/** Configuration change method
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

		// Output conversions

		inline void NumrecToComplex(Spectrum &out) const;

		inline void NumrecToOther(Spectrum &out) const;

	public:

		FFT_numrec();

		FFT_numrec(const FFTConfig &c) throw(ErrDynamicType);

		~FFT_numrec();

		const char * GetClassName() const {return "FFT_numrec";}

		// Execution methods

		bool Do();

		void Attach(Audio& in, Spectrum &out);

		bool Do(const Audio& in, Spectrum &out) const;

		// Port interfaces.

		bool SetPrototypes(const Audio& in, const Spectrum &out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

	};

};//namespace CLAM

#endif // _FFT_numrec_
