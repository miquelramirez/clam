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


#ifndef _IFFT_ooura_
#define _IFFT_ooura_

#include "IFFT_base.hxx"
#include "DataTypes.hxx"
#include "SpecTypeFlags.hxx"
#include "ErrDynamicType.hxx"

namespace CLAM {

	struct IFFTConfig;
	class Spectrum;
	class Audio;
	class ProcessingConfig;

	/** Implementation of the IFFT using the algorithm from Takuya OOURA
	 * in C.
	 * @see <a HREF="http://momonga.t.u-tokyo.ac.jp/~ooura/fft.html">
	 *      Ooura's IFFT Homepage</a>
	 */
	class IFFT_ooura: public IFFT_base
	{
	  /** Internal bit reversal, cos & sin tables */
		int *ip;
		TData *w;
		
		/** Internal output buffer */
		TData* ifftbuffer;
		/** Auxiliary flags structure, used to add the complex attribute. */
		SpecTypeFlags mComplexflags;

		/* IFFT possible execution states.
		*/
		typedef enum {
			sComplex, // We just need to read the complex array.
			sOther // The complex array is not present.
		} IFFTState;

		/** Execution state of the IFFT object. It includes I/O
	    prototypes state */
		IFFTState mState;
	
		bool IFFTConfigure();

		/** Configuration change method
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

		// Memory Management (for work areas and stuff)

		void ReleaseMemory();

		void SetupMemory();

		// Output conversions

		void ComplexToIFFTOoura(const Spectrum &in) const;
		void OtherToIFFTOoura(const Spectrum &in) const;

		inline void CheckTypes(const Spectrum& in, const Audio &out) const;

	public:

		IFFT_ooura();

		IFFT_ooura(const IFFTConfig &c) throw(ErrDynamicType);

		~IFFT_ooura();

		const char * GetClassName() const {return "IFFT_ooura";}

		// Execution methods

		bool Do();

		bool Do(const Spectrum& in, Audio &out) const;

		// Port interfaces.

		bool SetPrototypes(const Spectrum& in,const Audio &out);

		bool SetPrototypes();

		bool UnsetPrototypes();
		
		bool MayDisableExecution() const {return true;}

	};

};//namespace CLAM

#endif // _IFFT_numrec_

