#ifndef __MIDIMessage__
#define __MIDIMessage__

#include "MIDIDataTypes.hxx"

namespace MIDI
{

	class Message
	/* an ordinary MIDI message contains between 2 and 4 bytes. reserve
	** space for the maximum, 4. */
	{
	friend class Event;
	public:
		Message()
		{
			mData.mStatus = 0;
			mData.mData1 = 0;
			mData.mData2 = 0;
			mData.mData3 = 0;
		}

		Message(Byte status,Byte data1,Byte data2 = 0,Byte data3 = 0)
		{
			mData.mStatus = status;
			mData.mData1 = data1;
			mData.mData2 = data2;
			mData.mData3 = data3;
		}

		Byte operator [] (int i) const { return mVal[i]; }

	private:
		union
		{
			struct
			{
				Byte mStatus;
				Byte mData1;
				Byte mData2;
				Byte mData3;
			} mData;

			Byte mVal[4];
		};
	};

}

#endif
