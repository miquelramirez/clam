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

#ifndef _SpectralPeakDetect_
#define _SpectralPeakDetect_

#include "Processing.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "SpectralPeakDetectConfig.hxx"
#include "Spectrum.hxx"

namespace CLAM
{

	class SpectralPeakArray;
	class Storage;
	class ProcessingConfig;

	/**
	 * This class is dedicated to find magnitude SpectralPeaks in a given spectral frame.
	 * The spectral frame has to be in phase/magnitudes (in dB's) format for the algorithm.
	 * <p>
	 * In order to get a better frequency solution, the precise SpectralPeakBinPosition 
	 * is interpolated. The Magnitude at this interpolated BinPos is calculated a 3-point 
	 * curve-fit, phase is interpolated linearly.
	 * <p>
	 * The type of the  output is  SpectralPeakArray
	 */
	class SpectralPeakDetect: public Processing {

	protected: // attributes
		SpectralPeakDetectConfig mConfig;

		InPort< Spectrum >           mInput;
		OutPort< SpectralPeakArray > mOutput;

	protected: // methods
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SpectralPeakDetect";}

		/** Config change method
		 *  @param The ProcessingConfig object
		 *  @pre the argument should be an SpectralPeakDetectConfig object.
		 *  @return True if the cast has been commited correctly		 
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:
		/** Base constructor of class. Calls Configure method with a SpectralPeakDetectConfig initialised by default*/
		SpectralPeakDetect();
		/** Constructor with an object of SpectralPeakDetectConfig class by parameter
		 *  @param c SpectralPeakDetectConfig object created by the user
		*/
		SpectralPeakDetect(const SpectralPeakDetectConfig &c);

		/** Destructor of the class*/
 		virtual ~SpectralPeakDetect();


	       /** This method allows to obtain the SpectralPeakDetectConfig object of the class
		*  @return The configuration of the class
		*/
		const ProcessingConfig &GetConfig() const { return mConfig;}

		/** Supervised Do() function */
		bool Do(void);

		/** Unsupervised Do function, receives an Spectrum object input, and the output of method is an SpectralPeakArray (passed by parameter)
		 *  @param input The spectrum we want to get the SpectralPeaks
		 *  @param out An SpectralPeakArray object where will be setted the SpectralPeaks of Spectrum in.
		 *  @return Boolean value, true if all has been executed correctly.
		 */
		bool Do(const Spectrum& input, SpectralPeakArray& out);

		/** Change the internal type state.  
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(Spectrum& inputs,const SpectralPeakArray& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

	private:
		/** Not implemented yet*/
		inline void Detect(Spectrum& inputs, Spectrum& out);

		bool CheckInputType(const Spectrum &in);
		bool CheckOutputType(const SpectralPeakArray &out);
		
		Spectrum mTmpLinearInSpectrum;
 	}; 

} //namespace CLAM

#endif // _SpectralPeak_Detect_

