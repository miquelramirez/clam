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

#ifndef _MASKING_PEAK_DETECT_H
#define _MASKING_PEAK_DETECT_H

#include "Processing.hxx"
#include "Port.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "Enum.hxx"
#include <queue>

namespace CLAM {

	class EFrequencyWeight : public Enum {
	public:
		
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EFrequencyWeight() : Enum(sEnumValues, sDefault) {}
		EFrequencyWeight(tValue v) : Enum(sEnumValues, v) {};
		EFrequencyWeight(std::string s) : Enum(sEnumValues, s) {};
		
		typedef enum {
			eNone,
			eLogarithmic,
			eInverse
		};
		
		virtual Component* Species() const
		{
			return (Component*) new EFrequencyWeight(eNone);
		};
	};


	class EMaskingShape : public Enum {
	public:
		
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EMaskingShape() : Enum(sEnumValues, sDefault) {}
		EMaskingShape(tValue v) : Enum(sEnumValues, v) {};
		EMaskingShape(std::string s) : Enum(sEnumValues, s) {};
		
		typedef enum {
			eNone,
			eLinear,
			eGaussian,
		};
		
		virtual Component* Species() const
		{
			return (Component*) new EMaskingShape(eNone);
		};
	};


	class MaskingPeakDetectConfig : public ProcessingConfig {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MaskingPeakDetectConfig, 8,ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, TSize,            MaxPeaks);
		DYN_ATTRIBUTE (2, public, TData,            MagThreshold);
		DYN_ATTRIBUTE (3, public, TSize,            SpectralSize);
		DYN_ATTRIBUTE (4, public, TData,            SpectralRange);
		DYN_ATTRIBUTE (5, public, EFrequencyWeight, FrequencyWeight);
		DYN_ATTRIBUTE (6, public, EMaskingShape,    MaskingShape);
		DYN_ATTRIBUTE (7, public, TData,            MaskingBandwidth);
	protected:
		void DefaultInit();
 	}; 

	class MaskingPeakDetect : public Processing {

	public:

		InPortTmpl<Spectrum>           Input;
		OutPortTmpl<SpectralPeakArray> Output;

		MaskingPeakDetect(const MaskingPeakDetectConfig& c = MaskingPeakDetectConfig());

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(const Spectrum& inp, SpectralPeakArray& out);

		void StoreOn(Storage &s) {};

	private:

		typedef enum {
			eLeft = -1,
			eRight = 1
		} EDirection;

		MaskingPeakDetectConfig  mConfig;

		TSize mMaxPeaks;
		TSize mSpectralSize;
		TData mSpectralRange;
		TSize mNumLocalPeaks;

		// This is where all the found local peaks are stored.
		// All the peak indexes in the code refer to the positions
		// in this arrays.
		Array<TData> mInterpolatedMags;
		Array<TData> mInterpolatedFreqs;

		void CheckData(const Spectrum& inp, SpectralPeakArray& out);

		bool ConcreteConfigure(const ProcessingConfig& cfg) throw(std::bad_cast);

		void WriteUnmaskedPeaks(SpectralPeakArray &output);

		void FindAndSortLocalPeaks(const Array<TData> &magnitude);

		void TagMaskedPeaks();

		void TagMaskedNeighbourgs(int peak_pos,
								  EDirection direction);

		inline TData MaskingShape(TData);

		Array<bool>       mMasked;
		TData             mMaskingBandwidth;
		EMaskingShape     mMaskingShape;


		struct ValuedIndex {
			TData value;
			int   index;
			bool operator <(const ValuedIndex &c) const {return value < c.value;}
			ValuedIndex() : value(0.0), index(0) {}
			ValuedIndex(float v, int i) : value(v), index(i) {}
		};

		struct IndexSorter : public std::priority_queue<ValuedIndex> {
			void reserve(int i) { c.reserve(i); }
			void clear()        { c.resize(0);  }
		};

		IndexSorter mPeakMagHeap;
		IndexSorter mPeakFreqHeap;


		class PeakFinder {

		public:

			PeakFinder();

			void Configure(int spectrum_size,
						   EFrequencyWeight weight,
						   TData spectral_range,
						   TData relative_threshold); // In negative dBs

			void Start(const Array<TData> &magnitude);

			bool FindNextPeak();

			inline TData InterpolatedPeakMag(); 

			inline TData InterpolatedPeakFreq();

			inline TData WeightedPeakMag();

		private:

			inline bool CheckAndComputePeak();

			inline bool CheckAndComputePlainPeak();

			typedef enum {
				eUp,
				eDown,
				eHorizontal
			} EDirection;

			typedef enum {
				eGoingUphill,
				eInPlainGoingUphill,
				eGoingDownhill
			} EState;

			EState       mState;
			int          mCurrent;
			int          mUphillPlainFirst;

			TData        mPeakMag;
			TData        mPeakWeightedMag;
			TData        mPeakFreq;

			Array<TData> mMag;
			Array<TData> mWeightedMags;
			TData        mRelativeThreshold;

			Array<TData> mWeightTable;
			TData        mAbsoluteThreshold;
			TData        mBinDelta;

		};

		PeakFinder mPeakFinder;

	};

}

#endif
