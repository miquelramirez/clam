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

#ifndef __AUDIODescriptors_H__
#define __AUDIODescriptors_H__


#include "Flags.hxx"
#include "DynamicType.hxx"
#include "ProcessingData.hxx"
#include "ProcessingDataConfig.hxx"
#include "DataTypes.hxx"


/*
 * This class holds Descriptors computed from Audio data 
 * 
 *
 */

namespace CLAM {

	/**
	* Flags that are used to decide which Descriptorss will be computed
	*/
	class AudioDescriptorsFlags : public Flags<11> {

	// Construction/Destruction
	public:
		static tFlagValue sFlagValues[];
		virtual CLAM::Component * Species() const {
			return new AudioDescriptorsFlags();
		}
		typedef enum {
			eMean=0,
			eVariance=1,
			eTemporalCentroid=2,
			eAttack=3,
			eDecay=4,
			eSustain=5,
			eRelease=6,
			eLogAttackTime=7,
			eEnergy=8,
			eZeroCrossingRate=9,
			eRiseTime=10,
		} tFlag;

		AudioDescriptorsFlags () :
			CLAM::Flags<11>(sFlagValues),
			mean(operator[](eMean)),
			variance(operator[](eVariance)),
			temporalCentroid(operator[](eTemporalCentroid)),
			attack(operator[](eAttack)),
			decay(operator[](eDecay)),
			sustain(operator[](eSustain)),
			release(operator[](eRelease)),
			logAttackTime(operator[](eLogAttackTime)),
			energy(operator[](eEnergy)),
			zeroCrossingRate(operator[](eZeroCrossingRate)),
			riseTime(operator[](eRiseTime))
		{
		};

		template <class T> AudioDescriptorsFlags(const T &t) : 
			CLAM::Flags<11>(sFlagValues,t),
			mean(operator[](eMean)),
			variance(operator[](eVariance)),
			temporalCentroid(operator[](eTemporalCentroid)),
			attack(operator[](eAttack)),
			decay(operator[](eDecay)),
			sustain(operator[](eSustain)),
			release(operator[](eRelease)),
			logAttackTime(operator[](eLogAttackTime)),
			energy(operator[](eEnergy)),
			zeroCrossingRate(operator[](eZeroCrossingRate)),
			riseTime(operator[](eRiseTime))
		{};

		template <class T1, class T2> AudioDescriptorsFlags(const T1 &t1, const T2 &t2) : 
			CLAM::Flags<11>(sFlagValues,t1,t2),
			mean(operator[](eMean)),
			variance(operator[](eVariance)),
			temporalCentroid(operator[](eTemporalCentroid)),
			attack(operator[](eAttack)),
			decay(operator[](eDecay)),
			sustain(operator[](eSustain)),
			release(operator[](eRelease)),
			logAttackTime(operator[](eLogAttackTime)),
			energy(operator[](eEnergy)),
			zeroCrossingRate(operator[](eZeroCrossingRate)),
			riseTime(operator[](eRiseTime))
		{};

		/**
		* The Mean (DC offset)
		*/
		reference mean;

		/**
		* Variance
		*/
		reference variance;

		/**
		* Temporal Centroid
		*/
		reference temporalCentroid;

		/**
		* Attack Duration and Energy
		*/
		reference attack;

		/**
		* Decay Duration and Energy
		*/
		reference decay;

		/**
		* Sustain Duration and Energy
		*/
		reference sustain;

		/**
		* Release Duration and Energy
		*/
		reference release;

		/**
		* Logarithmic Attack Time
		*/
		reference logAttackTime;

		/**
		* Energy (RMS value) of the Signal
		*/
		reference energy;

		/**
		* The Zero Crossing Rate
		*/
		reference zeroCrossingRate;

		/**
		* The Rise Time on logarithmic scale
		*/
		reference riseTime;

	};

	class AudioDescriptors : public ProcessingData {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (AudioDescriptors, 11, ProcessingData);
		DYN_ATTRIBUTE (0, public, TData, Mean);
		DYN_ATTRIBUTE (1, public, TData, Variance);
		DYN_ATTRIBUTE (2, public, TData, TemporalCentroid);
		DYN_ATTRIBUTE (3, public, TData, Attack);
		DYN_ATTRIBUTE (4, public, TData, Decay);
		DYN_ATTRIBUTE (5, public, TData, Sustain);
		DYN_ATTRIBUTE (6, public, TData, Release);
		DYN_ATTRIBUTE (7, public, TData, LogAttackTime);
		DYN_ATTRIBUTE (8, public, TData, Energy);
		DYN_ATTRIBUTE (9, public, TData, ZeroCrossingRate);
		DYN_ATTRIBUTE (10,public, TData, RiseTime);

	public:

		AudioDescriptors(const AudioDescriptorsFlags& flags);
		void GetFlags(AudioDescriptorsFlags& flags) const;
		void SetFlags(const AudioDescriptorsFlags& flags);

		friend class AudioDescriptorsGen;

	};

};


#endif /* __AUDIODescriptors_H__ */
