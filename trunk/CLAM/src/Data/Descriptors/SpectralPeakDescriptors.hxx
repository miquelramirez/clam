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
		DYN_ATTRIBUTE (0, public, TData, MagnitudeMean);
		DYN_ATTRIBUTE (1, public, TData, HarmonicCentroid);
		DYN_ATTRIBUTE (2, public, TData, SpectralTilt);
		DYN_ATTRIBUTE (3, public, TData, HarmonicDeviation);
		DYN_ATTRIBUTE (4, public, TData, FirstTristimulus);
		DYN_ATTRIBUTE (5, public, TData, SecondTristimulus);
		DYN_ATTRIBUTE (6, public, TData, ThirdTristimulus);
		DYN_ATTRIBUTE (7, public, TData, OddHarmonics);
		DYN_ATTRIBUTE (8, public, TData, EvenHarmonics);
		DYN_ATTRIBUTE (9,public, TData, OddToEvenRatio);

	public:
		SpectralPeakDescriptors(SpectralPeakArray* pSpectralPeakArray);
		SpectralPeakDescriptors(TData initVal);

		const SpectralPeakArray* GetpSpectralPeakArray() const;
		void SetpSpectralPeakArray(SpectralPeakArray* pSpectralPeakArray);
		void ConcreteCompute();

	private:
		void DefaultInit();
		void CopyInit(const SpectralPeakDescriptors & copied);
		
		TData ComputeSpectralTilt();
		TData ComputeFirstTristimulus();
		TData ComputeSecondTristimulus();
		TData ComputeThirdTristimulus();
		TData ComputeHarmonicDeviation();
		TData ComputeOddHarmonics();
		TData ComputeEvenHarmonics();
		TData ComputeOddToEvenRatio();

	private:
		SpectralPeakArray* mpSpectralPeakArray;
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
	if (a.HasSpectralTilt() && b.HasSpectralTilt())
	{
		if(b.GetSpectralTilt()>a.GetSpectralTilt())
			tmpD.SetSpectralTilt(b.GetSpectralTilt());
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
	if (a.HasSpectralTilt() && b.HasSpectralTilt())
	{
		if(b.GetSpectralTilt()<a.GetSpectralTilt())
			tmpD.SetSpectralTilt(b.GetSpectralTilt());
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
