#ifndef __MIDIReader__
#define __MIDIReader__

#include <stdio.h>
#include "MIDIDataTypes.hxx"

namespace MIDI
{
	class Song;

	class Reader
	/* class to read midi data from a file, parse it and store it in a song;
	*/
	{
	private:
		FILE* mFile;
		int mCnt;
	public:
		Reader(const char* filename)
		{
			mFile = fopen(filename,"rb");
		}
		bool Ok(void)
		{
			return mFile!=NULL;
		}
		Byte GetByte(void)
		{
			mCnt++;
			return fgetc(mFile);
		}
		unsigned int GetInt(void)
		{
			unsigned int val;
			fread(&val,4,1,mFile);
	    	unsigned char* ptr=(unsigned char*) &val;
	    	unsigned char tmp;
			#ifdef MIDI_FILE_NEEDS_SWAP
   	 	tmp=ptr[0]; ptr[0]=ptr[3]; ptr[3]=tmp;
   	 	tmp=ptr[1]; ptr[1]=ptr[2]; ptr[2]=tmp;
			#endif
			return val;
		}
		unsigned short GetShort(void)
		{
			unsigned short val;
			fread(&val,2,1,mFile);
   	 	unsigned char* ptr=(unsigned char*) &val;
   	 	unsigned char tmp;
			#ifdef MIDI_FILE_NEEDS_SWAP
   	 	tmp=ptr[0]; ptr[0]=ptr[1]; ptr[1]=tmp;
			#endif
			return val;
		}
		unsigned int GetVarLength(void)
		{
			/* read a variable lenght value. see midi file spec */
			unsigned int ret = 0;
			Byte tmp;

			tmp = GetByte();
			ret = tmp&0x7F;
			while (tmp&0x80)
			{
				tmp = GetByte();
				ret <<= 7;
				ret |= (tmp&0x7F);
			}
			return ret;
		}
		class Error
		{
		public:
			const char* mStr;
			Error(const char* str):mStr(str) { printf(str); }
		};

		void Read(Song& s);
	};

}

#endif
