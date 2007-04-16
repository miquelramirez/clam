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


#ifndef _IFFT_base_
#define _IFFT_base_

#include <typeinfo> // for bad_cast definition
#include "Processing.hxx"
#include "InPort.hxx"
#include "AudioOutPort.hxx"
#include "ProcessingData.hxx"
#include "DataTypes.hxx"
#include "Spectrum.hxx"

#include "IFFTConfig.hxx"

namespace CLAM {

    class Audio;
    class Spectrum;

    /** Abstract base class for IFFT classes */
    class IFFT_base: public Processing
    {
    protected:
	enum {CLAM_DEFAULT_IFFT_SIZE=1024};
		
	/** IFFT Configuration */
	IFFTConfig mConfig;
	/** IFFT size */
	int mSize;

	InPort<Spectrum> mInput;
	AudioOutPort   mOutput;

	// Control change callback function
	void ChangeSize(int n);
	int GetSize() {return mSize;}
		
	virtual bool ConcreteConfigure(const ProcessingConfig&) = 0;

    public:

	IFFT_base();

	virtual ~IFFT_base();

	const char *GetClassName() const {return "IFFT";}
		
	/** Config access:
	 */
	virtual const ProcessingConfig &GetConfig() const { return mConfig;}

	/** Supervised-mode Do function.
	 */
	virtual bool Do(void) = 0;

	/** Standard IFFT Do function, with storage class references as
	 * arguments. This method implements the old conversor routines.
	 */
	virtual bool Do(const Spectrum& in, Audio &out) const = 0;

	// Input/Output configuration methods

	/** IFFT non-supervised mode SetPrototypes function */
	virtual bool SetPrototypes(const Spectrum& in,const Audio &out) = 0;

	/** Standard supervised-mode SetPrototypes function. */
	virtual bool SetPrototypes() {return false;}

	/** Standard UnsetPrototypes function. */
	virtual bool UnsetPrototypes() {return false;}

	// Enable/Disable methods. Maybe we should not be deriving
	// these ones in IFFT subclasses. (IFFT implementations will
	// probably be always memoryless.

	virtual bool MayDisableExecution() const {return false;}

    };
}

#endif // _IFFT_base_

