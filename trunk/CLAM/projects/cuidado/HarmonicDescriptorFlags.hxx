//////////////////////////////////////////////////////////////////////
// HarmonicDescriptorFlags header file
// This file is generated from HarmonicDescriptorFlags.flag. Edit it instead.
//////////////////////////////////////////////////////////////////////
#ifndef _HarmonicDescriptorFlags_
#define _HarmonicDescriptorFlags_

#include "Flags.hxx"

namespace MTG {

	/**
	* These are descriptors about temporal evolution of single harmonic sounds
	*/
	class HarmonicDescriptorFlags : public MTG::Flags<5> {

	// Construction/Destruction
	public:
		static tFlagValue sFlagValues[];
		virtual MTG::Component * Species() const {
			return new HarmonicDescriptorFlags();
		}
		typedef enum {
			eTransientMidPoint=0,
			eHarmonicRiseTimes=1,
			eOvershoots=2,
			eLowHarmonicsDelay=3,
			eHighHarmonicsDelay=4
		} tFlag;

		HarmonicDescriptorFlags () :
			MTG::Flags<5>(sFlagValues),
			transientMidPoint(operator[](eTransientMidPoint)),
			harmonicRiseTimes(operator[](eHarmonicRiseTimes)),
			overshoots(operator[](eOvershoots)),
			lowHarmonicsDelay(operator[](eLowHarmonicsDelay)),
			highHarmonicsDelay(operator[](eHighHarmonicsDelay))
		{
		};

		template <class T> HarmonicDescriptorFlags(const T &t) : 
			MTG::Flags<5>(sFlagValues,t),
			transientMidPoint(operator[](eTransientMidPoint)),
			harmonicRiseTimes(operator[](eHarmonicRiseTimes)),
			overshoots(operator[](eOvershoots)),
			lowHarmonicsDelay(operator[](eLowHarmonicsDelay)),
			highHarmonicsDelay(operator[](eHighHarmonicsDelay))
		{};

		template <class T1, class T2> HarmonicDescriptorFlags(const T1 &t1, const T2 &t2) : 
			MTG::Flags<5>(sFlagValues,t1,t2),
			transientMidPoint(operator[](eTransientMidPoint)),
			harmonicRiseTimes(operator[](eHarmonicRiseTimes)),
			overshoots(operator[](eOvershoots)),
			lowHarmonicsDelay(operator[](eLowHarmonicsDelay)),
			highHarmonicsDelay(operator[](eHighHarmonicsDelay))
		{};

		/**
		* The transient midpoint
		*/
		reference transientMidPoint;

		/**
		* Harmonic Rise Time
		*/
		reference harmonicRiseTimes;

		/**
		* Energy of the first harmonic at the attack
		*/
		reference overshoots;

		/**
		* Delay of the second,third and fourth Harmonic during attack
		*/
		reference lowHarmonicsDelay;

		/**
		* Delay of the higher order harmonics during attack
		*/
		reference highHarmonicsDelay;


	};
}

#endif//_HarmonicDescriptorFlags_


