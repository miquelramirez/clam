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


#ifndef _FFT_
#define _FFT_

#include <typeinfo> // for bad_cast definition
#include "Processing.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"
#include <string>
#include "FFTConfig.hxx"
#include "SpecTypeFlags.hxx"
#include "Spectrum.hxx"

namespace CLAM {

    class FFT_rfftw;
    class Storage;
    class ProcessingConfig;
    class Audio;
    class Spectrum;
	
    /** Default FFT implemntation. 
     * In order to use it, only the FFT.hxx file needs to be included;
     * it will include any other necesary header.
     */
    typedef FFT_rfftw FFT;

    /** Abstract base class for FFT classes */
    class FFT_base: public Processing
    {
    protected:

	/** Internal output buffer */
	TData* fftbuffer;

	/** Auxiliary flags structure, used to add the complex attribute. */
	static SpecTypeFlags mComplexflags;

	/** Auxiliary spectrum used if output spectrum does not have complex array */
	Spectrum mComplexSpectrum;
	
    /** FFT Configuration */
	FFTConfig mConfig;
	/** FFT size */
	int mSize;

	/* FFT possible execution states.
	 */
	typedef enum {
		sComplex, // We just need to write the complex array.
		sComplexSync, // We write the complex array and synchronize.
		sOther // The complex array is not present.
	} FFTState;

	/** I/O Prototype state of the FFT object. */
	FFTState mState;
	
    /** When the object enters "Disabled" mode, it stores the
	 * previoius state here. It would have been easier to use a
	 * single state variable, and a "Disabled" flag outside of the
	 * state, but this way we can implement Do()s with a single
	 * switch level, which is slightly faster.
	 */
	FFTState mBackupState;

	InPortTmpl<Audio>     mInput;
	OutPortTmpl<Spectrum> mOutput;

	// Control change callback function
	void ChangeSize(int n);
	int GetSize() {return mSize;}

	virtual bool ConcreteConfigure(const ProcessingConfig&) = 0;

    public:

	FFT_base();
	virtual ~FFT_base();

	/** Configuration access:
	 */
	const ProcessingConfig &GetConfig() const { return mConfig;}
	
	const char *GetClassName() const {return "FFT";}
	
	virtual void Attach(Audio& in, Spectrum &out);

	/** Supervised-mode Do function.
	 */
	virtual bool Do(void) = 0;

	/** Standard FFT Do function, with storage class references as
	 * arguments. This method implements the old conversor routines.
	 * The resulting spectrum will be of input audio size / 2 + 1. 
	 * Input audio must be a power-of-two.
	 */
	virtual bool Do(const Audio& in, Spectrum &out)= 0;

	// Input/Output configuration methods

	/** FFT non-supervised mode SetPrototypes function */
	bool SetPrototypes(const Audio& in,const Spectrum &out);

	/** Standard supervised-mode SetPrototypes function. */
	bool SetPrototypes() {return false;}

	/** Standard UnsetPrototypes function. */
	bool UnsetPrototypes();

	void CheckTypes(const Audio& in, const Spectrum &out) const;


	// Enable/Disable methods. Maybe we should not be deriving
	// these ones in FFT subclasses. (FFT implementations will
	// probably be always memoryless.

	virtual bool MayDisableExecution() const {return false;}

	virtual bool DisableExecution() {return false;}

	virtual bool EnableExecution() {return false;}

	// Output conversions

	virtual void ToComplex(Spectrum &out) = 0;

	void ToOther(Spectrum &out);		

    };


};//namespace CLAM

// We include the default implementation here.
#include "FFT_rfftw.hxx"

#endif // _FFT_
