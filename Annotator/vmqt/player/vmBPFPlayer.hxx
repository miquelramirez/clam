/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __VMQT_BPF_PLAYER_H__
#define __VMQT_BPF_PLAYER_H__

#include "BPF.hxx"
#include "vmPlayer.hxx"

namespace CLAM
{
	class Audio;

	namespace VM
	{
		class BPFPlayer : public Player
		{
			Q_OBJECT
		public:
			BPFPlayer(QObject* parent=0);
			~BPFPlayer();

			void SetData(const BPF& bpf);
			void SetAudioPtr(const Audio* audio, unsigned channelMask=1|2); // TODO: Make this be enums
			void SetPitchBounds(double min, double max);

		private:
			const BPF*   mBPF;
			const Audio* mAudio0;
			const Audio* mAudio1;
			Range        mPitchBounds;

			void run();

			unsigned FirstIndex();
			double GetPitch(unsigned index);
		};
	}
}

#endif

