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
#include "ProcessingData.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"
#include "DataTypes.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include <string>

namespace CLAM {

	class FFT_rfftw;

	/** Default FFT implemntation. 
	 * In order to use it, only the FFT.hxx file needs to be included;
	 * it will include any other necesary header.
	 */
	typedef FFT_rfftw FFT;


	/** Configuration class for FFT classes
	 */
	struct FFTConfig: public ProcessingConfig {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (FFTConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, int, AudioSize);
	protected:
		/** Dynamic type initialization: All attributes are
		 *   instantiated.
		 */
		void DefaultInit(void);
	};


	/** Abstract base class for FFT classes */
	class FFT_base: public Processing
	{
	protected:
		/** FFT Configuration */
		FFTConfig mConfig;
		/** FFT size */
		int mSize;

		InPortTmpl<Audio>     mInput;
		OutPortTmpl<Spectrum> mOutput;

		const char *GetClassName() const {return "FFT";}

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

		virtual void Attach(Audio& in, Spectrum &out) = 0;

		/** Supervised-mode Do function.
		 */
		virtual bool Do(void) = 0;

		/** Standard FFT Do function, with storage class references as
		 * arguments. This method implements the old conversor routines.
		 * The resulting spectrum will be of input audio size / 2 + 1. 
		 * Input audio must be a power-of-two.
		 */
		virtual bool Do(const Audio& in, Spectrum &out) const = 0;

		// Input/Output configuration methods

		/** FFT non-supervised mode SetPrototypes function */
		virtual bool SetPrototypes(const Audio& in,const Spectrum &out) = 0;

		/** Standard supervised-mode SetPrototypes function. */
		virtual bool SetPrototypes() {return false;}

		/** Standard UnsetPrototypes function. */
		virtual bool UnsetPrototypes() {return false;}

		// Enable/Disable methods. Maybe we should not be deriving
		// these ones in FFT subclasses. (FFT implementations will
		// probably be always memoryless.

		virtual bool MayDisableExecution() const {return false;}

		virtual bool DisableExecution() {return false;}

		virtual bool EnableExecution() {return false;}

		// COMPONENT Methods

		/** @todo Not yet implemented. */
		virtual void StoreOn(Storage &s) {};
		/** @todo Not yet implemented. */
		virtual void LoadFrom(Storage &s) {};

	};
};//namespace CLAM

// We include the default implementation here.
#include "FFT_rfftw.hxx"

#endif // _FFT_
