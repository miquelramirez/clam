#include "MIDIReader.hxx"
#include "MIDISong.hxx"
#include "MIDITrack.hxx"

namespace MIDI
{

	void Reader::Read(Song& s)
  /* ToDo: document and explain this function */
	{
		static int nbytesPerChnMsg[7] =
		{ 3,3,3,3,2,3,3 };

		unsigned int chnkType = GetInt();
		unsigned int length;
		unsigned short format,ntrcks;
		
		if (chnkType!=0x4D546864) throw Error("Expected a header chunk\n");

		length = GetInt();
		if (length!=6)
			throw Error("Unexpected header chunk length\n");

		format = GetShort();
		if (format!=0 && format!=1)
			throw Error("Unsupported MIDI file format\n");

		ntrcks = GetShort();

		s.SetTicksPerQ(GetShort());

		for (int i=0;i<ntrcks;i++)
		{
			chnkType = GetInt();
			if (chnkType!=0x4D54726B) throw Error("Expected a track chunk\n");

			int chnkLength = GetInt();

			Byte runningStatus = 0;
			Ticks t = 0;
			Track *track = new Track;
			s.AddTrack(track);

			mCnt = 0;

			while (mCnt!=chnkLength)
			{
				unsigned int dt = GetVarLength();
				t += dt;
				Byte b = GetByte();
				if (b & 0x80)
				{
					int type = ((b&0xF0)>>4)-8;
					if (type==7)
					{
						if (b == 0xFF)
						{
							Byte metaType = GetByte();
							unsigned int length = GetVarLength();
							if (metaType!=0x2F) // don't add end-of-track events
							{
								MetaEvent* ev = new MetaEvent(Message(b,metaType),t,length);

								track->Add(ev);

								for (unsigned int i=0;i<length;i++)
									ev->mData[i] = GetByte();

								if (metaType == 3) // sequence/track name
								{
									track->Name(ev->mData,length);
								}
							}
						}
						else
						{
							throw Error("Currently only supporting MIDI and META events\n");
						}
						runningStatus = 0;					
					}else{
						if (nbytesPerChnMsg[type]==2)
						{
							Byte b1 = GetByte();
							track->Add(new Event(Message(b,b1),t));
						}
						else
						{
							Byte b1 = GetByte();
							Byte b2 = GetByte();
							track->Add(new Event(Message(b,b1,b2),t));
						}
						runningStatus = b;
					}
				}else{
					int type = ((runningStatus&0xF0)>>4)-8;
					if (nbytesPerChnMsg[type]==2)
					{
						track->Add(new Event(Message(runningStatus,b),t));
					}
					else
					{
						Byte b2 = GetByte();
						track->Add(new Event(Message(runningStatus,b,b2),t));
					}
				}
			}
		}	
	}

}
