/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "APlayer.hxx"
#include "QtAudioPlayer.hxx"

namespace CLAM
{
    namespace VM
    {
		QtAudioPlayer::QtAudioPlayer(QWidget* parent) 
			: QtSinglePlayer(parent)
		{
			SetPlayer(new APlayer());
		}
		
		QtAudioPlayer::~QtAudioPlayer()
		{
		}

		void QtAudioPlayer::SetData(std::vector<const Audio*> data, bool setTime)
		{
			((APlayer*)mPlayer)->SetData(data,setTime);
		}

		void QtAudioPlayer::SetLeftChannelMuted(bool b)
		{
			((APlayer*)mPlayer)->SetLeftChannelMuted(b);
		}

		void QtAudioPlayer::SetRightChannelMuted(bool b)
		{
			((APlayer*)mPlayer)->SetRightChannelMuted(b);
		}
    }
}

// END

