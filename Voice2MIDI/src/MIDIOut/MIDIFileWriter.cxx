/*
 * MIDIFileWriter.cxx
 * Implementation for MIDIFileWriter class
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/

#include "MIDIFileWriter.hxx"

namespace V2M
{

MIDIFileWriter::MIDIFileWriter()
{
	fp = NULL;
	_numBytesWritten = 0;
}

void MIDIFileWriter::Write(const MIDIMelody& melody,int program,const std::string& filename)
{
	_melody = melody;
	_program = program;

	fp = fopen(filename.c_str(),"w");
	WriteHeader();
	WriteTrack();
	fclose(fp);
}

void MIDIFileWriter::WriteVarLen(register unsigned long value)
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
		EPutc(buffer);
		if (buffer & 0x80)
			buffer >>= 8;
		else
			 break;
   }
}

void MIDIFileWriter::WriteHeader()
{
	unsigned long id=MThd;

    Write32Bit(id);  		// MThd 
    Write32Bit(6);			// header length = 6
    Write16Bit(0);			// format = 0
    Write16Bit(1);			// ntracks = 1
	Write16Bit(DIVISION);	// division
}

void MIDIFileWriter::WriteTrack()
{
	unsigned long trkhdr= MTrk,trklength=0;
	long offset,endf;
	
	// remember position
	offset = ftell(fp);

	// write header
	Write32Bit(trkhdr);
	Write32Bit(trklength);

	_numBytesWritten = 0;

	char *msg = new char[3];
    int i;

	// write tempo 
	WriteVarLen(0);
	EPutc(0xFF);
	EPutc(0x51);
	EPutc(0x03);
	EPutc(0x07);
	EPutc(0xA1);
	EPutc(0x20);

	// program
	EPutc(0);
	msg[0]=(unsigned)0xc0;	// channel 1
	msg[1]=_program;	// program
	for(i=0;i < 2;i++) EPutc((unsigned)msg[i]);

	// volume
	EPutc(0);
	msg[0]=(unsigned)0xb0;	// channel 1
	msg[1]=7;			// control 7
	msg[2]=120;			// value = 120
	for(i=0;i < 3;i++) EPutc((unsigned)msg[i]);

	int nNotes = _melody.GetNumberOfNotes();
	float t,t1,t2;

	t = 0.0f;
	
	// note on
	msg[0]=(unsigned)0x90;	// channel 1

	for(int k = 0;k < nNotes;k++)
	{
		t1 = float(_melody.GetNoteArray()[k].GetTime().GetBegin());
		t2 = float(_melody.GetNoteArray()[k].GetTime().GetEnd());
		
		// on
		WriteVarLen(Sec2Ticks(t1-t,DIVISION,TEMPO));
		msg[1]=_melody.GetNoteArray()[k].GetKey();
		msg[2]=_melody.GetNoteArray()[k].GetVelocity();
		for(i=0;i < 3;i++) EPutc(msg[i]);
		
		// off
		WriteVarLen(Sec2Ticks(t2-t1,DIVISION,TEMPO));
		msg[1]=_melody.GetNoteArray()[k].GetKey();
		msg[2]=0;
		for(i=0;i < 3;i++) EPutc(msg[i]);

		t = t2;
		
	}
	// write end of track
	EPutc(0);
	EPutc((unsigned)0xFF);
	EPutc((unsigned)0x2f);
 	EPutc(0);

	endf = ftell(fp);

	// write track length
	fseek(fp,offset,0);
	trklength = _numBytesWritten;
	Write32Bit(trkhdr);
	Write32Bit(trklength);
	
	fseek(fp,endf,0);
}

void MIDIFileWriter::Write32Bit(unsigned long data)
{
    EPutc((unsigned)((data >> 24) & 0xff));
    EPutc((unsigned)((data >> 16) & 0xff));
    EPutc((unsigned)((data >> 8 ) & 0xff));
    EPutc((unsigned)(data & 0xff));
}

void MIDIFileWriter::Write16Bit(int data)
{
    EPutc((unsigned)((data & 0xff00) >> 8));
    EPutc((unsigned)(data & 0xff));
}

void MIDIFileWriter::EPutc(char c)
{
	putc(c,fp);
	_numBytesWritten++;
}

unsigned long MIDIFileWriter::Sec2Ticks(float secs,int division,unsigned tempo)
{
     return (long)(((secs * 1000.0) / 4.0 * division) / tempo);
}

} // namespace V2M

// END

