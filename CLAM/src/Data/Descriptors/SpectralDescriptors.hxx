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

#ifndef __SpectralDescriptors_H__
#define __SpectralDescriptors_H__


#include "Array.hxx"
#include "Descriptor.hxx"
#include "Spectrum.hxx"

/*
 * This class holds Descriptors computed from Spectral data 
 * 
 *
 */

namespace CLAM {

  	class SpectralDescriptors : public Descriptor {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralDescriptors, 21, Descriptor);
		
		/** The spectral power mean value.
		 *  The unit of this measure can be dB
		 *  or none, depending on the scale set for the
		 *  measured Spectrum object.
		 *  @see Spectrum::SetScale
		 *  @see EScale
		 *  @see Stats::GetMean
		 */
		DYN_ATTRIBUTE (0, public, TData, Mean);
		/**
		 * The geometric mean for the spectral power values sequence.
		 * See <a href="http://mathworld.wolfram.com/GeometricMean.html">this</a> for a definition of
		 * this pythagorean mean. Note that computing this measurement over long sequences of
		 * small real numbers ( as the ones one usually founds in spectral power distributions derived
		 * of audio windowed with a normalized window function ) pose a numerical problem. To avoid
		 * this, computation of Geometric mean is restricted to Log scale Spectral Power Distributions 
		 * since this allows to change the product for a summation.
		 *
		 * This measure is expressed in dBs.
		 * @see Stats::GetGeometricMean
		 */
		DYN_ATTRIBUTE (1, public, TData, GeometricMean);
		/**
		 *  The squared sum of spectral power distribution values.
		 *  This measure comes in the same units as the distribution
		 *  values.
		 *  @see Stats::GetEnergy
		 */
		DYN_ATTRIBUTE (2, public, TData, Energy);
		/**
		 *  The frequency where the center of mass of the spectral power
		 *  distribution lies.
		 *  This measure is expressed in Hz.
		 * 
		 *  @see Stats::GetCentroid
		 */
		DYN_ATTRIBUTE (3, public, TData, Centroid);
		DYN_ATTRIBUTE (4, public, TData, Moment2);
		DYN_ATTRIBUTE (5, public, TData, Moment3);
		DYN_ATTRIBUTE (6, public, TData, Moment4);
		DYN_ATTRIBUTE (7, public, TData, Moment5);
		DYN_ATTRIBUTE (8, public, TData, Moment6);
		DYN_ATTRIBUTE (9, public, TData, Flatness);
		DYN_ATTRIBUTE (10,public, TData, MagnitudeKurtosis);
		DYN_ATTRIBUTE (11,public, Array<TData>, MFCC);
		/**
		 * Frequency of the maximum magnitude of the spectrum 
		 * normalized by the spectral range
		 */
		DYN_ATTRIBUTE (12,public, TData, MaxMagFreq); 
		/**
		 * The ratio between the energy over 0-100 Hz band and the whole spectrum energy.
		 * To avoid singularities while keeping descriptor continuity, 
		 * when the whole spectrum energy drops bellow $10^{-4}$, 
		 * such value is considered as whole spectrum energy.
		 */
		DYN_ATTRIBUTE (13,public, TData, LowFreqEnergyRelation); 
		/**
		 * The spectral spread is the variation of the spectrum
		 * around its mean value. It's computed from the second
		 * order moment.
		 */
		DYN_ATTRIBUTE (14,public, TData, Spread);
		DYN_ATTRIBUTE (15,public, TData, MagnitudeSkewness);
		/**
		 * The spectral roll-off point is the frequency value 
		 * so that the 85% of the spectral energy is contained below 
		 * it. For silences this is 0Hz. Measured in Hz.
		 *
		 * \f[
		 * Rolloff / \sum_{f=0}^{RollOff} {a_f^2} = 0.85 \times \sum_{f=0}^{SpectralRange} {a_f^2}
		 * \f]
		 */
		DYN_ATTRIBUTE (16,public, TData, Rolloff); 
		/**
		 * The spectral slope represents the amount of decreasing of
		 * the spectral magnitude. Measured in ??.
		 * @see Stats::Slope
		 */
		DYN_ATTRIBUTE (17,public, TData, Slope);
		/**
		 * Sum of the squared spectrum magnitude multiplied by the wave number of the bin.
		 * It could be considered the energy derivative, a high pass filter,
		 * which gives higher values for high frequency content.
		 *
		 * \f[
		 * HighFrequencyContent = \sum_{i=0}^{nBins} i magnitude_i^2
		 * \f]
		 */
		DYN_ATTRIBUTE (18,public, TData, HighFrequencyContent);
		DYN_ATTRIBUTE (19,public, Array<SpectralDescriptors>, BandDescriptors);

		DYN_ATTRIBUTE (20,public, Array<TData>,PCP);

	public:
		SpectralDescriptors(Spectrum* pSpectrum);
		SpectralDescriptors(TData initVal);

		const Spectrum* GetpSpectrum() const;
		void SetpSpectrum(Spectrum* pSpectrum);
		void ConcreteCompute();

//XA_C2S	private:
		void DefaultInit();
		void CopyInit(const SpectralDescriptors & copied);
		
