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



