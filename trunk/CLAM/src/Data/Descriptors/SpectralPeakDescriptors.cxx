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




#include "ProcessingData.hxx"
#include "SpectralPeakDescriptors.hxx"
#include "SpectralPeakArray.hxx"
#include "CLAM_Math.hxx"

using namespace CLAM;


SpectralPeakDescriptors::SpectralPeakDescriptors(SpectralPeakArray* pSpectralPeakArray): Descriptor(eNumAttr)
{
	MandatoryInit();
	mpSpectralPeakArray=pSpectralPeakArray;
}

void SpectralPeakDescriptors::SetpSpectralPeakArray(SpectralPeakArray* pSpectralPeakArray) {
	mpSpectralPeakArray=pSpectralPeakArray;
    //TODO: it may give problems because pointer passed
	InitStats(&mpSpectralPeakArray->GetMagBuffer());

}


void SpectralPeakDescriptors::ConcreteCompute()
{

	if (HasMagnitudeMean())
		SetMagnitudeMean(mpStats->GetMean());
	if (HasHarmonicCentroid())
		SetHarmonicCentroid(mCentroid(mpSpectralPeakArray->GetMagBuffer(),
							mpSpectralPeakArray->GetFreqBuffer()));
	if(HasSpectralTilt())
		SetSpectralTilt(ComputeSpectralTilt());
	if(HasFirstTristimulus())
		SetFirstTristimulus(ComputeFirstTristimulus());
	if(HasSecondTristimulus())
		SetSecondTristimulus(ComputeSecondTristimulus());
	if(HasThirdTristimulus())
		SetThirdTristimulus(ComputeThirdTristimulus());
	if(HasHarmonicDeviation())
		SetHarmonicDeviation(ComputeHarmonicDeviation());
	if(HasOddHarmonics())
		SetOddHarmonics(ComputeOddHarmonics());
	if(HasEvenHarmonics())
		SetEvenHarmonics(ComputeEvenHarmonics());
	if(HasOddToEvenRatio())
		SetOddToEvenRatio(ComputeOddToEvenRatio());
}

/*this has been mostly copied and pasted from cuidado and should be checked and some of
it promoted into basicOps*/
TData SpectralPeakDescriptors::ComputeSpectralTilt()
{

	/* TODO check me , this computation does not seem to work*/
	TData m1;
	int i;

	TData d1=0;
	TData d2=0;
	TData ti=0;
	TData SumTi2 = 0;
	TData Tilt = 0;

	SpectralPeakArray tmpSpectralPeakArray=*mpSpectralPeakArray;
	tmpSpectralPeakArray.ToLinear();
	DataArray& mag=tmpSpectralPeakArray.GetMagBuffer();
	DataArray& pos=mpSpectralPeakArray->GetFreqBuffer();

	TData size=mag.Size();

	m1 = Mean()(mag);

	for (i=0;i<size;i++)
	  {
	d1 += pos[i]/mag[i];
	d2 += 1/mag[i];
	  }

	/* ti = m1/ai *(n - (d1/d2)) */
	/* SpecTilt = m1²/ti² * SUM[1/ai *(i-d1/d2)]  */

	for (i=0;i<size;i++) {
	  Tilt += (1/mag[i] *(pos[i]-d1/d2));
	  ti = m1/mag[i]*(pos[i] - (d1/d2));
	  SumTi2 += ti*ti;
	}

	Tilt*= (m1*m1/SumTi2);
	return Tilt;
}

TData SpectralPeakDescriptors::ComputeFirstTristimulus()
{
	TData firstHarmonicMag=mpSpectralPeakArray->GetMagBuffer()[0];
	return firstHarmonicMag*firstHarmonicMag/mpStats->GetEnergy();
}

TData SpectralPeakDescriptors::ComputeSecondTristimulus()
{
	TData secondHarmonicMag=mpSpectralPeakArray->GetMagBuffer()[1];
	TData thirdHarmonicMag=mpSpectralPeakArray->GetMagBuffer()[2];
	TData fourthHarmonicMag=mpSpectralPeakArray->GetMagBuffer()[3];
	
	return (secondHarmonicMag*secondHarmonicMag+thirdHarmonicMag*thirdHarmonicMag+
			fourthHarmonicMag*fourthHarmonicMag)/mpStats->GetEnergy();
}

TData SpectralPeakDescriptors::ComputeThirdTristimulus()
{
	DataArray& a=mpSpectralPeakArray->GetMagBuffer();
	return accumulate(a.GetPtr()+4,a.GetPtr()+a.Size(),0,PowerTmpl<2,TData>())/mpStats->GetEnergy();	
}

TData SpectralPeakDescriptors::ComputeHarmonicDeviation()
{
	TData num=0,denom=0,SE=0;
	int size=mpSpectralPeakArray->GetnPeaks();
	DataArray& data=mpSpectralPeakArray->GetMagBuffer();
	int i;
	for (i=1;i<size-1;i++)
	{
		SE=(data[i-1]+data[i]+data[i+1])/3;
		num+=data[i]-SE;
		denom+=data[i];
	}
	//we add first and last point by hand
	num+=data[0]-(data[0]+data[1])/2;
	denom+=data[0];
	num+=data[size-1]-(data[size-2]+data[size-1])/2;
	denom+=data[size-1];
	return num/denom;
}

TData SpectralPeakDescriptors::ComputeOddHarmonics()
{
	int size=mpSpectralPeakArray->GetnPeaks();
	DataArray& data=mpSpectralPeakArray->GetMagBuffer();
	int i;
	DataArray odd;
	for (i=3;i<size;i+=2)
	{
		odd.AddElem(data[i]);
	}
	return Energy()(odd)/mpStats->GetEnergy();
	
}

TData SpectralPeakDescriptors::ComputeEvenHarmonics()
{
	int size=mpSpectralPeakArray->GetnPeaks();
	DataArray& data=mpSpectralPeakArray->GetMagBuffer();
	int i;
	DataArray even;
	for (i=2;i<size;i+=2)
	{
		even.AddElem(data[i]);
	}
	return Energy()(even)/mpStats->GetEnergy();

}

TData SpectralPeakDescriptors::ComputeOddToEvenRatio()
{
	TData odd,even;
	if(HasOddHarmonics()) odd=GetOddHarmonics();
	else odd=ComputeOddHarmonics();
	if(HasEvenHarmonics()) even=GetEvenHarmonics();
	else even=ComputeEvenHarmonics();
	
	return odd/even;
}
