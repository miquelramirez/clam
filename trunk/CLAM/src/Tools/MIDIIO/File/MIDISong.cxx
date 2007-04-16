/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 * UNIVERSITAT POMPEU FABRA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * MIDIFileReader C++ classes
 * This code is part of the CLAM library, but also usable stand-alone.
 * Maarten de Boer <mdeboer@iua.upf.es>
 *
 */
#include <vector>
#include "MIDITrack.hxx"
#include "MIDISong.hxx"

namespace MIDI
{

		class SongImpl
		{	
		friend class Song;
		private:	
			std::vector<Track*> mTrackList;
			unsigned short mTicksPerQ;
			
			int Tracks(void) const
			{
				return mTrackList.size();
			}

			Track* GetTrack(int i) const
			{
				return mTrackList[i];
			}

			void AddTrack(Track* t)
			{
				mTrackList.push_back(t);
			}

			unsigned short GetTicksPerQ(void) const
			{
				return mTicksPerQ;
			}
			
			void SetTicksPerQ(unsigned int v)
			{
				mTicksPerQ = v;
			}
		};

		Song::Song()
		{
			mImpl = new SongImpl;
		}

		Song::~Song()
		{
			delete mImpl;
		}

		int Song::Tracks(void) const
		{
			return mImpl->Tracks();
		}

		Track* Song::GetTrack(int i) const
		{
			return mImpl->GetTrack(i);
		}

		void Song::AddTrack(Track* t)
		{
			mImpl->AddTrack(t);
		}

		unsigned short Song::GetTicksPerQ(void) const
		{
			return mImpl->GetTicksPerQ();
		}

		void Song::SetTicksPerQ(unsigned int v)
		{
			mImpl->SetTicksPerQ(v);
		}
	
}

