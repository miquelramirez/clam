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


#include "Array.hxx"
#include "Descriptor.hxx"


/*
 * This class holds Descriptors computed from Spectral data 
 * 
 *
 */

namespace CLAM {

	class Spectrum;

	class SpectralDescriptors : public Descriptor {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralDescriptors, 23, Descriptor);
		
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
		DYN_ATTRIBUTE (17,public, TData, MaxMagFreq); 
		// Frequency of the maximun of the spectrum normalized by the spectral range
		DYN_ATTRIBUTE (18,public, TData, LowFreqEnergyRelation); 
		DYN_ATTRIBUTE (19,public, TData, Skewness); 
		DYN_ATTRIBUTE (20,public, TData, Rolloff); 
		DYN_ATTRIBUTE (21,public, TData, HighFrequencyCoefficient);
		DYN_ATTRIBUTE (22,public, Array<SpectralDescriptors>, BandDescriptors);

	public:
		SpectralDescriptors(Spectrum* pSpectrum);

		const Spectrum* GetpSpectrum() const;
		void SetpSpectrum(Spectrum* pSpectrum);
		void ConcreteCompute();

		void SetPrototype(const SpectralDescriptors& p);

		

	private:
		void DefaultInit();
		void CopyInit(const SpectralDescriptors & copied);
		
		TData ComputeSpectralTilt();
		TData ComputeSpectralFlatness();
		TData ComputeHighFrequencyCoefficient();
		TData ComputeMaxMagFreq();
		TData ComputeLowFreqEnergyRelation();
		TData ComputeRolloff();

	private:
		Spectrum* mpSpectrum;
		
		/** Conversion from index to frequency, needed for many descriptors */
		TData mDeltaFreq;
	};





}


#endif /* __SpectralDescriptors_H__ */
