#ifndef __MIDIDataTypes__
#define __MIDIDataTypes__

#include "ByteOrder.hxx"

#ifdef CLAM_LITTLE_ENDIAN
#define MIDI_FILE_NEEDS_SWAP
#else
#ifdef CLAM_BIG_ENDIAN
#undef MIDI_FILE_NEEDS_SWAP
#else
Error: unknown byte order
#endif
#endif


namespace MIDI
{

	typedef unsigned char Byte;
	typedef unsigned int  Ticks;
	typedef unsigned int  Milliseconds;


}

#endif
