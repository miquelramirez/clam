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

#include "QtPlayer.hxx"
#include "PlayableList.hxx"
#include "PlayablePlot.hxx"

namespace CLAM
{
    namespace VM
    {
	PlayablePlot::PlayablePlot() 
	{
	    _player = NULL;
	    PlayableList::Add(this);
	}

	PlayablePlot::~PlayablePlot()
	{
	    if(_player) 
	    {
		Stop();
		delete _player;
	    }
	}
				
	void PlayablePlot::Play()
	{
	    if(_player) _player->play();
	}

	void PlayablePlot::Pause()
	{
	    if(_player) _player->pause();
	}

	void PlayablePlot::Stop()
	{
	    if(_player) _player->stop();
	}
				
	bool PlayablePlot::IsPlaying()
	{
	    if(_player) return _player->IsPlaying();
	    return false;
	}

	void PlayablePlot::RemoveFromPlayList()
	{
	    PlayableList::Remove(this);
	}
    }
}

// END

