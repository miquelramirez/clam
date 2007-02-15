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


#ifndef _IFFT_rfftw_
#define _IFFT_rfftw_

#include "IFFT_base.hxx"
#include "ErrDynamicType.hxx"
#include "SpectrumConfig.hxx"

#ifdef CLAM_DOUBLE
#define FFTW_HEADER <rfftw.h>
#else
#ifdef WIN32
#define FFTW_HEADER <rfftw.h>
#else
#define FFTW_HEADER <srfftw.h>
#endif
#endif

extern "C" {
#if !defined(WIN32) && !defined(CLAM_DOUBLE)
	#include <srfftw.h>
#else
	#include <rfftw.h>
#endif
}

namespace CLAM {

    

    /** Implementation of the IFFT using the Fastest Fourier in the West 
     * @see <a HREF="http://www.fftw.org/"> FFTW Home Page</a>
     */
    class IFFT_rfftw: public IFFT_base
    {
	rfftw_plan	mpPlan;
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

	void DefaultInit();

	inline void CheckTypes(const Spectrum& in, const Audio &out) const;

	// Output conversions

	/** Converts a complex array to the RIFFTW input format.
	 * @param The spectrum object from which the complex array is taken. It 
	 *  must have its ComplexArray attribute instantiated.
	 * @see the <a HREF="http://www.fftw.org/doc/fftw_2.html#SEC5"> RFFTW docs </a>
	 * for a description of this format.
	 */
	inline void ComplexToRIFFTW(Spectrum &in) const;

	inline void OtherToRIFFTW(Spectrum &in) const;

	/** Configuration change method
	 * @pre argument should be a IFFTConfig
	 */
	bool ConcreteConfigure(const ProcessingConfig&);

    public:

	IFFT_rfftw();

	IFFT_rfftw(const IFFTConfig &c) throw(ErrDynamicType);

	~IFFT_rfftw();

	const char * GetClassName() const {return "IFFT_rfftw";}

	bool Do();

	bool Do(const Spectrum& in, Audio &out) const;

	// Port interfaces.

	bool SetPrototypes(const Spectrum& in,const Audio &out);

	bool SetPrototypes();

	bool UnsetPrototypes();

	bool MayDisableExecution() const {return true;}

    };

}

#endif // _IFFT_rfftw_

