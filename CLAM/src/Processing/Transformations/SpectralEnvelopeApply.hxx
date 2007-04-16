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

#ifndef _SpectralEnvelope_Apply_
#define _SpectralEnvelope_Apply_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "SpectralPeak.hxx"
#include "SpectralPeakArray.hxx"

namespace CLAM {


	/** Configuration class for SpectralEnvelopeApply objects
	 *  @see ProcessingConfig, SpectralEnvelopeApply
	 */
	class SpectralEnvelopeApplyConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralEnvelopeApplyConfig, 0,ProcessingConfig);
 	}; 

	/**
	 * Applies an existing spectral envelope to incoming spectral peaks or spectrum
	 * @todo: the spline interpolation gets a bit out of hand in the ends of the spectral envelope
	 *	so you may get artifacts in the higher frequencies!!!
	 */
	class SpectralEnvelopeApply: public Processing {
		SpectralEnvelopeApplyConfig mConfig;

		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SpectralEnvelopeApply";}

		/** Config change method
		 *  @param The ProcessingConfig object
		 *  @pre argument should be a SpectralEnvelopeApplyConfig
		 *  @return True if the cast has been commited correctly		 
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:
		/** Base constructor of class. Calls Configure method with a SpectralEnvelopeApplyConfig initialised by default*/
		SpectralEnvelopeApply();
		/** Constructor with an object of SpectralEnvelopeApplyConfig class by parameter
		 *  @param c SpectralEnvelopeApplyConfig object created by the user
*/
		SpectralEnvelopeApply(const SpectralEnvelopeApplyConfig &c);

		/** Destructor of the class*/
 		~SpectralEnvelopeApply();

	       /** This method allows to obtain the SpectralEnvelopeApplyConfig object of the class
		*  @return The configuration of the class
		*/
		const ProcessingConfig &GetConfig() const { return mConfig;}

		/** Supervised Do() function, not implemented yet*/
		bool Do(void);

		/** Unsupervised Do function, applies an spectral envelope to incoming Spectral Peaks
		 *  @param input The SpectralPeaks we want to apply the spectral Envelope to
		 *  @param spectralEnvelope The Spectrum that holds the spectral Envelope
		 *  @param out An SpectralPeakArray object with the new SpectralEnvelope applied
		 *  @return Boolean value, true if all has been executed correctly.
		 */
		bool Do(const SpectralPeakArray& input,const Spectrum& spectralEnvelope,SpectralPeakArray& output);

		/** Unsupervised Do function, applies an spectral envelope to incoming spectrum
		 *  @param input The Spectrum we want to apply the spectral Envelope to
		 *  @param spectralEnvelope The Spectrum that holds the spectral Envelope
		 *  @param out An Spectrum object with the new SpectralEnvelope applied
		 *  @return Boolean value, true if all has been executed correctly.
		 */
		bool Do(const Spectrum& input,const Spectrum& spectralEnvelope,Spectrum& output);


		/** Change the internal type state.  
		 */

		bool SetPrototypes(const SpectralPeakArray& input,Spectrum& output);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

 	}; 

};//namespace CLAM

#endif // _SpectralEnvelope_Apply_

