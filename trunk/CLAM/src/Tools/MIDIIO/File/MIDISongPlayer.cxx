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
#include "MIDISongPlayer.hxx"
#include "MIDITrack.hxx"
#include "MIDISong.hxx"


namespace MIDI
{

	class TrackPlayer
	/* a helper class for SongPlayerImpl to iterate through each track */
	{
	friend class SongPlayerImpl;
	private:
		Track* mTrack;
		std::list<Event*>::const_iterator mIterator;
	public:
		TrackPlayer(Track* track);

	};

	TrackPlayer::TrackPlayer(Track* track)
	{
		mTrack = track;
		mIterator =  mTrack->Begin();
	}

	class SongPlayerImpl
	{
	/* hidden implementation of class SongPlayer */
	friend class SongPlayer;
	private:
		Song* mSong;
		std::vector<TrackPlayer*> mTrackPlayerList;
		SongPlayerImpl(Song* song)
		{
			Init(song);
		}
		
		void Init(Song* song)
		{
			mTrackPlayerList.clear();
			mSong = song;
			if (mSong)
			{
				for (int i=0;i<mSong->Tracks();i++)
				{
					mTrackPlayerList.push_back(
						new TrackPlayer(mSong->GetTrack(i)));
				}
			}
		}

		bool GetEvent(Event& event,int& trackId)
		{
			Ticks smallest = 0;
			int smallestId = -1;

			for (unsigned int i=0;i<mTrackPlayerList.size();i++)
			/* look for the track with the next event */
			{
				if (mTrackPlayerList[i]->mIterator != 
					mTrackPlayerList[i]->mTrack->End())
				{
					Event* ev = *mTrackPlayerList[i]->mIterator;
					if (smallestId==-1 || ev->GetTicks()<smallest)
					{
						smallest = ev->GetTicks();
						smallestId = i;
					}
				}
			}
			if (smallestId != -1)
			{
				/* return it and increment the iterator of that track */
				event = *(*(mTrackPlayerList[smallestId]->mIterator));
				trackId = smallestId;
				mTrackPlayerList[smallestId]->mIterator++;
				return true;
			}
			return false;
		}

	};
	
	SongPlayer::SongPlayer(Song* song)
	{
		mImpl = new SongPlayerImpl(song);
	}

	void SongPlayer::Init(Song* song)
	{
		mImpl->Init(song);
	}
	
	SongPlayer::~SongPlayer()
	{
		delete mImpl;
	}

	bool SongPlayer::GetEvent(Event& event,int& trackId)
	{
		return mImpl->GetEvent(event,trackId);
	}
	
}



