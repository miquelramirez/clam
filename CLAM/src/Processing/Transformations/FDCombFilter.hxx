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


	/** Configuration class for FDCombFilter objects
	 *  @see ProcessingConfig, FDCombFilter
	 */
	class FDCombFilterConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (FDCombFilterConfig, 0,ProcessingConfig);
 	}; 

	/**
	 *	Basid Frequency domain Comb Filter. It has one control (mFreq) which sets the fundamental of
	 *	harmonic series that wants to be amplified.
	 *	@todo: add a gain control to be able to filter out harmonic series instead of amplifying it.
	 *  @todo: don't know why it does not work correctly with spectrums that have less than 512 bins !!
	 */
	class FDCombFilter: public Processing {
		FDCombFilterConfig mConfig;

		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "FDCombFilter";}

		/** Config change method
		 *  @param The ProcessingConfig object
		 *  @pre argument should be a FDCombFilterConfig.
		 *  @return True if the cast has been commited correctly		 
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:
		/** Base constructor of class. Calls Configure method with a FDCombFilterConfig initialised by default*/
		FDCombFilter();
		/** Constructor with an object of FDCombFilterConfig class by parameter
		 *  @param c FDCombFilterConfig object created by the user
		 */
		FDCombFilter(const FDCombFilterConfig &c);

		/** Destructor of the class*/
 		~FDCombFilter();

	       /** This method allows to obtain the FDCombFilterConfig object of the class
		*  @return The configuration of the class
		*/
		const ProcessingConfig &GetConfig() const { return mConfig;}

		/** Supervised Do() function, not implemented yet*/
		bool Do(void);

		/** Unsupervised Do function, applies an spectral envelope to incoming spectrum
		 *  @param input The Spectrum we want to filter
		 *  @param output The filtered spectrum
		 *  @return Boolean value, true if all has been executed correctly.
		 */
		bool Do(const Spectrum& input,Spectrum& output);


		/** Change the internal type state.  
		 */

		bool SetPrototypes(const Spectrum& input,Spectrum& output);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		InControlTmpl<FDCombFilter> mFreq;

	
		
 	}; 

};//namespace CLAM

#endif // _FDCombFilter_

