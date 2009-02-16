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

#ifndef _FDCombFilter_
#define _FDCombFilter_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "InControl.hxx"

namespace CLAM {


	/**
	 *	Basic Frequency domain Comb Filter. It has one control (mFreq) which sets the fundamental of
	 *	harmonic series that wants to be amplified.
	 *	@todo: add a gain control to be able to filter out harmonic series instead of amplifying it.
	 *  @todo: don't know why it does not work correctly with spectrums that have less than 512 bins !!
	 */
	class FDCombFilter: public Processing {

	public:
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "FDCombFilter";}

		/** Constructor with an object of FDCombFilterConfig class by parameter
		 *  @param c FDCombFilterConfig object created by the user
		 */
		FDCombFilter(const Config &c = Config());

		/** Destructor of the class*/
 		~FDCombFilter();

		/** Supervised Do() function, not implemented yet*/
		bool Do();

		/** Unsupervised Do function, applies an spectral envelope to incoming spectrum
		 *  @param input The Spectrum we want to filter
		 *  @param output The filtered spectrum
		 *  @return Boolean value, true if all has been executed correctly.
		 */
		bool Do(const Spectrum& input,Spectrum& output);

		FloatInControl mFreq;

	
		
 	}; 

};//namespace CLAM

#endif // _FDCombFilter_

