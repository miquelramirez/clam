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
		DYNAMIC_TYPE_USING_INTERFACE (AudioDescriptors, 12, DescriptorAbs);
		DYN_ATTRIBUTE (0, public, TData, Mean);
		DYN_ATTRIBUTE (1, public, TData, Variance);

		/**
		 * The temporal centroid is the time averaged over the
		 * energy envelope. What is the effect of a silent signal?
		 * Measured in ???.
		 */
		DYN_ATTRIBUTE (2, public, TData, TemporalCentroid);
		DYN_ATTRIBUTE (3, public, TData, Attack);
		DYN_ATTRIBUTE (4, public, TData, Decay);
		DYN_ATTRIBUTE (5, public, TData, Sustain);
		DYN_ATTRIBUTE (6, public, TData, Release);

		/**
		 * The log-attack time is the (base 10) logarithm of the
		 * rise time. For a silent signal, log-attack time is
		 * -5 (approx. silence). Measured in log10(seconds).
		 */
		DYN_ATTRIBUTE (7, public, TData, LogAttackTime);

		/**
		 * The total energy estimates the signal power at a given
		 * time. Measured in energy.
		 */
		DYN_ATTRIBUTE (8, public, TData, Energy);

		/**
		 * The zero-crossing rate is a measure of the number of time
		 * the signal value cross the zero axe, averaged over the
		 * whole signal. Measured in crossings/second.
		 */
		DYN_ATTRIBUTE (9, public, TData, ZeroCrossingRate);

		/**
		 * The rise time is the time duration between the signal
		 * reached 2% of it maximum value to the time it reaches 80%
		 * of its maximum value. For a silent signal, rise time is
		 * 0. Measured in seconds.
		 */
		DYN_ATTRIBUTE (10,public, TData, RiseTime);

		/**
		 * The temporal decrease is a measure of the amount of
		 * decrease in the signal energy. Measured in dB per
		 * seconds??
		 */
		DYN_ATTRIBUTE (11,public, TData, Decrease);

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
		TData ComputeDecrease();

		
	private:
		Audio* mpAudio;
		static const TData mEpsilon;
		
		bool mIsAttackTimeComputed;
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
	if(a.HasDecrease() && b.HasDecrease() )
	{
		if(b.GetDecrease()<a.GetDecrease())
			tmpD.SetDecrease(b.GetDecrease() );
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
	if(a.HasDecrease() && b.HasDecrease() )
	{
		if(b.GetDecrease()>a.GetDecrease())
			tmpD.SetDecrease(b.GetDecrease() );
	}
	return tmpD;

}


};


#endif /* __AUDIODescriptors_H__ */
