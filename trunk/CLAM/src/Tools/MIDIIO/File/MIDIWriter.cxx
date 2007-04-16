#include "MIDITrack.hxx"
#include "MIDISong.hxx"
#include "MIDIWriter.hxx"

namespace MIDI
{
	Writer::Writer(const char* filename)
		: mFile(0)
		, mBytesWritten(0)
	{
		mFile = fopen(filename,"wb");
	}

	Writer::~Writer()
	{
	}

	void Writer::Write(Song& s)
	{
		if(!mFile) return;

		static int nbytesPerChnMsg[7] = { 3,3,3,3,2,3,3 };

		mTracks = s.Tracks();
		mFormat = (mTracks > 1) ? 1 : 0;
		mDivision = (int)s.GetTicksPerQ();

		WriteHeader();
		
		unsigned long trkhdr=MTrk;
		long offset,endt;
		unsigned t0,t1;
		for(int i=0; i < mTracks; i++)
		{
			// write track header
			Write32Bit(trkhdr);
			// remember position
			offset = ftell(mFile);
			Write32Bit(0);
			mBytesWritten=0;
			t0=t1=0;
			Track* track = s.GetTrack(i);
			Track::EventIterator it = track->Begin();
			for(;it != track->End(); it++)
			{
				const Event &ev = **it;
				t1=(unsigned)ev.GetTicks();
				int type = ((ev[0]&0xF0)>>4)-8;
				if(type==7)
				{
					if(ev[0]==0xFF && ev[1]==0x51)
					{
						// write tempo information
						WriteVarLen(t1-t0);
						WriteCh(0xFF);
						WriteCh(0x51);
						WriteCh(0x03);
						MetaEvent* e = (MetaEvent*)*it;
						for(int k=0; k < 3; k++)
						{
							WriteCh(e->mData[k]);
						}
					}
				}
				else
				{
					WriteVarLen(t1-t0); // write delta time
					int msglen = nbytesPerChnMsg[type];
					for(int j=0; j < msglen; j++) 
					{
						WriteCh((char)ev[j]);
					}
				}
				t0=t1;
				
			}
			// write end of track
			WriteCh(0);
			WriteCh((unsigned)0xFF);
			WriteCh((unsigned)0x2f);
			WriteCh(0);
			endt = ftell(mFile);
			fseek(mFile,offset,0);
			// write track length
			Write32Bit(mBytesWritten);
			// retrieve position
			fseek(mFile,endt,0);
		}
		fclose(mFile);
	}

	void Writer::WriteHeader()
	{
		unsigned long id=MThd;
		
		Write32Bit(id);  	   // MThd 
		Write32Bit(6);		   // header length = 6
		Write16Bit(mFormat);   // format 
		Write16Bit(mTracks);   // ntracks 
		Write16Bit(mDivision); // division
	}

	void Writer::WriteVarLen(register unsigned long value)
	{
		register unsigned long buffer;
		buffer = value & 0x7F;

		while( (value >>= 7) )
		{
			buffer <<= 8;
			buffer |= ((value & 0x7F) | 0x80);
		}

		while(1)
		{
			WriteCh(buffer);
			if (buffer & 0x80)
				buffer >>= 8;
			else
				break;
		}
	}

	void Writer::Write32Bit(unsigned long data)
	{
		WriteCh((unsigned)((data >> 24) & 0xff));
		WriteCh((unsigned)((data >> 16) & 0xff));
		WriteCh((unsigned)((data >> 8 ) & 0xff));
		WriteCh((unsigned)(data & 0xff));
	}

	void Writer::Write16Bit(int data)
	{
		WriteCh((unsigned)((data & 0xff00) >> 8));
		WriteCh((unsigned)(data & 0xff));
	}

	void Writer::WriteCh(char c)
	{
		fputc(c,mFile);
		mBytesWritten++;
	}
}

// END

