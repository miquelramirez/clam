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



#include <cmath>
#include "AudioDescriptors.hxx"
#include "Audio.hxx"

namespace CLAM {

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
	SetAttack(initVal);
	SetDecay(initVal);
	SetSustain(initVal);
	SetRelease(initVal);
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
	mComputedAttackTime=0;	
}

void AudioDescriptors::ConcreteCompute()
{
	if (HasMean())
		SetMean(mpStats->GetMean());
	if (HasTemporalCentroid())
		SetTemporalCentroid(mpStats->GetCentroid()*mpAudio->GetDuration()/mpAudio->GetSize());
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
/*
		Not implemented yet;

		DYN_ATTRIBUTE (3, public, TData, Attack);
		DYN_ATTRIBUTE (4, public, TData, Decay);
		DYN_ATTRIBUTE (5, public, TData, Sustain);
		DYN_ATTRIBUTE (6, public, TData, Release);
*/
}

TData AudioDescriptors::ComputeZeroCrossingRate()
{
	int sum = 0;
	DataArray& data=mpAudio->GetBuffer();
	int size=data.Size();
	for (int i=1; i<size; i++) {
	  if (((data[i] < 0.0) && (data[i-1] > 0.0)) ||
		  ((data[i] > 0.0) && (data[i-1] < 0.0)))
		sum++;
	}
	return ((TData)sum)/size;
}

TData AudioDescriptors::ComputeAttackTime()
{
	if(mComputedAttackTime) return mComputedAttackTime;

	TData max = 0.;
	TIndex maxindex = -1,offset;

	//this algorithm is not the first time I see it, should be generalized and optimized
	int i;
	int size=mpAudio->GetSize();
	for (i=0;i<size;i++)
	   if (data[i] > max) {
		max = data[i];
		maxindex = i;
	}
	i=0;
	TData offsetMag=0.02*max;
	while(true)
	{
		if(data[i]>offsetMag){ 
			offset=i;
			break;}
		i++;
	}

	mComputedAttackTime=maxindex-offset;
	return mComputedAttackTime;
}

TData AudioDescriptors::ComputeLogAttackTime()
{
	return log(ComputeAttackTime());
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
	if(a.HasAttack())
	{
		tmpD.SetAttack(a.GetAttack()*mult);
	}
	if(a.HasDecay())
	{
		tmpD.SetDecay(a.GetDecay()*mult);
	}
	if(a.HasSustain())
	{
		tmpD.SetSustain(a.GetSustain()*mult);
	}
	if(a.HasRelease())
	{
		tmpD.SetRelease(a.GetRelease()*mult);
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
	if(a.HasAttack() && b.HasAttack() )
	{
		tmpD.AddAttack();
		tmpD.UpdateData();
		tmpD.SetAttack(a.GetAttack()*b.GetAttack() );
	}
	if(a.HasDecay() && b.HasDecay() )
	{
		tmpD.AddDecay();
		tmpD.UpdateData();
		tmpD.SetDecay(a.GetDecay()*b.GetDecay() );
	}
	if(a.HasSustain() && b.HasSustain() )
	{
		tmpD.AddSustain();
		tmpD.UpdateData();
		tmpD.SetSustain(a.GetSustain()*b.GetSustain() );
	}
	if(a.HasRelease() && b.HasRelease() )
	{
		tmpD.AddRelease();
		tmpD.UpdateData();
		tmpD.SetRelease(a.GetRelease()*b.GetRelease() );
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
	if(a.HasAttack() && b.HasAttack() )
	{
		tmpD.AddAttack();
		tmpD.UpdateData();
		tmpD.SetAttack(a.GetAttack()+b.GetAttack() );
	}
	if(a.HasDecay() && b.HasDecay() )
	{
		tmpD.AddDecay();
		tmpD.UpdateData();
		tmpD.SetDecay(a.GetDecay()+b.GetDecay() );
	}
	if(a.HasSustain() && b.HasSustain() )
	{
		tmpD.AddSustain();
		tmpD.UpdateData();
		tmpD.SetSustain(a.GetSustain()+b.GetSustain() );
	}
	if(a.HasRelease() && b.HasRelease() )
	{
		tmpD.AddRelease();
		tmpD.UpdateData();
		tmpD.SetRelease(a.GetRelease()+b.GetRelease() );
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
