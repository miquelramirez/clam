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

#ifndef _SpectralPeak_DETECT_
#define _SpectralPeak_DETECT_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "SpectralPeak.hxx"
#include "SpectralPeakArray.hxx"

namespace CLAM {


	/** Configuration class for SpectralPeakDetect objects
	 *  @see ProcessingConfig, SpectralPeakDetect
	 */
	class SpectralPeakDetectConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralPeakDetectConfig, 6,ProcessingConfig);
		/** Name of the SpectralPeakDetect object*/
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/** Maximum namber of SpectralPeaks*/
		DYN_ATTRIBUTE (1, public, TSize, MaxPeaks);
		/** Threshold of magnitude; SpectralPeaks will be located above this value*/
		DYN_ATTRIBUTE (2, public, TSize, MagThreshold);
		/** Number of bands (Spectrum size) */
		DYN_ATTRIBUTE (3, public, TSize, NumBands);
		/** Type of Magnitude interpolation between bins*/
		DYN_ATTRIBUTE (4, public, EInterpolation, InterpolMag);
		/** Type of Phase interpolation between bins */
		DYN_ATTRIBUTE (5, public, EInterpolation, InterpolPhase);
	protected:
		void DefaultInit();
	public:
	  /** Initialize configuration object with default values (name 'SpectralPeakDetect', 513 bands, Magnitude treshold set to -80 and MaxSpectralPeaks setted to 100
	   */
		void DefaultValues();
		~SpectralPeakDetectConfig(){};
		
 	}; 

	/**
	 * This class is dedicated to find magnitude SpectralPeaks in a given spectral frame.
	 * The spectral frame has to be in MagPhaseBPF format for the algorithm. All 
	 * other formats will be converted and will therefore run slower.
	 * <p>
	 * In order to get a better frequency solution, the precise SpectralPeakBinPosition 
	 * is interpolated. The Magnitude at this interpolated BinPos is calculated 
	 * by using the second order interpolation of the BPF type, phase is 
	 * interpolated linearly.
	 * <p>
	 * The type of the  output is  SpectralPeakArray
	 *
	 */
	class SpectralPeakDetect: public Processing {
		SpectralPeakDetectConfig mConfig;

		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SpectralPeakDetect";}

		/** Config change method
		 *  @param The ProcessingConfig object
		 *  @throw bad_cast exception when the argument is not an SpectralPeakDetectConfig object.
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
 		~SpectralPeakDetect();

	       /** This method allows to obtain the SpectralPeakDetectConfig object of the class
		*  @return The configuration of the class
		*/
		const ProcessingConfig &GetConfig() const { return mConfig;}

		/** Supervised Do() function, not implemented yet*/
		bool Do(void);

		/** Unsupervised Do function, receives an Spectrum object input, and the output of method is an SpectralPeakArray (passed by parameter)
		 *  @param input The spectrum we want to get the SpectralPeaks
		 *  @param out An SpectralPeakArray object where will be setted the SpectralPeaks of Spectrum in.
		 *  @return Boolean value, true if all has been executed correctly.
		 */
		bool Do(Spectrum& input, SpectralPeakArray& out);

		/** Change the internal type state.  
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(Spectrum& inputs,const SpectralPeakArray& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		/** Not implemented yet*/
		void StoreOn(Storage &s) {};

	private:
		/** Not implemented yet*/
		inline void Detect(Spectrum& inputs, Spectrum& out);

		/** Checks if the output type has the correct size and attributes .
		 *  If not, resize Buffers and changes control flags to set correctly the object
		 *  @param out SpectrumSpectralPeakArray object to be checked
		 *  @return true if success
		 */
		bool CheckOutputType(SpectralPeakArray& out);
		
 	}; 

};//namespace CLAM

#endif // _SpectralPeak_Detect_
