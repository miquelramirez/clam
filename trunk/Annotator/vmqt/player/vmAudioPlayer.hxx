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

#ifndef __VMQT_AUDIO_PLAYER_H__
#define __VMQT_AUDIO_PLAYER_H__

#include <vector>
#include "vmPlayer.hxx"

namespace CLAM
{
	class Audio;

	namespace VM
	{
		class AudioPlayer : public Player
		{
			Q_OBJECT
		public:
			AudioPlayer(QObject* parent=0);
			~AudioPlayer();

			void SetData(std::vector<const Audio*> data);

		public slots:
			void muteChannel0(bool muted);
			void muteChannel1(bool muted);
			
		private:
			bool mChn0Muted;
			bool mChn1Muted;

			const Audio* mChn0;
			const Audio* mChn1;

			void run();
		};
	}
}

#endif

