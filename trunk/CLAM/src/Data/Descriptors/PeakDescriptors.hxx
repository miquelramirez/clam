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

#ifndef __PeakDescriptors_H__
#define  __PeakDescriptors_H__


#include "Flags.hxx"
#include "DynamicType.hxx"
#include "ProcessingData.hxx"
#include "ProcessingDataConfig.hxx"
#include "DataTypes.hxx"


/*
 * This class holds Descriptors computed from Peak data 
 * 
 *
 */

namespace CLAM {




	/**
	* Flags for descriptors derived from the peak array
	*/
	class PeakDescriptorsFlags : public CLAM::Flags<11> {

	// Construction/Destruction
	public:
		static tFlagValue sFlagValues[];
		virtual CLAM::Component * Species() const {
			return new PeakDescriptorsFlags();
		}
		typedef enum {
			eMagnitudeMean=0,
			eHarmonicCentroid=1,
			eSpectralTilt=2,
			eHarmonicDeviation=3,
			eFirstTristimulus=4,
			eSecondTristimulus=5,
			eThirdTristimulus=6,
			eBrightness=7,
			eOddHarmonics=8,
			eEvenHarmonics=9,
			eOddToEvenRatio=10
		} tFlag;

		PeakDescriptorsFlags () :
			CLAM::Flags<11>(sFlagValues),
			magnitudeMean(operator[](eMagnitudeMean)),
			harmonicCentroid(operator[](eHarmonicCentroid)),
			spectralTilt(operator[](eSpectralTilt)),
			harmonicDeviation(operator[](eHarmonicDeviation)),
			firstTristimulus(operator[](eFirstTristimulus)),
			secondTristimulus(operator[](eSecondTristimulus)),
			thirdTristimulus(operator[](eThirdTristimulus)),
			brightness(operator[](eBrightness)),
			oddHarmonics(operator[](eOddHarmonics)),
			evenHarmonics(operator[](eEvenHarmonics)),
			oddToEvenRatio(operator[](eOddToEvenRatio))
		{
		};

		template <class T> PeakDescriptorsFlags(const T &t) : 
			CLAM::Flags<11>(sFlagValues,t),
			magnitudeMean(operator[](eMagnitudeMean)),
			harmonicCentroid(operator[](eHarmonicCentroid)),
			spectralTilt(operator[](eSpectralTilt)),
			harmonicDeviation(operator[](eHarmonicDeviation)),
			firstTristimulus(operator[](eFirstTristimulus)),
			secondTristimulus(operator[](eSecondTristimulus)),
			thirdTristimulus(operator[](eThirdTristimulus)),
			brightness(operator[](eBrightness)),
			oddHarmonics(operator[](eOddHarmonics)),
			evenHarmonics(operator[](eEvenHarmonics)),
			oddToEvenRatio(operator[](eOddToEvenRatio))
		{};

		template <class T1, class T2> PeakDescriptorsFlags(const T1 &t1, const T2 &t2) : 
			CLAM::Flags<11>(sFlagValues,t1,t2),
			magnitudeMean(operator[](eMagnitudeMean)),
			harmonicCentroid(operator[](eHarmonicCentroid)),
			spectralTilt(operator[](eSpectralTilt)),
			harmonicDeviation(operator[](eHarmonicDeviation)),
			firstTristimulus(operator[](eFirstTristimulus)),
			secondTristimulus(operator[](eSecondTristimulus)),
			thirdTristimulus(operator[](eThirdTristimulus)),
			brightness(operator[](eBrightness)),
			oddHarmonics(operator[](eOddHarmonics)),
			evenHarmonics(operator[](eEvenHarmonics)),
			oddToEvenRatio(operator[](eOddToEvenRatio))
		{};

		/**
		* Mean of the magnitude of detected peaks
		*/
		reference magnitudeMean;

		/**
		* 1st order moment of harmonics
		*/
		reference harmonicCentroid;

		/**
		* Spectral Tilt of the sinusoidal component: linear regression of the (fi,ai) data points
		*/
		reference spectralTilt;

		/**
		* Spectral Deviation taking into account only harmonic content
		*/
		reference harmonicDeviation;

		/**
		* Energy of the 1st harmonic
		*/
		reference firstTristimulus;

		/**
		* 2nd modified Tristimulus parameter
		*/
		reference secondTristimulus;

		/**
		* 3rd modified Tristimulus parameter
		*/
		reference thirdTristimulus;

		/**
		* 1st order moment using number of harmonic instead of frequency
		*/
		reference brightness;

		/**
		* Content in odd harmonics excluding the 1st one
		*/
		reference oddHarmonics;

		/**
		* Content in even harmonics
		*/
		reference evenHarmonics;

		/**
		* Ratio of Odd harmonics content divided by Even harmonics content
		*/
		reference oddToEvenRatio;


	};


	class PeakDescriptorsConfig : public ProcessingDataConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (PeakDescriptorsConfig, 2, ProcessingDataConfig);

		void Init();

		DYN_ATTRIBUTE(1, HoldsData, bool);
	};


	class PeakDescriptors : public ProcessingData {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (PeakDescriptors, 11, ProcessingData);
		DYN_ATTRIBUTE(0, public, TData, MagnitudeMean);
		DYN_ATTRIBUTE(1, public, TData, HarmonicCentroid);
		DYN_ATTRIBUTE(2, public, TData, SpectralTilt);
		DYN_ATTRIBUTE(3, public, TData, HarmonicDeviation);
		DYN_ATTRIBUTE(4, public, TData, FirstTristimulus);
		DYN_ATTRIBUTE(5, public, TData, SecondTristimulus);
		DYN_ATTRIBUTE(6, public, TData, ThirdTristimulus);
		DYN_ATTRIBUTE(7, public, TData, Brightness);
		DYN_ATTRIBUTE(8, public, TData, OddHarmonics);
		DYN_ATTRIBUTE(9, public, TData, EvenHarmonics);
		DYN_ATTRIBUTE(10,public, TData, OddToEvenRatio);

	public:

		PeakDescriptors(const PeakDescriptorsFlags& flags)

		void GetFlags(PeakDescriptorsFlags & flags) const;
		void SetFlags(const PeakDescriptorsFlags & flags);

		friend class PeakDescriptorsGen;
	};


};


#endif /* __PeakDescriptors_H__ */
