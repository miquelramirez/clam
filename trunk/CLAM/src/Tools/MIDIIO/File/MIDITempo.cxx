#include "MIDITrack.hxx"
#include "MIDISong.hxx"
#include "MIDITempo.hxx"
#include "DataTypes.hxx"

namespace MIDI
{

	using CLAM::TInt64;

	class TempoImpl
	/* hidden implementation of class Tempo */
	{
	friend class Tempo;
	private:
		Song* mSong;
		Track* mTrack;
		Track::EventIterator mIterator;
		int mUsPerQ; // microseconds per quarternote
		
		/* while iterating through the tempo events, we need to keep track of
		** the time, by applying all the tempo changes and calculation the 
		** time increment. these two vars are used in the process
		*/
		int mLastTicks; 
		int mLastTime;
		
		TempoImpl(Song* song = 0,Track* track = 0)
		{
			Init(song,track);
		}
		
		void Init(Song* song = 0,Track* track = 0)
		{
			mSong = song;
			mTrack = track;
			if (mSong && mTrack==0)
			{
				for (int i=0;i<mSong->Tracks();i++)
				{
					Track* t = mSong->GetTrack(i);
					if (t->HasTempoEvents())
					{
						mTrack = t;
						break;
					}
				}
			}
			if (mTrack)
			{
				mIterator = mTrack->Begin();
			}
			mUsPerQ = 500000;
			mLastTime = 0;
			mLastTicks = 0;
		}

		Milliseconds TicksToTime(Ticks t)
		{
			int i = 0;

			/* move the iterator to the next tempo event */
			while (mIterator!=mTrack->End())
			{
				const Event &ev = **mIterator;
				if ( ev[0]==0xFF && ev[1]==0x51)
				{
					break;
				}
				mIterator++;
			}

			/* if we are at the end of the tempo track, or we are at a tempo 
			** event _after_ Ticks t, then we start from the beginning
			*/
			if (mIterator==mTrack->End() || ((*mIterator)->GetTicks()>t))
			{
				mIterator = mTrack->Begin();
				mUsPerQ = 500000;
				mLastTime = 0;
				mLastTicks = 0;
			}

			std::list<Event*>::const_iterator prevIterator = mIterator;


			/* look for the first tempo event after Ticks t, and adjust
			** mLastTime and mLastTicks while doing this
			*/
			while (mIterator!=mTrack->End())
			{
				const Event &ev = **mIterator;
				if ( ev[0]==0xFF && ev[1]==0x51 )
				{
					if (ev.GetTicks()>t)
					{
						break;
					}

					MetaEvent* ev = (MetaEvent*) *mIterator;

					// the following is to say: 
					// (ticks/ticksPerQ)*msPerQ
					// but we change the order to stay with integers
					mLastTime += 
						( (TInt64(ev->GetTicks() - mLastTicks) * TInt64(mUsPerQ)) / 
							(TInt64(mSong->GetTicksPerQ())*TInt64(1000)) );

					mUsPerQ = 
						(ev->mData[0]<<16) | 
						(ev->mData[1]<<8) |
						(ev->mData[0]);

					mLastTicks = ev->GetTicks();

					prevIterator = mIterator;
				}
				mIterator++;
				i++;
			}

			/* move one back, to the event before or at Ticks t */
			mIterator = prevIterator;

			// the following is to say: 
			// (ticks/ticksPerQ)*msPerQ
			// but we change the order to stay with integers
			return mLastTime + Milliseconds(
					(TInt64(t - mLastTicks) * TInt64(mUsPerQ)) / 
					(TInt64(mSong->GetTicksPerQ())*TInt64(1000))); 
		}
	};

	Tempo::Tempo(Song* song,Track* track)
	{
		mImpl = new TempoImpl(song,track);
	}

	Tempo::~Tempo()
	{
		delete mImpl;
	}

	void Tempo::Init(Song* song,Track* track)
	{
		mImpl->Init(song,track);
	}
	
	Milliseconds Tempo::TicksToTime(Ticks t)
	{
		return mImpl->TicksToTime(t);
	}
	
};

