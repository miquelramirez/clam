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

#ifndef __SegmentDescriptors_H__
#define __SegmentDescriptors_H__


#include "Flags.hxx"
#include "DynamicType.hxx"
#include "ProcessingData.hxx"
#include "DataTypes.hxx"

#include "Melody.hxx"

/*
 * This class holds Descriptors computed from Segment data, right now it only holds 
 * descriptors for spectral descriptors mean plus fundamental
 *
 */

namespace CLAM {

	/**
	* Flags that are used to decide which Descriptorss will be computed
	*/
	class SegmentDescriptorsFlags : public Flags<16> {

	// Construction/Destruction
	public:
		static tFlagValue sFlagValues[];
		virtual CLAM::Component * Species() const {
			return new SegmentDescriptorsFlags();
		}
		typedef enum {
			eSpectralMean=0,
			eSpectralGeometricMean=1,
			eSpectralEnergy=2,
			eSpectralCentroid=3,
			eSpectralMoment2=4,
			eSpectralMoment3=5,
			eSpectralMoment4=6,
			eSpectralMoment5=7,
			eSpectralMoment6=8,
			eSpectralIrregularity=9,
			eSpectralTilt=10,
			eSpectralFlatness=11,
			eSpectralKurtosis=12,
			eSpectralStrongPeak=13,
			eFundamental=14,
			eMelody=15
		} tFlag;

		SegmentDescriptorsFlags () :
			CLAM::Flags<16>(sFlagValues),
			spectralMean(operator[](eSpectralMean)),
			spectralGeometricMean(operator[](eSpectralGeometricMean)),
			spectralEnergy(operator[](eSpectralEnergy)),
			spectralCentroid(operator[](eSpectralCentroid)),
			spectralMoment2(operator[](eSpectralMoment2)),
			spectralMoment3(operator[](eSpectralMoment3)),
			spectralMoment4(operator[](eSpectralMoment4)),
			spectralMoment5(operator[](eSpectralMoment5)),
			spectralMoment6(operator[](eSpectralMoment6)),
			spectralIrregularity(operator[](eSpectralIrregularity)),
			spectralTilt(operator[](eSpectralTilt)),
			spectralFlatness(operator[](eSpectralFlatness)),
			spectralKurtosis(operator[](eSpectralKurtosis)),
			spectralStrongPeak(operator[](eSpectralStrongPeak)),
			fundamental(operator[](eFundamental)),
			melody(operator[](eMelody))
		{
		};

		template <class T> SegmentDescriptorsFlags(const T &t) : 
			CLAM::Flags<16>(sFlagValues,t),
			spectralMean(operator[](eSpectralMean)),
			spectralGeometricMean(operator[](eSpectralGeometricMean)),
			spectralEnergy(operator[](eSpectralEnergy)),
			spectralCentroid(operator[](eSpectralCentroid)),
			spectralMoment2(operator[](eSpectralMoment2)),
			spectralMoment3(operator[](eSpectralMoment3)),
			spectralMoment4(operator[](eSpectralMoment4)),
			spectralMoment5(operator[](eSpectralMoment5)),
			spectralMoment6(operator[](eSpectralMoment6)),
			spectralIrregularity(operator[](eSpectralIrregularity)),
			spectralTilt(operator[](eSpectralTilt)),
			spectralFlatness(operator[](eSpectralFlatness)),
			spectralKurtosis(operator[](eSpectralKurtosis)),
			spectralStrongPeak(operator[](eSpectralStrongPeak)),
			fundamental(operator[](eFundamental)),
			melody(operator[](eMelody))
		{};

