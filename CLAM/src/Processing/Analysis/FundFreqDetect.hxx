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

#ifndef _FUNDFREQ_DETECT__
#define _FUNDFREQ_DETECT__

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "SpectralPeak.hxx"
#include "SpectralPeakArray.hxx"
#include "Fundamental.hxx"

namespace CLAM {

	class FundFreqDetectConfig: public ProcessingConfig
	{
	public:
	  DYNAMIC_TYPE_USING_INTERFACE (FundFreqDetectConfig, 22,ProcessingConfig);
	  DYN_ATTRIBUTE (0, public, std::string, Name);
	  DYN_ATTRIBUTE (1,public,TData, referenceFundFreq); 
	  DYN_ATTRIBUTE (2,public,TData, lowestFundFreq); 
	  DYN_ATTRIBUTE (3,public,TData, highestFundFreq); 
	  DYN_ATTRIBUTE (4,public,TData, centralBandLF); 
	  DYN_ATTRIBUTE (5,public,TData, centralBandHF); 
	  DYN_ATTRIBUTE (6,public,TData, maxPeakMagDiff); 			
	  DYN_ATTRIBUTE (7,public,TData, maxCandMagDiff); 
	  DYN_ATTRIBUTE (8,public,TData, minBinWidth); 
	  DYN_ATTRIBUTE (9,public,TData, maxFundFreqError); 				 
	  DYN_ATTRIBUTE (10,public,TData, PMp); 
	  DYN_ATTRIBUTE (11,public,TData, PMq );
	  DYN_ATTRIBUTE (12,public,TData, PMr );
	  DYN_ATTRIBUTE (13,public,TData, MPp );
	  DYN_ATTRIBUTE (14,public,TData, MPq );
	  DYN_ATTRIBUTE (15,public,TData, MPr);
	  DYN_ATTRIBUTE (16,public,TSize, PMnPeaks); 
	  DYN_ATTRIBUTE (17,public,TSize, MPnPeaks);
	  DYN_ATTRIBUTE (18,public,TData, PMCont );
	  DYN_ATTRIBUTE (19,public,TData, MPCont );
	  DYN_ATTRIBUTE (20,public,TSize, nInt);
	  DYN_ATTRIBUTE (21, public,TSize, nMaxCandidates); 
	  void DefaultInit();
	  void DefaultValues();
	  ~FundFreqDetectConfig(){};
	};

  /**
   *
   *     This class performs a fundamental frequency detection algorithm   
   *     in the spectral domain for one single spectralFrame                                           
   *     The number of candidates is given by the CCLAMFundFreq object       
   *
   */


	class FundFreqDetect: public Processing {
		mutable FundFreqDetectConfig mConfig;

		const char *GetClassName() {return "FundFreqDetect";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

	public:
		FundFreqDetect();

		FundFreqDetect(const FundFreqDetectConfig &c);

		~FundFreqDetect();

		const ProcessingConfig &GetConfig() const;

		bool Do(void);

		bool Do(SpectralPeakArray& input,Fundamental& fund);

		// Port interfaces.

		/** Change the internal type state.  
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(const SpectralPeakArray& inputs);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};

	private:

		double WeightCandidate(double freq, double maxMag,SpectralPeakArray&) const;
		int GetClosestPeak(double freq, int peak,SpectralPeakArray& peaks) const;
		double GetClosestHarmonic(double peak, double fundfreq) const;
		bool IsGoodCandidate(double freq) const;

  /* Maximum number of candidates*/
		TData mnMaxCandidates;
	/* Analysis Parameters */
		TData mReferenceFundFreq;
		TData mLowestFundFreq;
		TData mHighestFundFreq;
	  
		/* Frequencies in central band: more probability of finding the fundamental frequency */
		TData mCentralBandLF; // 1000 Hz for singing voice 
		TData mCentralBandHF; // 3000 Hz for singing voice

		/* Magnitude maximun difference between peaks*/
		TData mMaxPeakMagDiff; // Max magnitude offset allowed between peaks (50 dB default)
		TData mMaxCandMagDiff; // Max magnitude offset allowed between candidates (30 dB default)

		TData mMinBinWidth; // Min binwidth required for peak (2 by default)
		TData mMaxFundFreqError; // Maximun error accepted
		TSize mnInt; // Number of integer frequency ratios considered (10 by default) as candidates

		// Parameters for weight candidates
		// Harmonic Error Constants
		TData mPMp, mPMq, mPMr, mMPp, mMPq, mMPr; // default values :0.5, 1.4, 0.5, 0.5, 1.4, 0.5
		TSize mPMnPeaks, mMPnPeaks;   // Harmonic peaks to weight error, 10 by default
		TData mPMCont, mMPCont; // Harmonic error contributions. Default: 1, 3
	};

};//namespace CLAM

#endif // _SPECTRUM_ADDER_
