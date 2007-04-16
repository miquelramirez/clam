/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Descriptor.hxx"

/*
 * This class holds Descriptors computed from SpectralPeak data 
 * 
 *
 */

namespace CLAM {

	
	class SpectralPeakArray;


	class SpectralPeakDescriptors : public Descriptor {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralPeakDescriptors, 10, Descriptor);
		/**
		* The average amplitude (arithmetic mean) of the spectral peaks.
		* It is computed on linear scale.
		* Range: [0, 100]
		*/
		DYN_ATTRIBUTE (0, public, TData, MagnitudeMean);
		/**
		* The center of gravity of the spectral peaks.
		* Only the frequency value (in Hz) of the centroid is computed.
		* Amplitude and frequency scale are linear for this operation.
		*
		* - Units: Hz
		* - Range: [0, Nyquist frequency]
		* - Singularities
		*   - Returns zero for empty peak array.
		* @todo Peaks at 0Hz are not counted
		*/
		DYN_ATTRIBUTE (1, public, TData, HarmonicCentroid);
		/**
		* Spectral deviation of magnitude components from spectral envelope.
		* Descriptor is a MPEG7 LLD (HarmonicSpectralDeviation), see 
		* ISO/IEC JTC 1/SC 29 N WXYZ from 2001-03-14.
		* 
		* The local spectral envelopes (SE) are computed on linear scale as
		* the mean of three consecutive peak amplitudes.
		* For the first and the last peak only two peak amplitudes are used. 
		*
		* The spectral HarmonicDeviation is then defined
		* as 
		* \f[ \sum_{i=1}^{Num. of Peaks} 
		* 	\left| PeakAmplitude(i) - SE(i)\right| 
		* 	\over
		* 	{\sum_{i=1}^{Num. of Peaks}PeakAmplitude(i)} \f]
		* ,where SE and PeakAmplitude were converted to logarithmic scale.
		* Returns zero for array with less than 4 peaks.
		*/
		DYN_ATTRIBUTE (2, public, TData, HarmonicDeviation);
		/**
		* Energy ratio of the first harmonic to the energy of all peaks.
		* 
		* Returns zero for empty peak array.
		*/
		DYN_ATTRIBUTE (3, public, TData, FirstTristimulus);
		/**
		* Energy ratio of the second, third and forth harmonic to the energy of all peaks.
		* Returns zero for array with less than 2 peaks.
		*/
		DYN_ATTRIBUTE (4, public, TData, SecondTristimulus);
		/**
		* Energy ratio of all the harmonics starting from the fifth to the energy of all peaks.
		* Returns zero for array with less than 5 peaks.
		*/
		DYN_ATTRIBUTE (5, public, TData, ThirdTristimulus);
		/**
		* Energy ratio of odd harmonics (excluding the fundamental) to 
		* the energy of all peaks.
		* 
		* Returns zero for array with less than 3 peaks.
		*/
		DYN_ATTRIBUTE (6, public, TData, OddHarmonics);
		/**
		* Energy ratio of even harmonics to the energy of all peaks.
		*
		* @pre The peaks represents an ordered list of harmonics starting 
		* at the fundamental.
		* 
		* Returns zero for array with less than 2 peaks.
		*/
		DYN_ATTRIBUTE (7, public, TData, EvenHarmonics);
		/**
		* Ratio of "OddHarmonics" to "EvenHarmonics".		
		* Returns zero if both values are zero.
		*
		* @pre The peaks represents an ordered list of harmonics starting 
		* at the fundamental.
		* 
		* Range: [0, 1]
		*
		* Formula: \f[ \frac{OddHarmonics}{EvenHarmonics+OddHarmonics} \f]
		* @see SpectralPeakDescriptors::DYN_ATTRIBUTE(OddHarmonics)
		* @see SpectralPeakDescriptors::DYN_ATTRIBUTE(EvenHarmonics)
		*/
		DYN_ATTRIBUTE (8, public, TData, OddToEvenRatio);
		DYN_ATTRIBUTE (9,public, Array<TData>, HPCP);  // Harmonic Pitch Class Profile

	public:
		SpectralPeakDescriptors(SpectralPeakArray* pSpectralPeakArray);
		SpectralPeakDescriptors(TData initVal);

		const SpectralPeakArray* GetpSpectralPeakArray() const;
		void SetpSpectralPeakArray(SpectralPeakArray* pSpectralPeakArray);
		void ConcreteCompute();

	private:
		void DefaultInit();
		void CopyInit(const SpectralPeakDescriptors & copied);
		
		TData ComputeCentroid();
		TData ComputeFirstTristimulus();
		TData ComputeSecondTristimulus();
		TData ComputeThirdTristimulus();
		TData ComputeHarmonicDeviation();
		TData ComputeOddHarmonics();
		TData ComputeEvenHarmonics();
		TData ComputeOddToEvenRatio();

	private:
		const SpectralPeakArray* mpSpectralPeakArray;
		CrossCenterOfGravity<1> mCentroid;

	};


SpectralPeakDescriptors operator / (const SpectralPeakDescriptors& a,TData div) ;
SpectralPeakDescriptors operator * (const SpectralPeakDescriptors& a,TData mult) ;
SpectralPeakDescriptors operator * (const SpectralPeakDescriptors& a,const SpectralPeakDescriptors& b) ;
SpectralPeakDescriptors operator + (const SpectralPeakDescriptors& a,const SpectralPeakDescriptors& b) ;

