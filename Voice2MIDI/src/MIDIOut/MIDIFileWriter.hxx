/*
 * MIDIFileWriter.hxx
 * Interface for MIDIFileWriter class
 * Used to write a MIDI file from the obtained MIDI melody.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/

#ifndef _MIDIFILEWRITER_
#define _MIDIFILEWRITER_

#include <string>
#include <CLAM/MIDIMelody.hxx>

#define MThd	0x4d546864
#define MTrk	0x4d54726b

#define DIVISION	96
#define TEMPO	   120

using CLAM::MIDIMelody;

namespace V2M
{

class MIDIFileWriter
{
public:
	MIDIFileWriter();
	~MIDIFileWriter(){}

	void Write(const MIDIMelody& melody,int program,const std::string& filename);

private:
	FILE* fp;
	MIDIMelody _melody;
	int _program;
	long _numBytesWritten;

	void WriteHeader();
	void WriteTrack();
	void WriteVarLen(register unsigned long value);
	void Write32Bit(unsigned long data);
	void Write16Bit(int data);
	void EPutc(char c);
	unsigned long Sec2Ticks(float secs,int division,unsigned tempo);
};

} // namespace V2M

#endif

