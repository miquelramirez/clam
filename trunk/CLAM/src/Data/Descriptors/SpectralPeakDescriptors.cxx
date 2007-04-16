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




#include "ProcessingData.hxx"
#include "SpectralPeakDescriptors.hxx"
#include "SpectralPeakArray.hxx"
#include "CLAM_Math.hxx"

namespace CLAM{

	DataArray Add(DataArray &a, DataArray &b);
	DataArray Multiply(TData &factor, DataArray &a);
	DataArray Multiply(DataArray &a, DataArray &b); // scalar product

SpectralPeakDescriptors::SpectralPeakDescriptors(SpectralPeakArray* pSpectralPeakArray): Descriptor(eNumAttr)
{
	CLAM_ASSERT(pSpectralPeakArray->GetScale()==EScale::eLinear,
		"Spectral Peak Descriptors require a linear magnitude SpectralPeakArray");
	MandatoryInit();
	mpSpectralPeakArray=pSpectralPeakArray;
}

SpectralPeakDescriptors::SpectralPeakDescriptors(TData initVal):Descriptor(eNumAttr)
{
	MandatoryInit();
	AddAll();
	UpdateData();
	
	SetMagnitudeMean(initVal);
	SetHarmonicCentroid(initVal);
	SetFirstTristimulus(initVal);
	SetSecondTristimulus(initVal);
	SetThirdTristimulus(initVal);
	SetHarmonicDeviation(initVal);
	SetOddHarmonics(initVal);
	SetEvenHarmonics(initVal);
	SetOddToEvenRatio(initVal);
}

void SpectralPeakDescriptors::DefaultInit() {
	mpSpectralPeakArray=0;
	mpStats=0;
}

void SpectralPeakDescriptors::CopyInit(const SpectralPeakDescriptors & copied) {
	mpSpectralPeakArray=copied.mpSpectralPeakArray;
	mpStats=0;
}

const SpectralPeakArray* SpectralPeakDescriptors::GetpSpectralPeakArray() const {
	return mpSpectralPeakArray;
}

void SpectralPeakDescriptors::SetpSpectralPeakArray(SpectralPeakArray* pSpectralPeakArray)
{
	CLAM_ASSERT(pSpectralPeakArray->GetScale()==EScale::eLinear,
		"Spectral Peak Descriptors require a linear magnitude SpectralPeakArray");
	mpSpectralPeakArray=pSpectralPeakArray;
    //TODO: it may give problems because pointer passed
	InitStats(&mpSpectralPeakArray->GetMagBuffer());
	mCentroid.Reset();
}


void SpectralPeakDescriptors::ConcreteCompute()
{

	if (HasMagnitudeMean())
		SetMagnitudeMean(mpStats->GetMean());
	if (HasHarmonicCentroid())
		SetHarmonicCentroid(ComputeCentroid());
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

TData SpectralPeakDescriptors::ComputeCentroid()
{
	unsigned size = mpSpectralPeakArray->GetnPeaks();
	if (size<=0) return 0;
	const Array<TData> & magnitudes = mpSpectralPeakArray->GetMagBuffer();
	const Array<TData> & frequencies = mpSpectralPeakArray->GetFreqBuffer();
	TData crossProduct=0.0;
	for (unsigned i = 0; i < size; i++)
	{
		crossProduct += magnitudes[i]*frequencies[i];
	}
	return crossProduct/(mpStats->GetMean()*size);
}

TData SpectralPeakDescriptors::ComputeFirstTristimulus()
{
	if (mpSpectralPeakArray->GetnPeaks()<=0) return 0;
	const TData firstHarmonicMag=mpSpectralPeakArray->GetMagBuffer()[0];
	return firstHarmonicMag*firstHarmonicMag/mpStats->GetEnergy();
}

TData SpectralPeakDescriptors::ComputeSecondTristimulus()
{
	if (mpSpectralPeakArray->GetnPeaks()<=3) return 0;

	const TData secondHarmonicMag=mpSpectralPeakArray->GetMagBuffer()[1];
	const TData thirdHarmonicMag=mpSpectralPeakArray->GetMagBuffer()[2];
	const TData fourthHarmonicMag=mpSpectralPeakArray->GetMagBuffer()[3];
	
	return (secondHarmonicMag*secondHarmonicMag+thirdHarmonicMag*thirdHarmonicMag+
			fourthHarmonicMag*fourthHarmonicMag)/mpStats->GetEnergy();
}

TData SpectralPeakDescriptors::ComputeThirdTristimulus()
{
	if (mpSpectralPeakArray->GetnPeaks()<=4) return 0;
	const DataArray& a=mpSpectralPeakArray->GetMagBuffer();
	return accumulate(a.GetPtr()+4,a.GetPtr()+a.Size(),0.,Power<2>())/mpStats->GetEnergy();	
}

TData SpectralPeakDescriptors::ComputeHarmonicDeviation()
{
	const unsigned size=mpSpectralPeakArray->GetnPeaks();
	if (size<4) return 0.0; //is it really necessary to have 4 or with 2 is enough
	const DataArray & data=mpSpectralPeakArray->GetMagBuffer();

	TData denom = 0;
	TData nom = 0;

	const TData tmp0 = log10((data[0]+data[1])/2);
	const TData logdata0=log10(data[0]);
	denom += logdata0;
	nom +=	CLAM::Abs(logdata0 - tmp0);
	
	for (unsigned i=1;i<size-1;i++)
	{
		const TData tmpi = log10((data[i-1]+data[i]+data[i+1])/3);
		const TData logdatai=log10(data[i]);
		denom += logdatai;
		nom +=	CLAM::Abs(logdatai - tmpi);
	}

	const TData tmpN = log10((data[size-2]+data[size-1])/2);
	const TData logdataN=log10(data[size-1]);
	denom += logdataN;
	nom +=	CLAM::Abs(logdataN - tmpN);

	return nom/denom;	
}

TData SpectralPeakDescriptors::ComputeOddHarmonics()
{
	const unsigned size=mpSpectralPeakArray->GetnPeaks();
	if (size<3) return 0;
	const DataArray& data=mpSpectralPeakArray->GetMagBuffer();
	TData oddEnergy = 0.0;
	for (unsigned i=2;i<size;i+=2)
	{
		oddEnergy+=data[i]*data[i];
	}
	return oddEnergy/mpStats->GetEnergy();
}

TData SpectralPeakDescriptors::ComputeEvenHarmonics()
{
	const unsigned size=mpSpectralPeakArray->GetnPeaks();
	if (size<2) return 0;
	const DataArray& data=mpSpectralPeakArray->GetMagBuffer();
	TData evenEnergy = 0.0;
	for (unsigned i=1;i<size;i+=2)
	{
		evenEnergy+=data[i]*data[i];
	}
	return evenEnergy/mpStats->GetEnergy();
}

TData SpectralPeakDescriptors::ComputeOddToEvenRatio()
{
	if (mpSpectralPeakArray->GetnPeaks()<=1) return 0.5;
	TData odd,even;
	if (HasOddHarmonics()) odd=GetOddHarmonics();
	else odd=ComputeOddHarmonics();
	if (HasEvenHarmonics()) even=GetEvenHarmonics();
	else even=ComputeEvenHarmonics();

	return odd/(even+odd);
}

SpectralPeakDescriptors operator * (const SpectralPeakDescriptors& a,TData mult) 
{
	SpectralPeakDescriptors tmpD(a);

	if (a.HasMagnitudeMean())
	{
		tmpD.SetMagnitudeMean(a.GetMagnitudeMean()*mult);
	}
	if (a.HasHarmonicCentroid())
	{
		tmpD.SetHarmonicCentroid(a.GetHarmonicCentroid()*mult);
	}
	if (a.HasFirstTristimulus())
	{
		tmpD.SetFirstTristimulus(a.GetFirstTristimulus()*mult);
	}
	if (a.HasSecondTristimulus())
	{
		tmpD.SetSecondTristimulus(a.GetSecondTristimulus()*mult);
	}
	if (a.HasThirdTristimulus())
	{
		tmpD.SetThirdTristimulus(a.GetThirdTristimulus()*mult);
	}
	if (a.HasHarmonicDeviation())
	{
		tmpD.SetHarmonicDeviation(a.GetHarmonicDeviation()*mult);
	}
	if (a.HasOddHarmonics())
	{
		tmpD.SetOddHarmonics(a.GetOddHarmonics()*mult);
	}
	if (a.HasEvenHarmonics())
	{
		tmpD.SetEvenHarmonics(a.GetEvenHarmonics()*mult);
	}
	if (a.HasOddToEvenRatio())
	{
		tmpD.SetOddToEvenRatio(a.GetOddToEvenRatio()*mult);
	}
	if(a.HasHPCP())
		tmpD.SetHPCP(Multiply(mult,a.GetHPCP()));

	return tmpD;
}

SpectralPeakDescriptors operator * (const SpectralPeakDescriptors& a,const SpectralPeakDescriptors& b) 
{
	SpectralPeakDescriptors tmpD;

	if (a.HasMagnitudeMean() && b.HasMagnitudeMean())
	{
		tmpD.AddMagnitudeMean();
		tmpD.UpdateData();
		tmpD.SetMagnitudeMean(a.GetMagnitudeMean()*b.GetMagnitudeMean());
	}
	if (a.HasHarmonicCentroid() && b.HasHarmonicCentroid())
	{
		tmpD.AddHarmonicCentroid();
		tmpD.UpdateData();
		tmpD.SetHarmonicCentroid(a.GetHarmonicCentroid()*b.GetHarmonicCentroid());
	}
	if (a.HasFirstTristimulus() && b.HasFirstTristimulus())
	{
		tmpD.AddFirstTristimulus();
		tmpD.UpdateData();
		tmpD.SetFirstTristimulus(a.GetFirstTristimulus()*b.GetFirstTristimulus());
	}
	if (a.HasSecondTristimulus() && b.HasSecondTristimulus())
	{
		tmpD.AddSecondTristimulus();
		tmpD.UpdateData();
		tmpD.SetSecondTristimulus(a.GetSecondTristimulus()*b.GetSecondTristimulus());
	}
	if (a.HasThirdTristimulus() && b.HasThirdTristimulus())
	{
		tmpD.AddThirdTristimulus();
		tmpD.UpdateData();
		tmpD.SetThirdTristimulus(a.GetThirdTristimulus()*b.GetThirdTristimulus());
	}
	if (a.HasHarmonicDeviation() && b.HasHarmonicDeviation())
	{
		tmpD.AddHarmonicDeviation();
		tmpD.UpdateData();
		tmpD.SetHarmonicDeviation(a.GetHarmonicDeviation()*b.GetHarmonicDeviation());
	}
	if (a.HasOddHarmonics() && b.HasOddHarmonics())
	{
		tmpD.AddOddHarmonics();
		tmpD.UpdateData();
		tmpD.SetOddHarmonics(a.GetOddHarmonics()*b.GetOddHarmonics());
	}
	if (a.HasEvenHarmonics() && b.HasEvenHarmonics())
	{
		tmpD.AddEvenHarmonics();
		tmpD.UpdateData();
		tmpD.SetEvenHarmonics(a.GetEvenHarmonics()*b.GetEvenHarmonics());
	}
	if (a.HasOddToEvenRatio() && b.HasOddToEvenRatio())
	{
		tmpD.AddOddToEvenRatio();
		tmpD.UpdateData();
		tmpD.SetOddToEvenRatio(a.GetOddToEvenRatio()*b.GetOddToEvenRatio());
	}
	if (a.HasHPCP() && b.HasHPCP())
	{
		tmpD.AddHPCP();
		tmpD.UpdateData();
		tmpD.SetHPCP(Multiply(a.GetHPCP(),b.GetHPCP()));
	}
	
	return tmpD;
}

SpectralPeakDescriptors operator + (const SpectralPeakDescriptors& a,const SpectralPeakDescriptors& b)
{
	SpectralPeakDescriptors tmpD;

	if (a.HasMagnitudeMean() && b.HasMagnitudeMean())
	{
		tmpD.AddMagnitudeMean();
		tmpD.UpdateData();
		tmpD.SetMagnitudeMean(a.GetMagnitudeMean()+b.GetMagnitudeMean());
	}
	if (a.HasHarmonicCentroid() && b.HasHarmonicCentroid())
	{
		tmpD.AddHarmonicCentroid();
		tmpD.UpdateData();
		tmpD.SetHarmonicCentroid(a.GetHarmonicCentroid()+b.GetHarmonicCentroid());
	}
	if (a.HasFirstTristimulus() && b.HasFirstTristimulus())
	{
		tmpD.AddFirstTristimulus();
		tmpD.UpdateData();
		tmpD.SetFirstTristimulus(a.GetFirstTristimulus()+b.GetFirstTristimulus());
	}
	if (a.HasSecondTristimulus() && b.HasSecondTristimulus())
	{
		tmpD.AddSecondTristimulus();
		tmpD.UpdateData();
		tmpD.SetSecondTristimulus(a.GetSecondTristimulus()+b.GetSecondTristimulus());
	}
	if (a.HasThirdTristimulus() && b.HasThirdTristimulus())
	{
		tmpD.AddThirdTristimulus();
		tmpD.UpdateData();
		tmpD.SetThirdTristimulus(a.GetThirdTristimulus()+b.GetThirdTristimulus());
	}
	if (a.HasHarmonicDeviation() && b.HasHarmonicDeviation())
	{
		tmpD.AddHarmonicDeviation();
		tmpD.UpdateData();
		tmpD.SetHarmonicDeviation(a.GetHarmonicDeviation()+b.GetHarmonicDeviation());
	}
	if (a.HasOddHarmonics() && b.HasOddHarmonics())
	{
		tmpD.AddOddHarmonics();
		tmpD.UpdateData();
		tmpD.SetOddHarmonics(a.GetOddHarmonics()+b.GetOddHarmonics());
	}
	if (a.HasEvenHarmonics() && b.HasEvenHarmonics())
	{
		tmpD.AddEvenHarmonics();
		tmpD.UpdateData();
		tmpD.SetEvenHarmonics(a.GetEvenHarmonics()+b.GetEvenHarmonics());
	}
	if (a.HasOddToEvenRatio() && b.HasOddToEvenRatio())
	{
		tmpD.AddOddToEvenRatio();
		tmpD.UpdateData();
		tmpD.SetOddToEvenRatio(a.GetOddToEvenRatio()+b.GetOddToEvenRatio());
	}
	if(a.HasHPCP() && b.HasHPCP() )
	{
		tmpD.AddHPCP();
		tmpD.UpdateData();
		tmpD.SetHPCP(Add(a.GetHPCP(),b.GetHPCP()));
	}

	
	return tmpD;
}

SpectralPeakDescriptors operator / (const SpectralPeakDescriptors& a,TData div) 
{
	return a*(1/div);
}

SpectralPeakDescriptors operator * (TData mult, const SpectralPeakDescriptors& a) 
{
	return a*mult;
}

}//CLAM

