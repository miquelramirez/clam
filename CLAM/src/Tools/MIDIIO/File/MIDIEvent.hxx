#ifndef __MIDIEvent__
#define __MIDIEvent__

#include "MIDIMessage.hxx"

namespace MIDI
{

	class Event
	/* a midi event is a time-stamped midi message */
	{
	private:
		Message mMessage;
		Ticks   mTicks;
	public:
		Event()
		{
			mTicks = 0;
		}

		Event(const Message& m,const Ticks& t):mMessage(m),mTicks(t)
		{
		}
		
		Byte operator [] (int i) const { return mMessage[i]; }
		
		Ticks GetTicks(void) const { return mTicks; }
	};


	/* meta and sysex events are really different, can be any length, etc
	** so we subclass Event for them
	*/
	
	class MetaEvent:public Event
	{
	public:
		Byte *mData;
		MetaEvent(const Message& m,const Ticks& t,int datasize):Event(m,t)
		{
			mData = new Byte[datasize];
		}
	};

	class SysExEvent:public Event
	{
	public:
		Byte *mData;
		SysExEvent(const Message& m,const Ticks& t,int datasize):Event(m,t)
		{
			mData = new Byte[datasize];
		}
	};

}

#endif
