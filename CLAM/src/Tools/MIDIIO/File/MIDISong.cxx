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
