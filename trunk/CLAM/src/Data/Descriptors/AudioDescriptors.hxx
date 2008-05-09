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

#ifndef __AUDIODescriptors_H__
#define __AUDIODescriptors_H__

#include <typeinfo>
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
		DYNAMIC_TYPE_USING_INTERFACE (AudioDescriptors, 8, DescriptorAbs);
		
		/** The mean value of the absolute value of the audio samples amplitude. 
		 *  The result is in signal units.
		 *  @see Stats::GetMean
		 */
		DYN_ATTRIBUTE (0, public, TData, Mean);
		/** The variance of audio samples amplitude. 
		 * The result is in signal difference squared units.
		 *  @see Stats::GetVariance
		 */
		DYN_ATTRIBUTE (1, public, TData, Variance);
		/**
		 * The temporal centroid is time where signal energy
		 * is "concentrated". For a "silence" signal the centroid will be 
		 * placed in the middle of the signal.
		 * It is computed by computing the statistical centroid over
		 * the absolute value of the signal.
		 * Measured in seconds.
		 * @see Stats::GetCentroid
		 */
		DYN_ATTRIBUTE (2, public, TData, TemporalCentroid);
		
		/**
		 * The log-attack time is the (base 10) logarithm of the
		 * rise time. For a silent signal, log-attack time is
		 * -5. Measured in log10(seconds).
		 */
		DYN_ATTRIBUTE (3, public, TData, LogAttackTime);

		/**
		 * The squared sum of audio samples amplitudes. 
		 * This measure is not limited to the range [0,1].
		 * Measured in squared signal units.
		 * @see Stats::GetEnergy
		 */
		DYN_ATTRIBUTE (4, public, TData, Energy);

		/**
		 * The zero-crossing rate is a measure of the number of time
		 * the signal value cross the zero axe, averaged over the
		 * whole signal. Measured in crossings/second.
		 */
		DYN_ATTRIBUTE (5, public, TData, ZeroCrossingRate);

		/**
		 * The rise time is the time duration between the signal
		 * reached 2% of it maximum value to the time it reaches 80%
		 * of its maximum value. For a silent signal, rise time is
		 * 0. Measured in seconds.
		 */
		DYN_ATTRIBUTE (6, public, TData, RiseTime);

		/**
		 * The temporal decrease is a measure of the amount of
		 * decrease in the signal energy. Measured in dB per
		 * seconds??
		 */
		DYN_ATTRIBUTE (7, public, TData, Decrease);

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
		const Audio* mpAudio;
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
	if(a.HasDecrease() && b.HasDecrease() )
	{
		if(b.GetDecrease()>a.GetDecrease())
			tmpD.SetDecrease(b.GetDecrease() );
	}
	return tmpD;

}


};


#endif /* __AUDIODescriptors_H__ */