template<>
inline SpectralPeakDescriptors CLAM_max (const SpectralPeakDescriptors& a,const SpectralPeakDescriptors& b)
{
	SpectralPeakDescriptors tmpD(a);

	if (a.HasMagnitudeMean() && b.HasMagnitudeMean())
	{
		if(b.GetMagnitudeMean()>a.GetMagnitudeMean())
			tmpD.SetMagnitudeMean(b.GetMagnitudeMean());
	}
	if (a.HasHarmonicCentroid() && b.HasHarmonicCentroid())
	{
		if(b.GetHarmonicCentroid()>a.GetHarmonicCentroid())
			tmpD.SetHarmonicCentroid(b.GetHarmonicCentroid());
	}
	if (a.HasFirstTristimulus() && b.HasFirstTristimulus())
	{
		if(b.GetFirstTristimulus()>a.GetFirstTristimulus())
			tmpD.SetFirstTristimulus(b.GetFirstTristimulus());
	}
	if (a.HasSecondTristimulus() && b.HasSecondTristimulus())
	{
		if(b.GetSecondTristimulus()>a.GetSecondTristimulus())
			tmpD.SetSecondTristimulus(b.GetSecondTristimulus());
	}
	if (a.HasThirdTristimulus() && b.HasThirdTristimulus())
	{
		if(b.GetThirdTristimulus()>a.GetThirdTristimulus())
			tmpD.SetThirdTristimulus(b.GetThirdTristimulus());
	}
	if (a.HasHarmonicDeviation() && b.HasHarmonicDeviation())
	{
		if(b.GetHarmonicDeviation()>a.GetHarmonicDeviation())
			tmpD.SetHarmonicDeviation(b.GetHarmonicDeviation());
	}
	if (a.HasOddHarmonics() && b.HasOddHarmonics())
	{
		if(b.GetOddHarmonics()>a.GetOddHarmonics())
			tmpD.SetOddHarmonics(b.GetOddHarmonics());
	}
	if (a.HasEvenHarmonics() && b.HasEvenHarmonics())
	{
		if(b.GetEvenHarmonics()>a.GetEvenHarmonics())
			tmpD.SetEvenHarmonics(b.GetEvenHarmonics());
	}
	if (a.HasOddToEvenRatio() && b.HasOddToEvenRatio())
	{
		if(b.GetOddToEvenRatio()>a.GetOddToEvenRatio())
			tmpD.SetOddToEvenRatio(b.GetOddToEvenRatio());
	}
	
	return tmpD;
}

template<>
inline SpectralPeakDescriptors CLAM_min (const SpectralPeakDescriptors& a,const SpectralPeakDescriptors& b)
{
	SpectralPeakDescriptors tmpD(a);

	if (a.HasMagnitudeMean() && b.HasMagnitudeMean())
	{
		if(b.GetMagnitudeMean()<a.GetMagnitudeMean())
			tmpD.SetMagnitudeMean(b.GetMagnitudeMean());
	}
	if (a.HasHarmonicCentroid() && b.HasHarmonicCentroid())
	{
		if(b.GetHarmonicCentroid()<a.GetHarmonicCentroid())
			tmpD.SetHarmonicCentroid(b.GetHarmonicCentroid());
	}
	if (a.HasFirstTristimulus() && b.HasFirstTristimulus())
	{
		if(b.GetFirstTristimulus()<a.GetFirstTristimulus())
			tmpD.SetFirstTristimulus(b.GetFirstTristimulus());
	}
	if (a.HasSecondTristimulus() && b.HasSecondTristimulus())
	{
		if(b.GetSecondTristimulus()<a.GetSecondTristimulus())
			tmpD.SetSecondTristimulus(b.GetSecondTristimulus());
	}
	if (a.HasThirdTristimulus() && b.HasThirdTristimulus())
	{
		if(b.GetThirdTristimulus()<a.GetThirdTristimulus())
			tmpD.SetThirdTristimulus(b.GetThirdTristimulus());
	}
	if (a.HasHarmonicDeviation() && b.HasHarmonicDeviation())
	{
		if(b.GetHarmonicDeviation()<a.GetHarmonicDeviation())
			tmpD.SetHarmonicDeviation(b.GetHarmonicDeviation());
	}
	if (a.HasOddHarmonics() && b.HasOddHarmonics())
	{
		if(b.GetOddHarmonics()<a.GetOddHarmonics())
			tmpD.SetOddHarmonics(b.GetOddHarmonics());
	}
	if (a.HasEvenHarmonics() && b.HasEvenHarmonics())
	{
		if(b.GetEvenHarmonics()<a.GetEvenHarmonics())
			tmpD.SetEvenHarmonics(b.GetEvenHarmonics());
	}
	if (a.HasOddToEvenRatio() && b.HasOddToEvenRatio())
	{
		if(b.GetOddToEvenRatio()<a.GetOddToEvenRatio())
			tmpD.SetOddToEvenRatio(b.GetOddToEvenRatio());
	}
	
	return tmpD;
}



}


#endif /* __SpectralPeakDescriptors_H__ */

