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

		ChunkType chnkType = GetChunkType();
		unsigned int length;
		unsigned short format,ntrcks;
		
		if (chnkType!="MThd") throw Error("Expected a header chunk\n");

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
			chnkType = GetChunkType();
			if (chnkType!="MTrk") throw Error("Expected a track chunk\n");

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
						else if (b == 0xF0 || b==0xF7)
						{
							/* TODO: For now, we'll just skip SysEx events */
							int length = GetVarLength();
							while (--length)
							{
								GetByte();
							}
							if (GetByte()!=0xF7)
							{
								throw Error("SysEx message did not terminate with 0xF7");
							}
						}
						else
						{
							throw Error("Encountered a message that I don't know how to handle");
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

