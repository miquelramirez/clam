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

#ifndef _SpectralEnvelope_Extract_
#define _SpectralEnvelope_Extract_

#include "Processing.hxx"
#include "SpectralEnvelopeExtractConfig.hxx"
#include "BPF.hxx"

namespace CLAM {

	class Spectrum;
	class SpectralPeakArray;
	class Storage;
	class ProcessingConfig;

	/** Configuration class for SpectralEnvelopeExtract objects
	 *  @see ProcessingConfig, SpectralEnvelopeExtract
	 */
	/* SpectralEnvelopeExtractConfig moved to SpectralEnvelopeExtractConfig.hxx */


	/**
	 *	Processing to extract spectral envelope from an array of spectral peaks.
	 */
	class SpectralEnvelopeExtract: public Processing {
		SpectralEnvelopeExtractConfig mConfig;

		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SpectralEnvelopeExtract";}

		/** Config change method
		 *  @param The ProcessingConfig object
		 *  @pre argument should be an SpectralEnvelopeExtractConfig object.
		 *  @return True if the cast has been commited correctly		 
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:
		/** Base constructor of class. Calls Configure method with a SpectralEnvelopeExtractConfig initialised by default*/
		SpectralEnvelopeExtract();
		/** Constructor with an object of SpectralEnvelopeExtractConfig class by parameter
		 *  @param c SpectralEnvelopeExtractConfig object created by the user
*/
		SpectralEnvelopeExtract(const SpectralEnvelopeExtractConfig &c);

		/** Destructor of the class*/
 		~SpectralEnvelopeExtract();

	       /** This method allows to obtain the SpectralEnvelopeExtractConfig object of the class
		*  @return The configuration of the class
		*/
		const ProcessingConfig &GetConfig() const { return mConfig;}

		/** Supervised Do() function, not implemented yet*/
		bool Do(void);

		/** Unsupervised Do function, receives an Spectrum object input, and the output of method is an SpectralPeakArray (passed by parameter)
		 *  @param input The Spectral Peaks we want to extract the envelope from
		 *  @param output A Spectrum in BPF form that represents the Spectral Envelope
		 *  @return Boolean value, true if all has been executed correctly.
		 */
		bool Do(const SpectralPeakArray& input,Spectrum& output);

		
		/** Change the internal type state.  
		 */

		bool SetPrototypes(const SpectralPeakArray& input,Spectrum& output);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

	private:
		
		/** Checks if the output type has the correct size and attributes .
		 *  If not, resize Buffers and changes control flags to set correctly the object
		 *  @param out SpectrumSpectralPeakArray object to be checked
		 *  @return true if success
		 */
		bool CheckOutputType(Spectrum& out);
		
		//member BPF's used for initialization
		BPF mMagBPF;
		BPF mPhaseBPF;
		
 	}; 

};//namespace CLAM

#endif // _SpectralEnvelope_Extract_

