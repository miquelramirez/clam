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
		DYNAMIC_TYPE_USING_INTERFACE (SpectralDescriptors, 27, Descriptor);
		
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
		/** Frequency of the maximun of the spectrum normalized by the spectral range */
		DYN_ATTRIBUTE (18,public, TData, LowFreqEnergyRelation); 
		/**
		 * The spectral spread is the variation of the spectrum
		 * around its mean value. It's computed from the second
		 * order moment.
		 */
		DYN_ATTRIBUTE (19,public, TData, Spread);
		DYN_ATTRIBUTE (20,public, TData, Skewness);
		/**
		 * The spectral roll-off point is the frequency so that 85%
		 * of the signal energy is contained below this
		 * frequency. Returns -1 if the rolloff point can't be
		 * found. Measured in Hz.
		 */
		DYN_ATTRIBUTE (21,public, TData, Rolloff); 
		/**
		 * The spectral slope represents the amount of decreasing of
		 * the spectral magnitude. Measured in ??.
		 */
	    DYN_ATTRIBUTE (22,public, TData, Slope); 
		DYN_ATTRIBUTE (23,public, TData, HighFrequencyCoefficient);
		DYN_ATTRIBUTE (24,public, Array<SpectralDescriptors>, BandDescriptors);

		DYN_ATTRIBUTE (25,public, Array<TData>,PCP);
		DYN_ATTRIBUTE (26, public, Array<TData>,MFCCDerivative);

	public:
		SpectralDescriptors(Spectrum* pSpectrum);
		SpectralDescriptors(TData initVal);

		const Spectrum* GetpSpectrum() const;
		void SetpSpectrum(Spectrum* pSpectrum);
		void ConcreteCompute();

//XA_C2S	private:
		void DefaultInit();
		void CopyInit(const SpectralDescriptors & copied);
		
		TData ComputeSpectralTilt();
		TData ComputeSpectralFlatness();
		TData ComputeHighFrequencyCoefficient();
		TData ComputeMaxMagFreq();
		TData ComputeLowFreqEnergyRelation();
		TData ComputeRolloff();
		TData ComputeSpread();
		TData ComputeSlope();

	private:
		Spectrum* mpSpectrum;
		
		/** Conversion from index to frequency, needed for many descriptors */
		TData mDeltaFreq;
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
	if(a.HasIrregularity() && b.HasIrregularity() )
	{
		if(b.GetIrregularity()>a.GetIrregularity())
			tmpD.SetIrregularity(b.GetIrregularity());
	}
	if(a.HasTilt() && b.HasTilt() )
	{
		if(b.GetTilt()>a.GetTilt())
			tmpD.SetTilt(b.GetTilt());
	}
	if(a.HasFlatness() && b.HasFlatness() )
	{
		if(b.GetFlatness()>a.GetFlatness())
			tmpD.SetFlatness(b.GetFlatness());
	}
	if(a.HasKurtosis() && b.HasKurtosis() )
	{
		if(b.GetKurtosis()>a.GetKurtosis())
			tmpD.SetKurtosis(b.GetKurtosis());
	}
	if(a.HasStrongPeak() && b.HasStrongPeak() )
	{
		if(b.GetStrongPeak()>a.GetStrongPeak())
			tmpD.SetStrongPeak(b.GetStrongPeak());
	}
	if(a.HasHFC() && b.HasHFC() )
	{
		if(b.GetHFC()>a.GetHFC())
			tmpD.SetHFC(b.GetHFC());
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
	if(a.HasSkewness() && b.HasSkewness() )
	{
		if(b.GetSkewness()>a.GetSkewness())
			tmpD.SetSkewness(b.GetSkewness());
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
	if(a.HasHighFrequencyCoefficient() && b.HasHighFrequencyCoefficient() )
	{
		if(b.GetHighFrequencyCoefficient()>a.GetHighFrequencyCoefficient())
			tmpD.SetHighFrequencyCoefficient(b.GetHighFrequencyCoefficient());
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
	if(a.HasBandEnergy() && b.HasBandEnergy() )
	{
		/* Array does not have these operators
		if(b.GetBandEnergy()>a.GetBandEnergy())
			tmpD.SetBandEnergy(b.GetBandEnergy());*/
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
	if(a.HasIrregularity() && b.HasIrregularity() )
	{
		if(b.GetIrregularity()<a.GetIrregularity())
			tmpD.SetIrregularity(b.GetIrregularity());
	}
	if(a.HasTilt() && b.HasTilt() )
	{
		if(b.GetTilt()<a.GetTilt())
			tmpD.SetTilt(b.GetTilt());
	}
	if(a.HasFlatness() && b.HasFlatness() )
	{
		if(b.GetFlatness()<a.GetFlatness())
			tmpD.SetFlatness(b.GetFlatness());
	}
	if(a.HasKurtosis() && b.HasKurtosis() )
	{
		if(b.GetKurtosis()<a.GetKurtosis())
			tmpD.SetKurtosis(b.GetKurtosis());
	}
	if(a.HasStrongPeak() && b.HasStrongPeak() )
	{
		if(b.GetStrongPeak()<a.GetStrongPeak())
			tmpD.SetStrongPeak(b.GetStrongPeak());
	}
	if(a.HasHFC() && b.HasHFC() )
	{
		if(b.GetHFC()<a.GetHFC())
			tmpD.SetHFC(b.GetHFC());
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
	if(a.HasSkewness() && b.HasSkewness() )
	{
		if(b.GetSkewness()<a.GetSkewness())
			tmpD.SetSkewness(b.GetSkewness());
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
	if(a.HasHighFrequencyCoefficient() && b.HasHighFrequencyCoefficient() )
	{
		if(b.GetHighFrequencyCoefficient()<a.GetHighFrequencyCoefficient())
			tmpD.SetHighFrequencyCoefficient(b.GetHighFrequencyCoefficient());
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
	if(a.HasBandEnergy() && b.HasBandEnergy() )
	{
		/* Array does not have these operators
		if(b.GetBandEnergy()<a.GetBandEnergy())
			tmpD.SetBandEnergy(b.GetBandEnergy());*/
	}
		
	return tmpD;

}

}

#endif /* __SpectralDescriptors_H__ */
