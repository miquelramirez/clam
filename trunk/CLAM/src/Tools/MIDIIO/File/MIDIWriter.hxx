#ifndef __MIDIWRITER__
#define __MIDIWRITER__

#include <stdio.h>

#define MThd 0x4d546864
#define MTrk 0x4d54726b

namespace MIDI
{
	class Song;
	
	class Writer
	{
	public:
		Writer(const char* filename);
		~Writer();

		void Write(Song& s);

	private:
		FILE* mFile;
		long  mBytesWritten;
		int   mTracks;
		int   mFormat;
		int   mDivision;
		
		void WriteHeader();
		void WriteVarLen(register unsigned long value);
		void Write32Bit(unsigned long data);
		void Write16Bit(int data);
		void WriteCh(char c);
	};
}

#endif

