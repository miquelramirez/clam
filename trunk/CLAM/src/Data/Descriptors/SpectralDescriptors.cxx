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

using namespace CLAM;

SpectralDescriptors::SpectralDescriptors(Spectrum* pSpectrum):Descriptor(eNumAttr)
{
	MandatoryInit();
	mpSpectrum=pSpectrum;
}

// Implementation

void SpectralDescriptors::DefaultInit() {
	mpSpectrum=0;
	mpStats=0;
	//Warning: no attributes are added by default, the user is in charge of adding the ones he is interested in
}

void SpectralDescriptors::CopyInit(const SpectralDescriptors & copied) {
	mpSpectrum=copied.mpSpectrum;
	mpStats=0;
}

inline const Spectrum* SpectralDescriptors::GetpSpectrum() const {
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
	if(HasKurtosis())	
		SetKurtosis(mpStats->GetKurtosis());
	if(HasSkewness())
	   SetSkewness(mpStats->GetSkew());
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
	TData  mean = mpStats->GetMean();;

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
