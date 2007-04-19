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

#include <CLAM/PlayablePlot.hxx>
#include <CLAM/PlayableList.hxx>

namespace CLAM
{
	namespace VM
	{
		PlayableList::PlayablePlotList PlayableList::mList;

		void PlayableList::Add(PlayablePlot* plot)
		{
			mList.push_back(plot);
		}
		
		void PlayableList::Remove(PlayablePlot* plot)
		{
			PlayablePlotList::iterator it = mList.begin();
			for(;it != mList.end();it++)
			{
				if((*it)==plot)
				{
					mList.erase(it);
					break;
				}
			}
		}
		
		void PlayableList::Stop()
		{
			PlayablePlotList::iterator it = mList.begin();
			for(;it != mList.end();it++)
			{
				if((*it)->IsPlaying())
				{
					(*it)->Stop();
					break;
				}
			}
		}
	}
}

// END

