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

#ifndef __SpectralPeakDescriptors_H__
#define __SpectralPeakDescriptors_H__

#include "DynamicType.hxx"
#include "ProcessingData.hxx"
#include "DataTypes.hxx"


/*
 * This class holds Descriptors computed from SpectralPeak data 
 * 
 *
 */

namespace CLAM {

	
	class SpectralPeakArray;


	class SpectralPeakDescriptors : public ProcessingData {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralPeakDescriptors, 11, ProcessingData);
		DYN_ATTRIBUTE (0, public, TData, MagnitudeMean);
		DYN_ATTRIBUTE (1, public, TData, HarmonicCentroid);
		DYN_ATTRIBUTE (2, public, TData, SpectralTilt);
		DYN_ATTRIBUTE (3, public, TData, HarmonicDeviation);
		DYN_ATTRIBUTE (4, public, TData, FirstTristimulus);
		DYN_ATTRIBUTE (5, public, TData, SecondTristimulus);
		DYN_ATTRIBUTE (6, public, TData, ThirdTristimulus);
		DYN_ATTRIBUTE (7, public, TData, Brightness);
		DYN_ATTRIBUTE (8, public, TData, OddHarmonics);
		DYN_ATTRIBUTE (9, public, TData, EvenHarmonics);
		DYN_ATTRIBUTE (10,public, TData, OddToEvenRatio);

	public:
		SpectralPeakDescriptors(SpectralPeakArray* pSpectralPeakArray);

		const SpectralPeakArray* GetpSpectralPeakArray() const;
		void SetpSpectralPeakArray(SpectralPeakArray* pSpectralPeakArray);

		friend class SpectralPeakDescriptorsGen;

	private:
		void DefaultInit();
		void CopyInit(const SpectralPeakDescriptors & copied);

	private:
		SpectralPeakArray* mpSpectralPeakArray;

	};



// Implementation

inline void SpectralPeakDescriptors::DefaultInit() {
	mpSpectralPeakArray=0;
}

inline void SpectralPeakDescriptors::CopyInit(const SpectralPeakDescriptors & copied) {
	mpSpectralPeakArray=copied.mpSpectralPeakArray;
}

inline const SpectralPeakArray* SpectralPeakDescriptors::GetpSpectralPeakArray() const {
	return mpSpectralPeakArray;
}

inline void SpectralPeakDescriptors::SetpSpectralPeakArray(SpectralPeakArray* pSpectralPeakArray) {
	mpSpectralPeakArray=pSpectralPeakArray;
}


}


#endif /* __SpectralPeakDescriptors_H__ */
