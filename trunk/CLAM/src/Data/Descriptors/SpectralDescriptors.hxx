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

#ifndef __SpectralDescriptors_H__
#define __SpectralDescriptors_H__


#include "Flags.hxx"
#include "DynamicType.hxx"
#include "ProcessingData.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"


/*
 * This class holds Descriptors computed from Spectral data 
 * 
 *
 */

namespace CLAM {

	class SpectralDescriptors : public ProcessingData {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralDescriptors, 17, ProcessingData);
		
		DYN_ATTRIBUTE (0, public, TData, Mean);
		DYN_ATTRIBUTE (1, public, TData, GeometricMean);
		DYN_ATTRIBUTE (2, public, TData, Energy);
		DYN_ATTRIBUTE (3, public, TData, Centroid);
		DYN_ATTRIBUTE (4, public, TData, Moment2);
		DYN_ATTRIBUTE (5, public, TData, Moment3);
		DYN_ATTRIBUTE (6, public, TData, Moment4);
		DYN_ATTRIBUTE (7, public, TData, Moment5);
		DYN_ATTRIBUTE (8, public, TData, Moment6);
		DYN_ATTRIBUTE (9, public, TData, Irregularity);
		DYN_ATTRIBUTE (10,public, TData, Tilt);
		DYN_ATTRIBUTE (11,public, TData, Flatness);
		DYN_ATTRIBUTE (12,public, TData, Kurtosis);
		DYN_ATTRIBUTE (13,public, TData, StrongPeak);
		DYN_ATTRIBUTE (14,public, TData, HFC);
		DYN_ATTRIBUTE (15,public, Array<TData>, MFCC);
		DYN_ATTRIBUTE (16,public, Array<TData>, BandEnergy);

	public:

		friend class SpectralDescriptorsGen;
		void SetAll() {
			AddMean();
			AddGeometricMean();
			AddEnergy();
			AddCentroid();
			AddMoment2();
			AddMoment3();
			AddMoment4();
			AddMoment5();
			AddMoment6();
			AddIrregularity();
			AddTilt();
			AddFlatness();
			AddKurtosis();
			AddStrongPeak();
			AddHFC();
			AddMFCC();
			AddBandEnergy();
			UpdateData();
		}

	};


}


#endif /* __SpectralDescriptors_H__ */
