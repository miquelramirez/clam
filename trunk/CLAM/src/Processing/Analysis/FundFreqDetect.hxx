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

	/**
	 * Config class for FundFreqDetect Processing.
	 */
	class FundFreqDetectConfig: public ProcessingConfig
	{
	public:
	  DYNAMIC_TYPE_USING_INTERFACE (FundFreqDetectConfig, 18,ProcessingConfig);
	  /** Name. */
	  DYN_ATTRIBUTE (0, public, std::string, Name);
	  /** */
	  DYN_ATTRIBUTE (1,public,TData, ReferenceFundFreq);
	  /** Lowest frequency allowed for the fundamental. */
	  DYN_ATTRIBUTE (2,public,TData, LowestFundFreq); 
	  /** Highest frequency allowed for the fundamental. */
	  DYN_ATTRIBUTE (3,public,TData, HighestFundFreq); 
	  /** Maximum magnitude difference allowed between candidates (default: 30dB). */
	  DYN_ATTRIBUTE (4,public,TData, MaxCandMagDiff); 
	  /** Maximun allowed Two-Way Mismatch (TWM) error (default: 10). */
	  DYN_ATTRIBUTE (5,public,TData, MaxFundFreqError);
	  /** Parameter to weight candidates (Harmonic Error Constants). Default: 0.5. */
	  DYN_ATTRIBUTE (6,public,TData, PMp); 
	  /** Parameter to weight candidates (Harmonic Error Constants). Default: 1.4. */
	  DYN_ATTRIBUTE (7,public,TData, PMq );
	  /** Parameter to weight candidates (Harmonic Error Constants). Default: 0.5. */
	  DYN_ATTRIBUTE (8,public,TData, PMr );
	  /** Parameter to weight candidates (Harmonic Error Constants). Default: 0.5. */
	  DYN_ATTRIBUTE (9,public,TData, MPp );
	  /** Parameter to weight candidates (Harmonic Error Constants). Default: 1.4. */
	  DYN_ATTRIBUTE (10,public,TData, MPq );
	  /** Parameter to weight candidates (Harmonic Error Constants). Default: 0.5. */
	  DYN_ATTRIBUTE (11,public,TData, MPr);
	  /** Harmonic peaks to weight error (default: 10). */
	  DYN_ATTRIBUTE (12,public,TSize, PMnPeaks); 
	  /** Harmonic peaks to weight error (default: 10). */
	  DYN_ATTRIBUTE (13,public,TSize, MPnPeaks);
	  /** Harmonic error contributions (default: 1). */
	  DYN_ATTRIBUTE (14,public,TData, PMCont );
	  /** Harmonic error contributions (default: 1). */
	  DYN_ATTRIBUTE (15,public,TData, MPCont );
	  /** Number of integer frequency ratios considered as candidates (default: 5). */
	  DYN_ATTRIBUTE (16,public,TSize, NInt);
	  /** Maximum number of candidates. */
	  DYN_ATTRIBUTE (17,public,TSize, NMaxCandidates); 
	  void DefaultInit();
	  void DefaultValues();
	  ~FundFreqDetectConfig(){};
	};

	/**    This class estimates a fundamental frequency value
	*     in the spectral domain for one single spectral Frame.
	*     The input is a set of spectral peaks stored in the class CLAM::SpectralPeakArray
	*     The number of candidates is given by the CLAM::Fundamental object  
	*     using the attribute nMaxCandidate. 
	*     This implementation is based in the Two-Way Mismatch algorithm 
	*     proposed by Maher and Beauchamp (JASA vol 95. pp. 2254-2263 1993).
	*     @author emilia.gomez@iua.upf.es
	*     @see Fundamental SpectralPeakArray
	*/
	class FundFreqDetect: public Processing {
		mutable FundFreqDetectConfig mConfig;

		const char *GetClassName() const {return "FundFreqDetect";}

		bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

	public:
		FundFreqDetect();

		FundFreqDetect(const FundFreqDetectConfig &c);

		~FundFreqDetect();

		const ProcessingConfig &GetConfig() const;

		bool Do(void);

		/** Do function */
		bool Do(SpectralPeakArray& input,Fundamental& fund);

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

		/** Maximum number of candidates*/
		TData mnMaxCandidates;
		/** Reference Fundamental Frequency */
		TData mReferenceFundFreq;
		TData mLowestFundFreq;
		TData mHighestFundFreq;
	  
		/** Max magnitude offset allowed between candidates (30 dB default) */
		TData mMaxCandMagDiff; 

		/** Maximun TWM error accepted (10 default) */
		TData mMaxFundFreqError; 
		/** Number of integer frequency ratios considered (5 by default) as candidates */
		TSize mnInt; 

		/** Parameters to weight candidates */
		/** Harmonic Error Constants */
		TData mPMp, mPMq, mPMr, mMPp, mMPq, mMPr; // default values :0.5, 1.4, 0.5, 0.5, 1.4, 0.5
		TSize mPMnPeaks, mMPnPeaks;   // Harmonic peaks to weight error, 10 by default
		TData mPMCont, mMPCont; // Harmonic error contributions. Default: 1, 1
	};

};// namespace CLAM

#endif // _FUNFREQ_DETECT_