		template <class T1, class T2> SegmentDescriptorsFlags(const T1 &t1, const T2 &t2) : 
			CLAM::Flags<15>(sFlagValues,t1,t2),
			spectralMean(operator[](eSpectralMean)),
			spectralGeometricMean(operator[](eSpectralGeometricMean)),
			spectralEnergy(operator[](eSpectralEnergy)),
			spectralCentroid(operator[](eSpectralCentroid)),
			spectralMoment2(operator[](eSpectralMoment2)),
			spectralMoment3(operator[](eSpectralMoment3)),
			spectralMoment4(operator[](eSpectralMoment4)),
			spectralMoment5(operator[](eSpectralMoment5)),
			spectralMoment6(operator[](eSpectralMoment6)),
			spectralIrregularity(operator[](eSpectralIrregularity)),
			spectralTilt(operator[](eSpectralTilt)),
			spectralFlatness(operator[](eSpectralFlatness)),
			spectralKurtosis(operator[](eSpectralKurtosis)),
			spectralStrongPeak(operator[](eSpectralStrongPeak)),
			fundamental(operator[](eFundamental)),
			melody(operator[](eMelody))
		{};

		/**
		* The Amplitude Mean
		*/
		reference spectralMean;

		/**
		* The Amplitude Geometric Mean
		*/
		reference spectralGeometricMean;

		/**
		* Energy computed from the spectrum
		*/
		reference spectralEnergy;

		/**
		* Centroid, 1st order spectralMoment
		*/
		reference spectralCentroid;

		/**
		* 2nd order spectralMoment
		*/
		reference spectralMoment2;

		/**
		* 3rd order spectralMoment
		*/
		reference spectralMoment3;

		/**
		* 4th order spectralMoment
		*/
		reference spectralMoment4;

		/**
		* 5th order spectralMoment
		*/
		reference spectralMoment5;

		/**
		* 6th order spectralMoment
		*/
		reference spectralMoment6;


		/**
		* Irregularity¿?
		*/
		reference spectralIrregularity;

		/**
		* Slope of the linear regression of the Segment data points
		*/
		reference spectralTilt;

		/**
		* Measure of the tonal (clear maxima) or noiselike (flat spectrum) character of the sound
		*/
		reference spectralFlatness;

		/**
		* Measure of how outlier-prone the spectrum is
		*/
		reference spectralKurtosis;

		/**
		* Reveals whether the spectrum presents a strong peak
		*/
		reference spectralStrongPeak;

		/**
		* Mean fundamental in the segment
		*/
		reference fundamental;

		/* Melody description of the audio segment */
		reference melody;
	};

	
	class SegmentDescriptors : public ProcessingData {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SegmentDescriptors, 16, ProcessingData);
		/*All these attributes refer to mean values of spectrum descriptors across
		frames in the segment*/
		DYN_ATTRIBUTE (0, public, TData, SpectralMean);
		DYN_ATTRIBUTE (1, public, TData, SpectralGeometricMean);
		DYN_ATTRIBUTE (2, public, TData, SpectralEnergy);
		DYN_ATTRIBUTE (3, public, TData, SpectralCentroid);
		DYN_ATTRIBUTE (4, public, TData, SpectralMoment2);
		DYN_ATTRIBUTE (5, public, TData, SpectralMoment3);
		DYN_ATTRIBUTE (6, public, TData, SpectralMoment4);
		DYN_ATTRIBUTE (7, public, TData, SpectralMoment5);
		DYN_ATTRIBUTE (8, public, TData, SpectralMoment6);
		DYN_ATTRIBUTE (9, public, TData, SpectralIrregularity);
		DYN_ATTRIBUTE (10, public, TData, SpectralTilt);
		DYN_ATTRIBUTE (11, public, TData, SpectralFlatness);
		DYN_ATTRIBUTE (12, public, TData, SpectralKurtosis);
		DYN_ATTRIBUTE (13, public, TData, SpectralStrongPeak);
		/** Mean value for fundamental across all frames */
		DYN_ATTRIBUTE (14,public, TData, Fundamental);
		DYN_ATTRIBUTE (15,public, Melody, Melody);

	public:

		SegmentDescriptors(SegmentDescriptorsFlags& flags);
		void SetFlags(const SegmentDescriptorsFlags& flags);
		void GetFlags(SegmentDescriptorsFlags& flags) const;

		friend class SegmentDescriptorsGen;

	};


};


#endif /* __SegmentDescriptors_H__ */
