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

#include "SpectralDescriptors.hxx"
#include "Spectrum.hxx"
#include "Array.hxx"
#include <algorithm>

namespace CLAM{
	
DataArray	Add(DataArray &a, DataArray &b) {
	TIndex i;
	TSize sizea=a.Size(); 
	TSize sizeb=b.Size();
	TSize size;
	DataArray result;
	
	// One has size==0 (optimized with respect to the following case?)
	if (sizea==0) {
		result=b;
		return result;
	}
	if (sizeb==0) {
		result=a;
		return result;
	}
	
	// Different sizes
	if(sizea != sizeb) {
		if (sizea < sizeb) {
			size=sizeb;
			result.Resize(size);
			result.SetSize(size);
			for (i=0; i<sizea; i++)
				result[i]=a[i]+b[i];
			for (i=sizea; i<sizeb; i++)
				result[i]=b[i];
			return result;
		}
		else {  // sizea>sizeb
			size=sizea;
			result.Resize(size);
			result.SetSize(size);
			for (i=0; i<sizeb; i++)
				result[i]=a[i]+b[i];
			for (i=sizeb; i<sizea; i++)
				result[i]=a[i];
			return result;
		}
	}
	// Equal size
	size=sizea;
	result.Resize(size);
	result.SetSize(size);
	for (i=0; i<size; i++)
		result[i]=a[i]+b[i];
	return result;
}

DataArray Multiply(TData &factor, DataArray &a) {
	TIndex i;
	TSize size=a.Size(); 
	DataArray result;
	result.Resize(size);
	result.SetSize(size);
	for (i=0; i<size; i++)
	 	result[i]=factor*a[i];
	return result;
}

/* Scalar product of two vectors */
DataArray Multiply(DataArray &a, DataArray &b) {
	TIndex i;
	TSize size=a.Size(); 
	DataArray result;
	result.Resize(size);
	result.SetSize(size);
	for (i=0; i<size; i++)
	 	result[i]=a[i]*b[i];
	return result;
}

SpectralDescriptors::SpectralDescriptors(Spectrum* pSpectrum):Descriptor(eNumAttr)
{
	CLAM_ASSERT(pSpectrum->GetScale()==EScale::eLinear,
		"Spectral Descriptors require a linear magnitude Spectrum");
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
	SetMagnitudeSkewness(initVal);
	SetMagnitudeKurtosis(initVal);
	SetFlatness(initVal);
	SetHighFrequencyContent(initVal);
	SetMaxMagFreq(initVal);
	SetLowFreqEnergyRelation(initVal);
	SetRolloff(initVal);
	SetSlope(initVal);
}

void SpectralDescriptors::DefaultInit() {
	mpSpectrum=0;
	mpStats=0;
	//Warning: no attributes are added by default, the user is in charge of adding the ones he is interested in
}

void SpectralDescriptors::CopyInit(const SpectralDescriptors & copied) 
{
	mpSpectrum=copied.mpSpectrum;
	mpStats=0;
}

const Spectrum* SpectralDescriptors::GetpSpectrum() const 
{
	return mpSpectrum;
}

void SpectralDescriptors::SetpSpectrum(Spectrum* pSpectrum) 
{
	
	CLAM_ASSERT(pSpectrum->GetScale()==EScale::eLinear,
		"Spectral Descriptors require a linear magnitude Spectrum");
	mpSpectrum=pSpectrum;
	//TODO: we are asuming Spectrum is in MagBuffer
	//TODO: it may give problems because pointer passed
	InitStats(&mpSpectrum->GetMagBuffer());

	mDeltaFreq=double(mpSpectrum->GetSpectralRange())/(mpSpectrum->GetSize()-1);
	
}

void SpectralDescriptors::ConcreteCompute()
{
	if (HasMean())
		SetMean(mpStats->GetMean());
	if (HasGeometricMean())
	{
		CLAM_ASSERT( mpSpectrum->GetScale() == CLAM::EScale::eLinear,
			     "The Geometric Mean, as implemented in CLAM, can only"
			     " be computed over Linear Spectral Power distirbutions");
		SetGeometricMean(mpStats->GetGeometricMean());
	}
	if (HasEnergy())
		SetEnergy(mpStats->GetEnergy());
	if (HasCentroid())
		SetCentroid(mpStats->GetCentroid()*mDeltaFreq);
	if(HasMoment2())
		SetMoment2(mpStats->GetMoment(SecondOrder));
	if(HasMoment3())
		SetMoment3(mpStats->GetMoment(ThirdOrder));	
	if(HasMoment4())
		SetMoment4(mpStats->GetMoment(FourthOrder));
	if(HasMoment5())
		SetMoment5(mpStats->GetMoment(FifthOrder));
	if(HasMoment6())
		SetMoment6(mpStats->GetMoment((O<6>*)(0)));
	if (HasSpread())
	        SetSpread(mpStats->GetSpread()*mDeltaFreq*mDeltaFreq);
	if(HasMagnitudeSkewness())
		SetMagnitudeSkewness(mpStats->GetSkew());
	if(HasMagnitudeKurtosis())	
 		SetMagnitudeKurtosis(mpStats->GetKurtosis());
	if(HasFlatness())
		SetFlatness(ComputeSpectralFlatness());
	if(HasHighFrequencyContent())
		SetHighFrequencyContent(ComputeHighFrequencyContent());
	if(HasMaxMagFreq())
		SetMaxMagFreq(ComputeMaxMagFreq());
	if(HasLowFreqEnergyRelation())
		SetLowFreqEnergyRelation(ComputeLowFreqEnergyRelation());
	if(HasRolloff())
		SetRolloff(ComputeRolloff());
	if(HasSlope())
		SetSlope(mpStats->GetSlope()/mDeltaFreq);
}


/*this has been mostly copied and pasted from cuidado and should be checked and some of
it promoted into basicOps*/
TData SpectralDescriptors::ComputeSpectralFlatness()
{
	return mpStats->GetFlatness();
}

/*this has been mostly copied and pasted from cuidado and should be checked and some of
it promoted into basicOps*/
TData SpectralDescriptors::ComputeHighFrequencyContent()
{
	const DataArray & mag = mpSpectrum->GetMagBuffer();
	const TSize size=mpSpectrum->GetSize();
	double temp = 0.0;
	for (int i=1;i<size;i++)
		temp += mag[i]*mag[i]*i;
	return temp;
	/*
	return WeightedPoweredSum<2>()(mpSpectrum->GetMagBuffer());
	*/ 
}

/**
 * It computes the frequency where the spectrum has its maximum value
 *
 * It there are more than one frequency with the same magnitude, 
 * it takes the lower one.
 *
 * @todo Promote MaxPosition to Stats
 */
TData SpectralDescriptors::ComputeMaxMagFreq()
{ 
	// Zero is not enough for spectrums in dB's
	TData max = -1000.0;
	TIndex index = -1;
	
	const DataArray& data=mpSpectrum->GetMagBuffer();
	const unsigned size=mpSpectrum->GetSize();
	for(unsigned i=0; i<size; i++) 
		if(data[i] > max )
		{
			max = data[i];
			index = i;
		} 

	// Convert from index to frequency value in Hz
	return (TData) index * mDeltaFreq;
}

/*this has been mostly copied and pasted from cuidado and should be checked and some of
it promoted into basicOps*/
TData SpectralDescriptors::ComputeLowFreqEnergyRelation() 
{ 
	// Energy(0-100 Hz) / Total Energy
	TIndex index = Round(100.0/mDeltaFreq);
	const DataArray & magnitudeBuffer = mpSpectrum->GetMagBuffer();
	const DataArray data(const_cast<TData*>(magnitudeBuffer.GetPtr()), index);

	Energy energyComputer;
	TData totalEnergy = mpStats->GetEnergy();
	if (totalEnergy < 10e-4) totalEnergy=10e-4;

	TData result=(energyComputer(data)/totalEnergy );
	return result;
}

TData SpectralDescriptors::ComputeRolloff() 
{ 
	DataArray& mags     = mpSpectrum->GetMagBuffer();
	TSize      magsSize = mpSpectrum->GetSize();

	TData eThreshold = 0.85 * mpStats->GetEnergy();
	TData cumEnergy  = 0;
	for (TIndex i=0; i<magsSize; i++)
	{
		cumEnergy += mags[i]*mags[i];
		if (cumEnergy <= eThreshold) continue;
		return i * mDeltaFreq;
	}
	return 0.0;
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
	if(a.HasFlatness())
		tmpD.SetFlatness(a.GetFlatness()*mult);
	if(a.HasMagnitudeKurtosis())
		tmpD.SetMagnitudeKurtosis(a.GetMagnitudeKurtosis()*mult);
	if(a.HasMaxMagFreq())
		tmpD.SetMaxMagFreq(a.GetMaxMagFreq()*mult);
	if(a.HasLowFreqEnergyRelation())
		tmpD.SetLowFreqEnergyRelation(a.GetLowFreqEnergyRelation()*mult);
	if(a.HasSpread())
		tmpD.SetSpread(a.GetSpread()*mult);
	if(a.HasMagnitudeSkewness())
		tmpD.SetMagnitudeSkewness(a.GetMagnitudeSkewness()*mult);
	if(a.HasRolloff())
		tmpD.SetRolloff(a.GetRolloff()*mult);
	if(a.HasSlope())
		tmpD.SetSlope(a.GetSlope()*mult);
	if(a.HasHighFrequencyContent())
		tmpD.SetHighFrequencyContent(a.GetHighFrequencyContent()*mult);
	if(a.HasBandDescriptors())
		//todo!!! We are not multiplying because we would need the operator implemented in the array
		tmpD.SetBandDescriptors(a.GetBandDescriptors());
	if(a.HasMFCC())
		//todo!!! We are not multiplying because we would need the operator implemented in the array
		tmpD.SetMFCC(a.GetMFCC());
	if(a.HasPCP())
		tmpD.SetPCP(Multiply(mult,a.GetPCP()));

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
	if(a.HasFlatness() && b.HasFlatness() )
	{
		tmpD.AddFlatness();
		tmpD.UpdateData();
		tmpD.SetFlatness(a.GetFlatness()*b.GetFlatness());
	}
	if(a.HasMagnitudeKurtosis() && b.HasMagnitudeKurtosis() )
	{
		tmpD.AddMagnitudeKurtosis();
		tmpD.UpdateData();
		tmpD.SetMagnitudeKurtosis(a.GetMagnitudeKurtosis()*b.GetMagnitudeKurtosis());
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
	if(a.HasMagnitudeSkewness() && b.HasMagnitudeSkewness() )
	{
		tmpD.AddMagnitudeSkewness();
		tmpD.UpdateData();
		tmpD.SetMagnitudeSkewness(a.GetMagnitudeSkewness()*b.GetMagnitudeSkewness());
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
	if(a.HasHighFrequencyContent() && b.HasHighFrequencyContent() )
	{
		tmpD.AddHighFrequencyContent();
		tmpD.UpdateData();
		tmpD.SetHighFrequencyContent(a.GetHighFrequencyContent()*b.GetHighFrequencyContent());
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
	if(a.HasPCP() && b.HasPCP() )
	{
		tmpD.AddPCP();
		tmpD.UpdateData();
		tmpD.SetPCP(Multiply(a.GetPCP(),b.GetPCP()));
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
	if(a.HasFlatness() && b.HasFlatness() )
	{
		tmpD.AddFlatness();
		tmpD.UpdateData();
		tmpD.SetFlatness(a.GetFlatness()+b.GetFlatness());
	}
	if(a.HasMagnitudeKurtosis() && b.HasMagnitudeKurtosis() )
	{
		tmpD.AddMagnitudeKurtosis();
		tmpD.UpdateData();
		tmpD.SetMagnitudeKurtosis(a.GetMagnitudeKurtosis()+b.GetMagnitudeKurtosis());
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
	if(a.HasMagnitudeSkewness() && b.HasMagnitudeSkewness() )
	{
		tmpD.AddMagnitudeSkewness();
		tmpD.UpdateData();
		tmpD.SetMagnitudeSkewness(a.GetMagnitudeSkewness()+b.GetMagnitudeSkewness());
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
	if(a.HasHighFrequencyContent() && b.HasHighFrequencyContent() )
	{
		tmpD.AddHighFrequencyContent();
		tmpD.UpdateData();
		tmpD.SetHighFrequencyContent(a.GetHighFrequencyContent()+b.GetHighFrequencyContent());
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
	if(a.HasPCP() && b.HasPCP() )
	{
		tmpD.AddPCP();
		tmpD.UpdateData();
		tmpD.SetPCP(Add(a.GetPCP(),b.GetPCP()));
	}
		
	return tmpD;

}


SpectralDescriptors operator * (TData mult,const SpectralDescriptors& a)
{
	return a*mult;
}


};

