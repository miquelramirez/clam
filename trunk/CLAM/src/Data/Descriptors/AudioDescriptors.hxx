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

#ifndef __AUDIODescriptors_H__
#define __AUDIODescriptors_H__


#include "Descriptor.hxx"
#include "DataTypes.hxx"
/*
 * This class holds Descriptors computed from Audio data
 *
 *
 */

namespace CLAM {

	class Audio;

	class AudioDescriptors : public DescriptorAbs {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (AudioDescriptors, 11, DescriptorAbs);
		DYN_ATTRIBUTE (0, public, TData, Mean);
		DYN_ATTRIBUTE (1, public, TData, Variance);
		DYN_ATTRIBUTE (2, public, TData, TemporalCentroid);
		DYN_ATTRIBUTE (3, public, TData, Attack);
		DYN_ATTRIBUTE (4, public, TData, Decay);
		DYN_ATTRIBUTE (5, public, TData, Sustain);
		DYN_ATTRIBUTE (6, public, TData, Release);
		DYN_ATTRIBUTE (7, public, TData, LogAttackTime);
		DYN_ATTRIBUTE (8, public, TData, Energy);
		DYN_ATTRIBUTE (9, public, TData, ZeroCrossingRate);
		DYN_ATTRIBUTE (10,public, TData, RiseTime);

	public:

		AudioDescriptors(Audio* pAudio);
		AudioDescriptors(TData initVal);

		const Audio* GetpAudio() const;
		void SetpAudio(Audio* pAudio);
		void ConcreteCompute();

	private:
		void DefaultInit();
		void CopyInit(const AudioDescriptors & copied);
		
		TData ComputeZeroCrossingRate();
		TData ComputeAttackTime();
		TData ComputeLogAttackTime();

		
	private:
		Audio* mpAudio;
		
		TData mComputedAttackTime;
	};


AudioDescriptors operator * (const AudioDescriptors& a,TData mult) ;
AudioDescriptors operator * (TData mult, const AudioDescriptors& a) ;
AudioDescriptors operator * (const AudioDescriptors& a,const AudioDescriptors& b) ;
AudioDescriptors operator + (const AudioDescriptors& a,const AudioDescriptors& b) ;
AudioDescriptors operator - (const AudioDescriptors& a,const AudioDescriptors& b) ;
AudioDescriptors operator / (const AudioDescriptors& a,TData div);

template<>
inline AudioDescriptors CLAM_min (const AudioDescriptors & a,const AudioDescriptors & b)
{
	AudioDescriptors tmpD(a);

	if (a.HasMean() && b.HasMean() )
	{
		if(b.GetMean()<a.GetMean())
			tmpD.SetMean(b.GetMean() );
	}
	if (a.HasTemporalCentroid() && b.HasTemporalCentroid() )
	{
		if(b.GetTemporalCentroid()<a.GetTemporalCentroid())
			tmpD.SetTemporalCentroid(b.GetTemporalCentroid() );
	}
	if (a.HasEnergy() && b.HasEnergy() )
	{
		if(b.GetEnergy()<a.GetEnergy())
			tmpD.SetEnergy(b.GetEnergy() );
	}
	if(a.HasVariance() && b.HasVariance() )
	{
		if(b.GetVariance()<a.GetVariance())
			tmpD.SetVariance(b.GetVariance() );
	}
	if(a.HasZeroCrossingRate() && b.HasZeroCrossingRate() )
	{
		if(b.GetZeroCrossingRate()<a.GetZeroCrossingRate())
			tmpD.SetZeroCrossingRate(b.GetZeroCrossingRate() );
	}
	if(a.HasRiseTime() && b.HasRiseTime() )
	{
		if(b.GetRiseTime()<a.GetRiseTime())
			tmpD.SetRiseTime(b.GetRiseTime() );
	}
	if(a.HasLogAttackTime() && b.HasLogAttackTime() )
	{
		if(b.GetLogAttackTime()<a.GetLogAttackTime())
			tmpD.SetLogAttackTime(b.GetLogAttackTime() );
	}
	if(a.HasAttack() && b.HasAttack() )
	{
		if(b.GetAttack()<a.GetAttack())
			tmpD.SetAttack(b.GetAttack() );
	}
	if(a.HasDecay() && b.HasDecay() )
	{
		if(b.GetDecay()<a.GetDecay())
			tmpD.SetDecay(b.GetDecay() );
	}
	if(a.HasSustain() && b.HasSustain() )
	{
		if(b.GetSustain()<a.GetSustain())
			tmpD.SetSustain(b.GetSustain() );
	}
	if(a.HasRelease() && b.HasRelease() )
	{
		if(b.GetRelease()<a.GetRelease())
			tmpD.SetRelease(b.GetRelease() );
	}
	return tmpD;


}

template<>
inline AudioDescriptors CLAM_max (const AudioDescriptors & a,const AudioDescriptors & b)
{
	AudioDescriptors tmpD(a);

	if (a.HasMean() && b.HasMean() )
	{
		if(b.GetMean()>a.GetMean())
			tmpD.SetMean(b.GetMean() );
	}
	if (a.HasTemporalCentroid() && b.HasTemporalCentroid() )
	{
		if(b.GetTemporalCentroid()>a.GetTemporalCentroid())
			tmpD.SetTemporalCentroid(b.GetTemporalCentroid() );
	}
	if (a.HasEnergy() && b.HasEnergy() )
	{
		if(b.GetEnergy()>a.GetEnergy())
			tmpD.SetEnergy(b.GetEnergy() );
	}
	if(a.HasVariance() && b.HasVariance() )
	{
		if(b.GetVariance()>a.GetVariance())
			tmpD.SetVariance(b.GetVariance() );
	}
	if(a.HasZeroCrossingRate() && b.HasZeroCrossingRate() )
	{
		if(b.GetZeroCrossingRate()>a.GetZeroCrossingRate())
			tmpD.SetZeroCrossingRate(b.GetZeroCrossingRate() );
	}
	if(a.HasRiseTime() && b.HasRiseTime() )
	{
		if(b.GetRiseTime()>a.GetRiseTime())
			tmpD.SetRiseTime(b.GetRiseTime() );
	}
	if(a.HasLogAttackTime() && b.HasLogAttackTime() )
	{
		if(b.GetLogAttackTime()>a.GetLogAttackTime())
			tmpD.SetLogAttackTime(b.GetLogAttackTime() );
	}
	if(a.HasAttack() && b.HasAttack() )
	{
		if(b.GetAttack()>a.GetAttack())
			tmpD.SetAttack(b.GetAttack() );
	}
	if(a.HasDecay() && b.HasDecay() )
	{
		if(b.GetDecay()>a.GetDecay())
			tmpD.SetDecay(b.GetDecay() );
	}
	if(a.HasSustain() && b.HasSustain() )
	{
		if(b.GetSustain()>a.GetSustain())
			tmpD.SetSustain(b.GetSustain() );
	}
	if(a.HasRelease() && b.HasRelease() )
	{
		if(b.GetRelease()>a.GetRelease())
			tmpD.SetRelease(b.GetRelease() );
	}
	return tmpD;

}


};


#endif /* __AUDIODescriptors_H__ */
