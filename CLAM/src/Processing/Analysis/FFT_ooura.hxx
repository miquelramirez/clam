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


#ifndef _FFT_ooura_
#define _FFT_ooura_

#include "FFT.hxx"
#include "DataTypes.hxx"
#include "SpecTypeFlags.hxx"

namespace CLAM {

	struct FFTConfig;
	class Spectrum;
	class Audio;
	class ProcessingConfig;

	/** Implementation of the FFT using the algorithm from Takuya OOURA
	 * in C.
	 * @see <a HREF="http://momonga.t.u-tokyo.ac.jp/~ooura/fft.html">
	 *      Ooura's FFT Homepage</a>
	 */
	class FFT_ooura: public FFT_base
	{
	  /** Internal bit reversal, cos & sin tables */
		TData *w;
		int *ip;

		bool FFTConfigure();

		/** Configuration change method
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

		// Memory Management (for work areas and stuff)

		void ReleaseMemory();

		void SetupMemory();

		// Output conversions

		void ToComplex(Spectrum &out);

		// FFTOOURA original functions, modified to accept
		// TData instead of double
		void rdft(int n, int isgn, TData *a, int *ip, TData *w) const;
		void makewt(int nw, int *ip, TData *w) const;
		void makect(int nc, int *ip, TData *c) const;
		void bitrv2(int n, int *ip, TData *a) const;
		void cftfsub(int n, TData *a, TData *w) const;
		void cftbsub(int n, TData *a, TData *w) const;
		void rftfsub(int n, TData *a, int nc, TData *c) const;
		void rftbsub(int n, TData *a, int nc, TData *c) const;
		void cft1st(int n, TData *a, TData *w) const;
		void cftmdl(int n, int l, TData *a, TData *w) const;

	public:

		FFT_ooura();

		FFT_ooura(const FFTConfig &c) throw(ErrDynamicType);

		~FFT_ooura();

		const char * GetClassName() const {return "FFT_ooura";}

		// Execution methods

		bool Do();

		bool Do(const Audio& in, Spectrum &out);

		
		bool MayDisableExecution() const {return true;}

	};

};//namespace CLAM

#endif // _FFT_numrec_
