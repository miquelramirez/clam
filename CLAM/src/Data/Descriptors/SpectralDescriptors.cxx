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

#include "SpectralDescriptors.hxx"
#include "Spectrum.hxx"
#include "Array.hxx"
#include <algorithm>

namespace CLAM{

SpectralDescriptors::SpectralDescriptors(Spectrum* pSpectrum):Descriptor(eNumAttr)
{
	MandatoryInit();
	mpSpectrum=pSpectrum;
}

SpectralDescriptors::SpectralDescriptors(TData initVal):Descriptor(eNumAttr)
{
	MandatoryInit();
	AddAll();
	UpdateData();
	SetMean(initVal);
	SetGeometricMean(initVal);
	SetEnergy(initVal);
	SetCentroid(initVal);
	SetMoment2(initVal);
	SetMoment3(initVal);	
	SetMoment4(initVal);
	SetMoment5(initVal);
	SetMoment6(initVal);
	SetSpread(initVal);
	SetSkewness(initVal);
	SetKurtosis(initVal);
	SetTilt(initVal);
	SetFlatness(initVal);
	SetHighFrequencyCoefficient(initVal);
	SetMaxMagFreq(initVal);
	SetLowFreqEnergyRelation(initVal);
	SetRolloff(initVal);
	SetSlope(initVal);
	SetIrregularity(initVal);
	SetStrongPeak(initVal);
	SetHFC(initVal);
}

void SpectralDescriptors::DefaultInit() {
	mpSpectrum=0;
	mpStats=0;
	//Warning: no attributes are added by default, the user is in charge of adding the ones he is interested in
}

void SpectralDescriptors::CopyInit(const SpectralDescriptors & copied) {
	mpSpectrum=copied.mpSpectrum;
	mpStats=0;
}

const Spectrum* SpectralDescriptors::GetpSpectrum() const {
	return mpSpectrum;
}

void SpectralDescriptors::SetpSpectrum(Spectrum* pSpectrum) {
	mpSpectrum=pSpectrum;
	//TODO: we are asuming Spectrum is in MagBuffer
    //TODO: it may give problems because pointer passed
	InitStats(&mpSpectrum->GetMagBuffer());

	mDeltaFreq=mpSpectrum->GetSpectralRange()/mpSpectrum->GetSize();
	
}

void SpectralDescriptors::ConcreteCompute()
{
	if (HasMean())
		SetMean(mpStats->GetMean());
	if (HasGeometricMean())
		SetGeometricMean(mpStats->GetGeometricMean());
	if (HasEnergy())
		SetEnergy(mpStats->GetEnergy());
	if (HasCentroid())
		SetCentroid(mpStats->GetCentroid()*mDeltaFreq);
	if(HasMoment2())
		SetMoment2(mpStats->GetMoment(SecondOrder)*mDeltaFreq);
	if(HasMoment3())
		SetMoment3(mpStats->GetMoment(ThirdOrder)*mDeltaFreq);	
	if(HasMoment4())
		SetMoment4(mpStats->GetMoment(FourthOrder)*mDeltaFreq);
	if(HasMoment5())
		SetMoment5(mpStats->GetMoment(FifthOrder)*mDeltaFreq);
	if(HasMoment6())
		SetMoment6(mpStats->GetMoment((O<6>*)(0))*mDeltaFreq);
	if (HasSpread())
	        SetSpread(ComputeSpread());
	if(HasSkewness())
		SetSkewness(mpStats->GetSkew());
	if(HasKurtosis())	
		SetKurtosis(mpStats->GetKurtosis());
	if(HasTilt())
		SetTilt(ComputeSpectralTilt());
	if(HasFlatness())
		SetFlatness(ComputeSpectralFlatness());
	if(HasHighFrequencyCoefficient())
		SetHighFrequencyCoefficient(ComputeHighFrequencyCoefficient());
	if(HasMaxMagFreq())
		SetMaxMagFreq(ComputeMaxMagFreq());
	if(HasLowFreqEnergyRelation())
		SetLowFreqEnergyRelation(ComputeLowFreqEnergyRelation());
	if(HasRolloff())
		SetRolloff(ComputeRolloff());
	if(HasSlope())
		SetSlope(ComputeSlope());
	if(HasIrregularity())
		//not implemented
		SetIrregularity(0);
	if(HasStrongPeak())
		//not implemented
		SetStrongPeak(0);
	if(HasHFC())
		//not implemented
		SetHFC(0);
}

void SpectralDescriptors::SetPrototype(const SpectralDescriptors& p)
{
	*this=p;
}

/*this has been mostly copied and pasted from cuidado and should be checked and some of
it promoted into basicOps*/
TData SpectralDescriptors::ComputeSpectralTilt()
{

	/* TODO check me check me check me check me check me check me check me */
	TData m1;
	int i;

	TData d1=0;
	TData d2=0;
	TData ti=0;
	TData SumTi2 = 0;
	TData Tilt = 0;

	DataArray& mag=mpSpectrum->GetMagBuffer();

	m1 = mpStats->GetMoment(FirstOrder);

	TData size=mag.Size();

	for (i=0;i<size;i++)
	  {
	d1 += i/mag[i];
	d2 += 1/mag[i];
	  }

	/* ti = m1/ai *(n - (d1/d2)) */
	/* SpecTilt = m1²/ti² * SUM[1/ai *(i-d1/d2)]  */

	for (i=0;i<size;i++) {
	  Tilt += (1/mag[i] *(i-d1/d2));
	  ti = m1/mag[i]*(i - (d1/d2));
	  SumTi2 += ti*ti;
	}

	Tilt*= (m1*m1/SumTi2);
	return Tilt;
}

/*this has been mostly copied and pasted from cuidado and should be checked and some of
it promoted into basicOps*/
TData SpectralDescriptors::ComputeSpectralFlatness()
{
	TData  mean = mpStats->GetMean();

	TData geomean = mpStats->GetGeometricMean();

	return 10*log(geomean/mean);
}

/*this has been mostly copied and pasted from cuidado and should be checked and some of
it promoted into basicOps*/
TData SpectralDescriptors::ComputeHighFrequencyCoefficient()
{
	/*int i;
	TData temp = 0;
	for (i=1;i<size;i++)
	  temp += pow(mag[i],2)*i;*/ 
	return WeightedPoweredSum<2>()(mpSpectrum->GetMagBuffer());
}

/*this has been mostly copied and pasted from cuidado and should be checked and some of
it promoted into basicOps*/
TData SpectralDescriptors::ComputeMaxMagFreq()
{ 
	// Frequency of the spectrum maxima 
	// Note: it is supposing the spectrum is in dB?
	int i;
	TData max = -1000.0;
	TIndex index = -1;
	
	DataArray& data=mpSpectrum->GetMagBuffer();
	int size=mpSpectrum->GetSize();
	for(i=0; i<size; i++) 
		if(data[i] > max ) {
			max = data[i];
			index = i;
		} 
	// Normalized by the spectral range
	return (TData) index* (mpSpectrum->GetSpectralRange()/(TData)(size-1));
}

/*this has been mostly copied and pasted from cuidado and should be checked and some of
it promoted into basicOps*/
TData SpectralDescriptors::ComputeLowFreqEnergyRelation() 
{ 
	// Energy(0-100 Hz) / Total Energy
	int size=mpSpectrum->GetSize();
	TIndex index = Round(100.0/(mpSpectrum->GetSpectralRange()/(TData)size));
	
	DataArray& data=mpSpectrum->GetMagBuffer();
	
	data.SetSize(index);

	Energy energyComputer;

	TData result=(energyComputer(data)/mpStats->GetEnergy() );
	data.SetSize(size);
	return result;
}

/*this has been mostly copied and pasted from cuidado and should be checked and some of
it promoted into basicOps*/
TData SpectralDescriptors::ComputeRolloff() 
{ 
	// Frequency f below which 85% of the magnitude distribution is concentrated
	TIndex i = 0;
	TData m = fabsf(data[0]);

	TData zeroMoment=mpStats->GetMoment(FirstOrder);
	zeroMoment*=0.85;
	int size=mpSpectrum->GetSize();
	DataArray& data=mpSpectrum->GetMagBuffer();

	for ( i = 0; ( m < zeroMoment ) && ( i < size ); i++ )
	{
		m += fabsf(data[i]);
	}
	 
	return i;
}


TData SpectralDescriptors::ComputeSpread() 
{ 
	DataArray& mags     = mpSpectrum->GetMagBuffer();
	TSize      magsSize = mpSpectrum->GetSize();

	TData centroid = mpStats->GetCentroid()*mDeltaFreq;

	// Compute spectrum variance around centroid frequency
	TData variance = 0;
	TData sumMags  = 0;
	for (TIndex i=0; i<magsSize; i++)
	{
		variance += pow((i*mDeltaFreq - centroid), 2) * mags[i];
		sumMags  += mags[i];
	}
	variance /= sumMags;

	// Return std.dev. normalized by centroid frequency
	return sqrt(variance) / centroid;
}


TData SpectralDescriptors::ComputeSlope() 
{ 
	DataArray& mags     = mpSpectrum->GetMagBuffer();
	TSize      magsSize = mpSpectrum->GetSize();

	// Compute means and gradient of decay part
	TData meanX = 0;
	TData meanY = 0;
	TData num   = 0;
	TData denum = 0;

	for (TIndex i=0; i<magsSize; i++)
	{
		meanX += (i*mDeltaFreq);
		meanY += mags[i];

		num   += (i*mDeltaFreq)*mags[i];
		denum += pow(i*mDeltaFreq, 2);
	}
	meanX /= magsSize;
	meanY /= magsSize;

	num   -= magsSize*meanX*meanY;
	denum -= magsSize*meanX*meanX;

	// Normalize by the total amplitude
	return (num/denum) / (meanY*magsSize);
}


SpectralDescriptors operator * (const SpectralDescriptors& a,TData mult)
{
	SpectralDescriptors  tmpD(a);
	if(a.HasMean())
		tmpD.SetMean(a.GetMean()*mult);
	if(a.HasGeometricMean())
		tmpD.SetGeometricMean(a.GetGeometricMean()*mult);
	if(a.HasEnergy())
		tmpD.SetEnergy(a.GetEnergy()*mult);
	if(a.HasCentroid())
		tmpD.SetCentroid(a.GetCentroid()*mult);
	if(a.HasMoment2())
		tmpD.SetMoment2(a.GetMoment2()*mult);
	if(a.HasMoment3())
		tmpD.SetMoment3(a.GetMoment3()*mult);
	if(a.HasMoment4())
		tmpD.SetMoment4(a.GetMoment4()*mult);
	if(a.HasMoment5())
		tmpD.SetMoment5(a.GetMoment5()*mult);
	if(a.HasMoment6())
		tmpD.SetMoment6(a.GetMoment6()*mult);
	if(a.HasIrregularity())
		tmpD.SetIrregularity(a.GetIrregularity()*mult);
	if(a.HasTilt())
		tmpD.SetTilt(a.GetTilt()*mult);
	if(a.HasFlatness())
		tmpD.SetFlatness(a.GetFlatness()*mult);
	if(a.HasKurtosis())
		tmpD.SetKurtosis(a.GetKurtosis()*mult);
	if(a.HasStrongPeak())
		tmpD.SetStrongPeak(a.GetStrongPeak()*mult);
	if(a.HasHFC())
		tmpD.SetHFC(a.GetHFC()*mult);
	if(a.HasMaxMagFreq())
		tmpD.SetMaxMagFreq(a.GetMaxMagFreq()*mult);
	if(a.HasLowFreqEnergyRelation())
		tmpD.SetLowFreqEnergyRelation(a.GetLowFreqEnergyRelation()*mult);
	if(a.HasSpread())
		tmpD.SetSpread(a.GetSpread()*mult);
	if(a.HasSkewness())
		tmpD.SetSkewness(a.GetSkewness()*mult);
	if(a.HasRolloff())
		tmpD.SetRolloff(a.GetRolloff()*mult);
	if(a.HasSlope())
		tmpD.SetSlope(a.GetSlope()*mult);
	if(a.HasHighFrequencyCoefficient())
		tmpD.SetHighFrequencyCoefficient(a.GetHighFrequencyCoefficient()*mult);
	if(a.HasBandDescriptors())
		//todo!!! We are not multiplying because we would need the operator implemented in the array
		tmpD.SetBandDescriptors(a.GetBandDescriptors());
	if(a.HasMFCC())
		//todo!!! We are not multiplying because we would need the operator implemented in the array
		tmpD.SetMFCC(a.GetMFCC());
	if(a.HasBandEnergy())
		//todo!!! We are not multiplying because we would need the operator implemented in the array
		tmpD.SetBandEnergy(a.GetBandEnergy());
	return tmpD;
}

SpectralDescriptors operator * (const SpectralDescriptors& a,const SpectralDescriptors& b)
{
	SpectralDescriptors  tmpD;
	if(a.HasMean() && b.HasMean() )
	{
		tmpD.AddMean();
		tmpD.UpdateData();
		tmpD.SetMean(a.GetMean()*b.GetMean());
	}
	if(a.HasGeometricMean() && b.HasGeometricMean() )
	{
		tmpD.AddGeometricMean();
		tmpD.UpdateData();
		tmpD.SetGeometricMean(a.GetGeometricMean()*b.GetGeometricMean());
	}
	if(a.HasEnergy() && b.HasEnergy() )
	{
		tmpD.AddEnergy();
		tmpD.UpdateData();
		tmpD.SetEnergy(a.GetEnergy()*b.GetEnergy());
	}
	if(a.HasCentroid() && b.HasCentroid() )
	{
		tmpD.AddCentroid();
		tmpD.UpdateData();
		tmpD.SetCentroid(a.GetCentroid()*b.GetCentroid());
	}
	if(a.HasMoment2() && b.HasMoment2() )
	{
		tmpD.AddMoment2();
		tmpD.UpdateData();
		tmpD.SetMoment2(a.GetMoment2()*b.GetMoment2());
	}
	if(a.HasMoment3() && b.HasMoment3() )
	{
		tmpD.AddMoment3();
		tmpD.UpdateData();
		tmpD.SetMoment3(a.GetMoment3()*b.GetMoment3());
	}
	if(a.HasMoment4() && b.HasMoment4() )
	{
		tmpD.AddMoment4();
		tmpD.UpdateData();
		tmpD.SetMoment4(a.GetMoment4()*b.HasMoment4());
	}
	if(a.HasMoment5() && b.HasMoment5())
	{
		tmpD.AddMoment5();
		tmpD.UpdateData();
		tmpD.SetMoment5(a.GetMoment5()*b.GetMoment5());
	}
	if(a.HasMoment6() && b.HasMoment6() )
	{
		tmpD.AddMoment6();
		tmpD.UpdateData();
		tmpD.SetMoment6(a.GetMoment6()*b.GetMoment6());
	}
	if(a.HasIrregularity() && b.HasIrregularity() )
	{
		tmpD.AddIrregularity();
		tmpD.UpdateData();
		tmpD.SetIrregularity(a.GetIrregularity()*b.GetIrregularity());
	}
	if(a.HasTilt() && b.HasTilt() )
	{
		tmpD.AddTilt();
		tmpD.UpdateData();
		tmpD.SetTilt(a.GetTilt()*b.GetTilt());
	}
	if(a.HasFlatness() && b.HasFlatness() )
	{
		tmpD.AddFlatness();
		tmpD.UpdateData();
		tmpD.SetFlatness(a.GetFlatness()*b.GetFlatness());
	}
	if(a.HasKurtosis() && b.HasKurtosis() )
	{
		tmpD.AddKurtosis();
		tmpD.UpdateData();
		tmpD.SetKurtosis(a.GetKurtosis()*b.GetKurtosis());
	}
	if(a.HasStrongPeak() && b.HasStrongPeak() )
	{
		tmpD.AddStrongPeak();
		tmpD.UpdateData();
		tmpD.SetStrongPeak(a.GetStrongPeak()*b.GetStrongPeak());
	}
	if(a.HasHFC() && b.HasHFC() )
	{
		tmpD.AddHFC();
		tmpD.UpdateData();
		tmpD.SetHFC(a.GetHFC()*b.GetHFC());
	}
	if(a.HasMaxMagFreq() && b.HasMaxMagFreq() )
	{
		tmpD.AddMaxMagFreq();
		tmpD.UpdateData();
		tmpD.SetMaxMagFreq(a.GetMaxMagFreq()*b.GetMaxMagFreq());
	}
	if(a.HasLowFreqEnergyRelation() && b.HasLowFreqEnergyRelation() )
	{
		tmpD.AddLowFreqEnergyRelation();
		tmpD.UpdateData();
		tmpD.SetLowFreqEnergyRelation(a.GetLowFreqEnergyRelation()*b.GetLowFreqEnergyRelation());
	}
	if(a.HasSpread() && b.HasSpread() )
	{
		tmpD.AddSpread();
		tmpD.UpdateData();
		tmpD.SetSpread(a.GetSpread()*b.GetSpread());
	}
	if(a.HasSkewness() && b.HasSkewness() )
	{
		tmpD.AddSkewness();
		tmpD.UpdateData();
		tmpD.SetSkewness(a.GetSkewness()*b.GetSkewness());
	}
	if(a.HasRolloff() && b.HasRolloff() )
	{
		tmpD.AddRolloff();
		tmpD.UpdateData();
		tmpD.SetRolloff(a.GetRolloff()*b.GetRolloff());
	}
	if(a.HasSlope() && b.HasSlope() )
	{
		tmpD.AddSlope();
		tmpD.UpdateData();
		tmpD.SetSlope(a.GetSlope()*b.GetSlope());
	}
	if(a.HasHighFrequencyCoefficient() && b.HasHighFrequencyCoefficient() )
	{
		tmpD.AddHighFrequencyCoefficient();
		tmpD.UpdateData();
		tmpD.SetHighFrequencyCoefficient(a.GetHighFrequencyCoefficient()*b.GetHighFrequencyCoefficient());
	}
	if(a.HasBandDescriptors() && b.HasBandDescriptors() )
	{
		tmpD.AddBandDescriptors();
		tmpD.UpdateData();
		//todo!!! We are not multiplying because we would need the operator implemented in the array
		tmpD.SetBandDescriptors(a.GetBandDescriptors() /** b.GetBandDescriptors()*/ );
	}
	if(a.HasMFCC() && b.HasMFCC() )
	{
		tmpD.AddMFCC();
		tmpD.UpdateData();
		//todo!!! We are not multiplying because we would need the operator implemented in the array
		tmpD.SetMFCC(a.GetMFCC() /** b.GetMFCC() */);
	}
	if(a.HasBandEnergy() && b.HasBandEnergy() )
	{
		tmpD.AddBandEnergy();
		tmpD.UpdateData();
		//todo!!! We are not multiplying because we would need the operator implemented in the array
		tmpD.SetBandEnergy(a.GetBandEnergy() /* * b.GetBandEnergy() */);
	}
		
	return tmpD;
}


SpectralDescriptors operator / (const SpectralDescriptors& a,TData div) 
{
	return a*(1/div);
}

SpectralDescriptors operator + (const SpectralDescriptors& a, const SpectralDescriptors& b)
{
	SpectralDescriptors  tmpD;
	if(a.HasMean() && b.HasMean() )
	{
		tmpD.AddMean();
		tmpD.UpdateData();
		tmpD.SetMean(a.GetMean()+b.GetMean());
	}
	if(a.HasGeometricMean() && b.HasGeometricMean() )
	{
		tmpD.AddGeometricMean();
		tmpD.UpdateData();
		tmpD.SetGeometricMean(a.GetGeometricMean()+b.GetGeometricMean());
	}
	if(a.HasEnergy() && b.HasEnergy() )
	{
		tmpD.AddEnergy();
		tmpD.UpdateData();
		tmpD.SetEnergy(a.GetEnergy()+b.GetEnergy());
	}
	if(a.HasCentroid() && b.HasCentroid() )
	{
		tmpD.AddCentroid();
		tmpD.UpdateData();
		tmpD.SetCentroid(a.GetCentroid()+b.GetCentroid());
	}
	if(a.HasMoment2() && b.HasMoment2() )
	{
		tmpD.AddMoment2();
		tmpD.UpdateData();
		tmpD.SetMoment2(a.GetMoment2()+b.GetMoment2());
	}
	if(a.HasMoment3() && b.HasMoment3() )
	{
		tmpD.AddMoment3();
		tmpD.UpdateData();
		tmpD.SetMoment3(a.GetMoment3()+b.GetMoment3());
	}
	if(a.HasMoment4() && b.HasMoment4() )
	{
		tmpD.AddMoment4();
		tmpD.UpdateData();
		tmpD.SetMoment4(a.GetMoment4()+b.HasMoment4());
	}
	if(a.HasMoment5() && b.HasMoment5())
	{
		tmpD.AddMoment5();
		tmpD.UpdateData();
		tmpD.SetMoment5(a.GetMoment5()+b.GetMoment5());
	}
	if(a.HasMoment6() && b.HasMoment6() )
	{
		tmpD.AddMoment6();
		tmpD.UpdateData();
		tmpD.SetMoment6(a.GetMoment6()+b.GetMoment6());
	}
	if(a.HasIrregularity() && b.HasIrregularity() )
	{
		tmpD.AddIrregularity();
		tmpD.UpdateData();
		tmpD.SetIrregularity(a.GetIrregularity()+b.GetIrregularity());
	}
	if(a.HasTilt() && b.HasTilt() )
	{
		tmpD.AddTilt();
		tmpD.UpdateData();
		tmpD.SetTilt(a.GetTilt()+b.GetTilt());
	}
	if(a.HasFlatness() && b.HasFlatness() )
	{
		tmpD.AddFlatness();
		tmpD.UpdateData();
		tmpD.SetFlatness(a.GetFlatness()+b.GetFlatness());
	}
	if(a.HasKurtosis() && b.HasKurtosis() )
	{
		tmpD.AddKurtosis();
		tmpD.UpdateData();
		tmpD.SetKurtosis(a.GetKurtosis()+b.GetKurtosis());
	}
	if(a.HasStrongPeak() && b.HasStrongPeak() )
	{
		tmpD.AddStrongPeak();
		tmpD.UpdateData();
		tmpD.SetStrongPeak(a.GetStrongPeak()+b.GetStrongPeak());
	}
	if(a.HasHFC() && b.HasHFC() )
	{
		tmpD.AddHFC();
		tmpD.UpdateData();
		tmpD.SetHFC(a.GetHFC()+b.GetHFC());
	}
	if(a.HasMaxMagFreq() && b.HasMaxMagFreq() )
	{
		tmpD.AddMaxMagFreq();
		tmpD.UpdateData();
		tmpD.SetMaxMagFreq(a.GetMaxMagFreq()+b.GetMaxMagFreq());
	}
	if(a.HasLowFreqEnergyRelation() && b.HasLowFreqEnergyRelation() )
	{
		tmpD.AddLowFreqEnergyRelation();
		tmpD.UpdateData();
		tmpD.SetLowFreqEnergyRelation(a.GetLowFreqEnergyRelation()+b.GetLowFreqEnergyRelation());
	}
	if(a.HasSpread() && b.HasSpread() )
	{
		tmpD.AddSpread();
		tmpD.UpdateData();
		tmpD.SetSpread(a.GetSpread()+b.GetSpread());
	}
	if(a.HasSkewness() && b.HasSkewness() )
	{
		tmpD.AddSkewness();
		tmpD.UpdateData();
		tmpD.SetSkewness(a.GetSkewness()+b.GetSkewness());
	}
	if(a.HasRolloff() && b.HasRolloff() )
	{
		tmpD.AddRolloff();
		tmpD.UpdateData();
		tmpD.SetRolloff(a.GetRolloff()+b.GetRolloff());
	}
	if(a.HasSlope() && b.HasSlope() )
	{
		tmpD.AddSlope();
		tmpD.UpdateData();
		tmpD.SetSlope(a.GetSlope()+b.GetSlope());
	}
	if(a.HasHighFrequencyCoefficient() && b.HasHighFrequencyCoefficient() )
	{
		tmpD.AddHighFrequencyCoefficient();
		tmpD.UpdateData();
		tmpD.SetHighFrequencyCoefficient(a.GetHighFrequencyCoefficient()+b.GetHighFrequencyCoefficient());
	}
	if(a.HasBandDescriptors() && b.HasBandDescriptors() )
	{
		tmpD.AddBandDescriptors();
		tmpD.UpdateData();
		//todo!!! We are not multiplying because we would need the operator implemented in the array
		tmpD.SetBandDescriptors(a.GetBandDescriptors() /* + b.GetBandDescriptors()*/ );
	}
	if(a.HasMFCC() && b.HasMFCC() )
	{
		tmpD.AddMFCC();
		tmpD.UpdateData();
		//todo!!! We are not multiplying because we would need the operator implemented in the array
		tmpD.SetMFCC(a.GetMFCC() /* + b.GetMFCC() */);
	}
	if(a.HasBandEnergy() && b.HasBandEnergy() )
	{
		tmpD.AddBandEnergy();
		tmpD.UpdateData();
		//todo!!! We are not multiplying because we would need the operator implemented in the array
		tmpD.SetBandEnergy(a.GetBandEnergy() /* + b.GetBandEnergy() */);
	}
		
	return tmpD;

}


SpectralDescriptors operator * (TData mult,const SpectralDescriptors& a)
{
	return a*mult;
}


};


