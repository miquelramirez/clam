//////////////////////////////////////////////////////////////////////
// AudioDescriptorFlags header file
// This file is generated from AudioDescriptorFlags.flag. Edit it instead.
//////////////////////////////////////////////////////////////////////
#ifndef _AudioDescriptorFlags_
#define _AudioDescriptorFlags_

#include "Flags.hxx"

namespace MTG {

	/**
	* Flags that are used to decide which Descriptors will be computed
	*/
	class AudioDescriptorFlags : public Flags<16> {

	// Construction/Destruction
	public:
		static tFlagValue sFlagValues[];
		virtual MTG::Component * Species() const {
			return new AudioDescriptorFlags();
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

		AudioDescriptorFlags () :
			MTG::Flags<16>(sFlagValues),
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

		template <class T> AudioDescriptorFlags(const T &t) : 
			MTG::Flags<16>(sFlagValues,t),
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

		template <class T1, class T2> AudioDescriptorFlags(const T1 &t1, const T2 &t2) : 
			MTG::Flags<16>(sFlagValues,t1,t2),
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


} // namespace MTG !!
#endif//_AudioDescriptorFlags_


