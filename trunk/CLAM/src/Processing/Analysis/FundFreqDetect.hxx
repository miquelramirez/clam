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

#ifndef _FundFreqDetect__
#define _FundFreqDetect__

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "FundFreqDetectConfig.hxx"

namespace CLAM {

	class Storage;
	class ProcessingConfig;
	class Fundamental;
	class SpectralPeakArray;

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

		bool ConcreteConfigure(const ProcessingConfig&);

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

} // namespace CLAM

#endif // _FUNFREQ_DETECT_