		TData ComputeSpectralFlatness();
		TData ComputeHighFrequencyContent();
		TData ComputeMaxMagFreq();
		TData ComputeLowFreqEnergyRelation();
		TData ComputeRolloff();
		TData ComputeSpread();
		TData ComputeSlope();

	private:
		const Spectrum* mpSpectrum;
		/** Conversion from index to frequency, needed for many descriptors */
		double mDeltaFreq; // double because a lot of computations depends on its precission
};

SpectralDescriptors operator * (const SpectralDescriptors& a,TData mult);
SpectralDescriptors operator * (TData mult,const SpectralDescriptors& a);
SpectralDescriptors operator / (const SpectralDescriptors& a,TData div);
SpectralDescriptors operator * (const SpectralDescriptors& a,const SpectralDescriptors& b) ;
SpectralDescriptors operator + (const SpectralDescriptors& a, const SpectralDescriptors& b);

template<>
inline SpectralDescriptors CLAM_max (const SpectralDescriptors& a,const SpectralDescriptors& b)
{
	SpectralDescriptors  tmpD(a);
	if(a.HasMean() && b.HasMean() )
	{
		if(b.GetMean()>a.GetMean())
			tmpD.SetMean(b.GetMean());
	}
	if(a.HasGeometricMean() && b.HasGeometricMean() )
	{
		if(b.GetGeometricMean()>a.GetGeometricMean())
			tmpD.SetGeometricMean(b.GetGeometricMean());
	}
	if(a.HasEnergy() && b.HasEnergy() )
	{
		if(b.GetEnergy()>a.GetEnergy())
			tmpD.SetEnergy(b.GetEnergy());
	}
	if(a.HasCentroid() && b.HasCentroid() )
	{
		if(b.GetCentroid()>a.GetCentroid())
			tmpD.SetCentroid(b.GetCentroid());
	}
	if(a.HasMoment2() && b.HasMoment2() )
	{
		if(b.GetMoment2()>a.GetMoment2())
			tmpD.SetMoment2(b.GetMoment2());
	}
	if(a.HasMoment3() && b.HasMoment3() )
	{
		if(b.GetMoment3()>a.GetMoment3())
			tmpD.SetMoment3(b.GetMoment3());
	}
	if(a.HasMoment4() && b.HasMoment4() )
	{
		if(b.GetMoment4()>a.GetMoment4())
			tmpD.SetMoment4(b.GetMoment4());
	}
	if(a.HasMoment5() && b.HasMoment5())
	{
		if(b.GetMoment5()>a.GetMoment5())
			tmpD.SetMoment5(b.GetMoment5());
	}
	if(a.HasMoment6() && b.HasMoment6() )
	{
		if(b.GetMoment6()>a.GetMoment6())
			tmpD.SetMoment6(b.GetMoment6());
	}
	if(a.HasFlatness() && b.HasFlatness() )
	{
		if(b.GetFlatness()>a.GetFlatness())
			tmpD.SetFlatness(b.GetFlatness());
	}
	if(a.HasMagnitudeKurtosis() && b.HasMagnitudeKurtosis() )
	{
		if(b.GetMagnitudeKurtosis()>a.GetMagnitudeKurtosis())
			tmpD.SetMagnitudeKurtosis(b.GetMagnitudeKurtosis());
	}
	if(a.HasMaxMagFreq() && b.HasMaxMagFreq() )
	{
		if(b.GetMaxMagFreq()>a.GetMaxMagFreq())
			tmpD.SetMaxMagFreq(b.GetMaxMagFreq());
	}
	if(a.HasLowFreqEnergyRelation() && b.HasLowFreqEnergyRelation() )
	{
		if(b.GetLowFreqEnergyRelation()>a.GetLowFreqEnergyRelation())
			tmpD.SetLowFreqEnergyRelation(b.GetLowFreqEnergyRelation());
	}
	if(a.HasSpread() && b.HasSpread() )
	{
		if(b.GetSpread()>a.GetSpread())
			tmpD.SetSpread(b.GetSpread());
	}
	if(a.HasMagnitudeSkewness() && b.HasMagnitudeSkewness() )
	{
		if(b.GetMagnitudeSkewness()>a.GetMagnitudeSkewness())
			tmpD.SetMagnitudeSkewness(b.GetMagnitudeSkewness());
	}
	if(a.HasRolloff() && b.HasRolloff() )
	{
		if(b.GetRolloff()>a.GetRolloff())
			tmpD.SetRolloff(b.GetRolloff());
	}
	if(a.HasSlope() && b.HasSlope() )
	{
		if(b.GetSlope()>a.GetSlope())
			tmpD.SetSlope(b.GetSlope());
	}
	if(a.HasHighFrequencyContent() && b.HasHighFrequencyContent() )
	{
		if(b.GetHighFrequencyContent()>a.GetHighFrequencyContent())
			tmpD.SetHighFrequencyContent(b.GetHighFrequencyContent());
	}
	if(a.HasBandDescriptors() && b.HasBandDescriptors() )
	{
		/* Array does not have these operators
		
		  if(b.GetBandDescriptors()>a.GetBandDescriptors())
			tmpD.SetBandDescriptors(b.GetBandDescriptors() );*/
	}
	if(a.HasMFCC() && b.HasMFCC() )
	{
		/* Array does not have these operators
		if(b.GetMFCC()>a.GetMFCC())
			tmpD.SetMFCC(b.GetMFCC());*/
	}
	return tmpD;

}

