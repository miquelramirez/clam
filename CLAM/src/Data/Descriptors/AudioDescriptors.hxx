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

	class AudioDescriptors : public Descriptor {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (AudioDescriptors, 11, Descriptor);
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

		const Audio* GetpAudio() const;
		void SetpAudio(Audio* pAudio);
		void ConcreteCompute();
	private:
//		void DefaultInit();
//		void CopyInit(const AudioDescriptors & copied);
		
		TData ComputeZeroCrossingRate();
		TData ComputeAttackTime();
		TData ComputeLogAttackTime();

		
	private:
		Audio* mpAudio;
		
		TData mComputedAttackTime;
	};



// Implementation

/*inline void AudioDescriptors::DefaultInit() {
	mpAudio=0;
}

inline void AudioDescriptors::CopyInit(const AudioDescriptors & copied) {
	mpAudio=copied.mpAudio;
}

inline const Audio* AudioDescriptors::GetpAudio() const {
	return mpAudio;
}*/


};


#endif /* __AUDIODescriptors_H__ */
