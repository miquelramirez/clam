#ifndef __MIDITrack__
#define __MIDITrack__

#include <list>
#include "MIDIEvent.hxx"

namespace MIDI
{

	class Track
	{
	private:
		/* a midi track consists of a list of midi event */
		std::list<Event*> mEventList; 
		
		/* and may have a name */
		char* mName;
		
	public:
		Track(void)
		:mName(0)
		{
		}

		~Track(void)
		{
			if (mName) delete mName;
		}

		void Add(Event* e)
		{
			mEventList.push_back(e);
		}

		void Name(const Byte* ptr, int len)
		{
			if (mName) delete mName;
			mName = new char[len+1];
			for (int i=0;i<len;i++) mName[i] = ptr[i];
			mName[len] = 0;
		}


		/* iterator to traverse the list of events */
		typedef std::list<Event*>::const_iterator EventIterator;

		EventIterator Begin(void) { return mEventList.begin(); }
		EventIterator End(void) { return mEventList.end(); }

		bool HasTempoEvents(void)
		{
			EventIterator it = Begin();
			
			while (it!=End())
			{
				const Event &ev = **it;
        if ( ev[0]==0xFF && ev[1]==0x51 ) return true;
				it++;
			}
			return false;
		}

	};

}

#endif
