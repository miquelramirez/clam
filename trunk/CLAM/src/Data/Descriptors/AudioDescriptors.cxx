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

using namespace CLAM;

AudioDescriptors::AudioDescriptors(Audio* pAudio): Descriptor(eNumAttr)
{
	MandatoryInit();
	mpAudio=pAudio;
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
	TIndex maxindex,offset;

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
