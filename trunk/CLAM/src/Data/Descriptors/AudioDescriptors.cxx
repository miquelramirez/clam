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



#include <cmath>
#include "AudioDescriptors.hxx"
#include "Audio.hxx"
#include "OSDefines.hxx"

namespace CLAM {


const TData AudioDescriptors::mEpsilon = 1e-5;

AudioDescriptors::AudioDescriptors(Audio* pAudio): DescriptorAbs(eNumAttr)
{
	MandatoryInit();
	mpAudio=pAudio;
}

AudioDescriptors::AudioDescriptors(TData initVal):DescriptorAbs(eNumAttr)
{
	MandatoryInit();
	AddAll();
	UpdateData();
	SetMean(initVal);
	SetTemporalCentroid(initVal);
	SetEnergy(initVal);
	SetVariance(initVal);
	SetZeroCrossingRate(initVal);
	SetRiseTime(initVal);
	SetLogAttackTime(initVal);
	SetDecrease(initVal);
}

void AudioDescriptors::DefaultInit() {
	mpAudio=0;
}


void AudioDescriptors::CopyInit(const AudioDescriptors & copied) {
	mpAudio=copied.mpAudio;
}

const Audio* AudioDescriptors::GetpAudio() const {
	return mpAudio;
}



void AudioDescriptors::SetpAudio(Audio* pAudio) {
	mpAudio=pAudio;
	//TODO: it may give problems because pointer passed
	InitStats(&mpAudio->GetBuffer());
	mIsAttackTimeComputed=false;
}

void AudioDescriptors::ConcreteCompute()
{
	if (HasMean())
		SetMean(mpStats->GetMean());
	if (HasTemporalCentroid())
		SetTemporalCentroid(mpStats->GetCentroid()/mpAudio->GetSampleRate());
	if (HasEnergy())
		SetEnergy(mpStats->GetEnergy());
	if(HasVariance())
		SetVariance(mpStats->GetVariance());
	if(HasZeroCrossingRate())
		SetZeroCrossingRate(ComputeZeroCrossingRate());
	if(HasRiseTime())
		SetRiseTime(ComputeAttackTime());
	if(HasLogAttackTime())
		SetLogAttackTime(ComputeLogAttackTime());
	if(HasDecrease())
		SetDecrease(ComputeDecrease());
}

TData AudioDescriptors::ComputeZeroCrossingRate()
{
	DataArray& data = mpAudio->GetBuffer();

	int signChangeCount = 0;
	const TSize size = data.Size();
	bool wasPositive = data[0] > 0.0;

	for (int i=1; i<size; i++)
	{
		const bool isPositive = (data[i] > 0.0);
		if (wasPositive ==  isPositive) continue;

		signChangeCount++;
		wasPositive = isPositive;
	}
	// Average
	return ((TData)signChangeCount)/size;
}

TData AudioDescriptors::ComputeAttackTime()
{
	if(mIsAttackTimeComputed) return mComputedAttackTime;

	const DataArray& data     = mpAudio->GetBuffer();
	const TSize      dataSize = mpAudio->GetSize();

	DataArray energyEnv;
	energyEnv.Resize(dataSize);
	energyEnv.SetSize(dataSize);

	// Compute 20Hz lowpass filter coefficients
	const TData omega_c = 2*PI*20/mpAudio->GetSampleRate();
	const TData alpha   = (1-sin(omega_c)) / cos(omega_c);

	const TData b0 = (1-alpha)/2;
	const TData a1 = -alpha;

	// Find maximum value
	energyEnv[0] = b0*CLAM::Abs(data[0]);
	TData maxVal = energyEnv[0];

	for (TIndex i=1; i<dataSize; i++) {
		energyEnv[i] = b0*(CLAM::Abs(data[i]) + CLAM::Abs(data[i-1])) - a1*energyEnv[i-1];
		if (energyEnv[i] > maxVal) maxVal = energyEnv[i];
	}

	// Locate start and stop of attack
	const TData startThreshold = 0.02*maxVal;
	const TData stopThreshold  = 0.80*maxVal;

	TIndex startIdx;
	for (startIdx=0; startIdx<dataSize; startIdx++) {
		if (energyEnv[startIdx] > startThreshold) break;
	}

	TIndex stopIdx;
	for (stopIdx=startIdx; stopIdx<dataSize; stopIdx++) {
		if (energyEnv[stopIdx] > stopThreshold) break;
	}

	mComputedAttackTime=(stopIdx - startIdx) / mpAudio->GetSampleRate();
	mIsAttackTimeComputed=true;
	return mComputedAttackTime;
}


TData AudioDescriptors::ComputeLogAttackTime()
{
	ComputeAttackTime();
	if (mComputedAttackTime==0)
		return log10(mEpsilon);
	return log10(mComputedAttackTime);
}


TData AudioDescriptors::ComputeDecrease()
{
	const DataArray& data = mpAudio->GetBuffer();
	const TSize dataSize  = mpAudio->GetSize();

	// Compute 20Hz lowpass filter coefficients
	const double omega_c = 2*PI*20/mpAudio->GetSampleRate();
	const double alpha   = (1-sin(omega_c)) / cos(omega_c);

	const double b0 = (1-alpha)/2;
	const double a1 = -alpha;

	// Find maximum value
	double y = b0*CLAM::Abs(data[0]);
	TData correctedY = y<mEpsilon ? mEpsilon : y;
	double logEnv = log10(correctedY);

	TData maxVal = logEnv;
	TSize maxIdx = 0;
	double sumXX = 0;
	double sumY = 0;
	double sumXY = 0;

	for (TIndex i=1; i<dataSize; i++)
	{
		y = b0*(CLAM::Abs(data[i-1]) + CLAM::Abs(data[i])) - a1*y;
		correctedY = y<mEpsilon ? mEpsilon : y;
		const double logEnv = log10(correctedY);

		if (logEnv > maxVal)
		{
			maxVal = logEnv;
			maxIdx = i;
			sumXX = 0;
			sumY = 0;
			sumXY = 0;
		}
		sumY += logEnv;
		sumXY += i*logEnv;
		sumXX += i*i;
	}

	// Compute means and gradient of decay part
	const long N = dataSize - maxIdx;
	TData sumX = N*(N + 2*maxIdx - 1)/2;

	TData num = N * sumXY - sumX * sumY;
	TData den = N * sumXX - sumX * sumX;

	return (num / den) * mpAudio->GetSampleRate();
}


AudioDescriptors operator * (const AudioDescriptors& a,TData mult)
{
	
	AudioDescriptors tmpD(a);

	if (a.HasMean())
	{
		tmpD.SetMean(a.GetMean()*mult);
	}
	if (a.HasTemporalCentroid())
	{
		tmpD.SetTemporalCentroid(a.GetTemporalCentroid()*mult);
	}
	if (a.HasEnergy())
	{
		tmpD.SetEnergy(a.GetEnergy()*mult);
	}
	if(a.HasVariance())
	{
		tmpD.SetVariance(a.GetVariance()*mult);
	}
	if(a.HasZeroCrossingRate())
	{
		tmpD.SetZeroCrossingRate(a.GetZeroCrossingRate()*mult);
	}
	if(a.HasRiseTime())
	{
		tmpD.SetRiseTime(a.GetRiseTime()*mult);
	}
	if(a.HasLogAttackTime())
	{
		tmpD.SetLogAttackTime(a.GetLogAttackTime()*mult);
	}
	if(a.HasDecrease())
	{
		tmpD.SetDecrease(a.GetDecrease()*mult);
	}
	return tmpD;
}

AudioDescriptors operator * (TData mult, const AudioDescriptors& a)
{
	return a*mult;
}

AudioDescriptors operator * (const AudioDescriptors& a,const AudioDescriptors& b)
{
	AudioDescriptors tmpD;

	if (a.HasMean() && b.HasMean() )
	{
		tmpD.AddMean();
		tmpD.UpdateData();
		tmpD.SetMean(a.GetMean()*b.GetMean() );
	}
	if (a.HasTemporalCentroid() && b.HasTemporalCentroid() )
	{
		tmpD.AddTemporalCentroid();
		tmpD.UpdateData();
		tmpD.SetTemporalCentroid(a.GetTemporalCentroid()*b.GetTemporalCentroid() );
	}
	if (a.HasEnergy() && b.HasEnergy() )
	{
		tmpD.AddEnergy();
		tmpD.UpdateData();
		tmpD.SetEnergy(a.GetEnergy()*b.GetEnergy() );
	}
	if(a.HasVariance() && b.HasVariance() )
	{
		tmpD.AddVariance();
		tmpD.UpdateData();
		tmpD.SetVariance(a.GetVariance()*b.GetVariance() );
	}
	if(a.HasZeroCrossingRate() && b.HasZeroCrossingRate() )
	{
		tmpD.AddZeroCrossingRate();
		tmpD.UpdateData();
		tmpD.SetZeroCrossingRate(a.GetZeroCrossingRate()*b.GetZeroCrossingRate() );
	}
	if(a.HasRiseTime() && b.HasRiseTime() )
	{
		tmpD.AddRiseTime();
		tmpD.UpdateData();
		tmpD.SetRiseTime(a.GetRiseTime()*b.GetRiseTime() );
	}
	if(a.HasLogAttackTime() && b.HasLogAttackTime() )
	{
		tmpD.AddLogAttackTime();
		tmpD.UpdateData();
		tmpD.SetLogAttackTime(a.GetLogAttackTime()*b.GetLogAttackTime() );
	}
	if(a.HasDecrease() && b.HasDecrease() )
	{
		tmpD.AddDecrease();
		tmpD.UpdateData();
		tmpD.SetDecrease(a.GetDecrease()*b.GetDecrease() );
	}
	return tmpD;
}

AudioDescriptors operator + (const AudioDescriptors& a,const AudioDescriptors& b)
{
	AudioDescriptors tmpD;

	if (a.HasMean() && b.HasMean() )
	{
		tmpD.AddMean();
		tmpD.UpdateData();
		tmpD.SetMean(a.GetMean()+b.GetMean() );
	}
	if (a.HasTemporalCentroid() && b.HasTemporalCentroid() )
	{
		tmpD.AddTemporalCentroid();
		tmpD.UpdateData();
		tmpD.SetTemporalCentroid(a.GetTemporalCentroid()+b.GetTemporalCentroid() );
	}
	if (a.HasEnergy() && b.HasEnergy() )
	{
		tmpD.AddEnergy();
		tmpD.UpdateData();
		tmpD.SetEnergy(a.GetEnergy()+b.GetEnergy() );
	}
	if(a.HasVariance() && b.HasVariance() )
	{
		tmpD.AddVariance();
		tmpD.UpdateData();
		tmpD.SetVariance(a.GetVariance()+b.GetVariance() );
	}
	if(a.HasZeroCrossingRate() && b.HasZeroCrossingRate() )
	{
		tmpD.AddZeroCrossingRate();
		tmpD.UpdateData();
		tmpD.SetZeroCrossingRate(a.GetZeroCrossingRate()+b.GetZeroCrossingRate() );
	}
	if(a.HasRiseTime() && b.HasRiseTime() )
	{
		tmpD.AddRiseTime();
		tmpD.UpdateData();
		tmpD.SetRiseTime(a.GetRiseTime()+b.GetRiseTime() );
	}
	if(a.HasLogAttackTime() && b.HasLogAttackTime() )
	{
		tmpD.AddLogAttackTime();
		tmpD.UpdateData();
		tmpD.SetLogAttackTime(a.GetLogAttackTime()+b.GetLogAttackTime() );
	}
	if(a.HasDecrease() && b.HasDecrease() )
	{
		tmpD.AddDecrease();
		tmpD.UpdateData();
		tmpD.SetDecrease(a.GetDecrease()+b.GetDecrease() );
	}
	return tmpD;

}

AudioDescriptors operator - (const AudioDescriptors& a,const AudioDescriptors& b)
{
	return a+((-1)*b);
}

AudioDescriptors operator / (const AudioDescriptors& a,TData div) 
{
	return a*(1/div);
}

}