template<>
inline SpectralDescriptors CLAM_min (const SpectralDescriptors& a,const SpectralDescriptors& b)
{
	SpectralDescriptors  tmpD(a);
	if(a.HasMean() && b.HasMean() )
	{
		if(b.GetMean()<a.GetMean())
			tmpD.SetMean(b.GetMean());
	}
	if(a.HasGeometricMean() && b.HasGeometricMean() )
	{
		if(b.GetGeometricMean()<a.GetGeometricMean())
			tmpD.SetGeometricMean(b.GetGeometricMean());
	}
	if(a.HasEnergy() && b.HasEnergy() )
	{
		if(b.GetEnergy()<a.GetEnergy())
			tmpD.SetEnergy(b.GetEnergy());
	}
	if(a.HasCentroid() && b.HasCentroid() )
	{
		if(b.GetCentroid()<a.GetCentroid())
			tmpD.SetCentroid(b.GetCentroid());
	}
	if(a.HasMoment2() && b.HasMoment2() )
	{
		if(b.GetMoment2()<a.GetMoment2())
			tmpD.SetMoment2(b.GetMoment2());
	}
	if(a.HasMoment3() && b.HasMoment3() )
	{
		if(b.GetMoment3()<a.GetMoment3())
			tmpD.SetMoment3(b.GetMoment3());
	}
	if(a.HasMoment4() && b.HasMoment4() )
	{
		if(b.GetMoment4()<a.GetMoment4())
			tmpD.SetMoment4(b.GetMoment4());
	}
	if(a.HasMoment5() && b.HasMoment5())
	{
		if(b.GetMoment5()<a.GetMoment5())
			tmpD.SetMoment5(b.GetMoment5());
	}
	if(a.HasMoment6() && b.HasMoment6() )
	{
		if(b.GetMoment6()<a.GetMoment6())
			tmpD.SetMoment6(b.GetMoment6());
	}
	if(a.HasFlatness() && b.HasFlatness() )
	{
		if(b.GetFlatness()<a.GetFlatness())
			tmpD.SetFlatness(b.GetFlatness());
	}
	if(a.HasMagnitudeKurtosis() && b.HasMagnitudeKurtosis() )
	{
		if(b.GetMagnitudeKurtosis()<a.GetMagnitudeKurtosis())
			tmpD.SetMagnitudeKurtosis(b.GetMagnitudeKurtosis());
	}
	if(a.HasMaxMagFreq() && b.HasMaxMagFreq() )
	{
		if(b.GetMaxMagFreq()<a.GetMaxMagFreq())
			tmpD.SetMaxMagFreq(b.GetMaxMagFreq());
	}
	if(a.HasLowFreqEnergyRelation() && b.HasLowFreqEnergyRelation() )
	{
		if(b.GetLowFreqEnergyRelation()<a.GetLowFreqEnergyRelation())
			tmpD.SetLowFreqEnergyRelation(b.GetLowFreqEnergyRelation());
	}
	if(a.HasSpread() && b.HasSpread() )
	{
		if(b.GetSpread()<a.GetSpread())
			tmpD.SetSpread(b.GetSpread());
	}
	if(a.HasMagnitudeSkewness() && b.HasMagnitudeSkewness() )
	{
		if(b.GetMagnitudeSkewness()<a.GetMagnitudeSkewness())
			tmpD.SetMagnitudeSkewness(b.GetMagnitudeSkewness());
	}
	if(a.HasRolloff() && b.HasRolloff() )
	{
		if(b.GetRolloff()<a.GetRolloff())
			tmpD.SetRolloff(b.GetRolloff());
	}
	if(a.HasSlope() && b.HasSlope() )
	{
		if(b.GetSlope()<a.GetSlope())
			tmpD.SetSlope(b.GetSlope());
	}
	if(a.HasHighFrequencyContent() && b.HasHighFrequencyContent() )
	{
		if(b.GetHighFrequencyContent()<a.GetHighFrequencyContent())
			tmpD.SetHighFrequencyContent(b.GetHighFrequencyContent());
	}
	if(a.HasBandDescriptors() && b.HasBandDescriptors() )
	{
		/* Array does not have these operators
		if(b.GetBandDescriptors()<a.GetBandDescriptors())
			tmpD.SetBandDescriptors(b.GetBandDescriptors() );*/
	}
	if(a.HasMFCC() && b.HasMFCC() )
	{
		/* Array does not have these operators
		if(b.GetMFCC()<a.GetMFCC())
			tmpD.SetMFCC(b.GetMFCC());*/
	}

	return tmpD;

}

}

#endif /* __SpectralDescriptors_H__ */

