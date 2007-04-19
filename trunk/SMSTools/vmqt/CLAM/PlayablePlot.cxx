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

#include <CLAM/QtPlayer.hxx>
#include <CLAM/PlayableList.hxx>
#include <CLAM/PlayablePlot.hxx>

namespace CLAM
{
    namespace VM
    {
		PlayablePlot::PlayablePlot() 
			: mPlayer(0)
		{
		}

		PlayablePlot::~PlayablePlot()
		{
			if(mPlayer) 
			{
				Stop();
				delete mPlayer;
			}
		}
		
		void PlayablePlot::AddToPlayList()
		{
			PlayableList::Add(this);
		}

		void PlayablePlot::Play()
		{
			if(mPlayer) mPlayer->play();
		}

		void PlayablePlot::Pause()
		{
			if(mPlayer) mPlayer->pause();
		}

		void PlayablePlot::Stop()
		{
			if(mPlayer) mPlayer->stop();
		}
				
		bool PlayablePlot::IsPlaying()
		{
			if(mPlayer) return mPlayer->IsPlaying();
			return false;
		}

		void PlayablePlot::RemoveFromPlayList()
		{
			PlayableList::Remove(this);
		}
    }
}

// END

