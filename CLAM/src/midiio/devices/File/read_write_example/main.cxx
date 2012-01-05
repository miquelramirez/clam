#include "../MIDISong.hxx"
#include "../MIDIReader.hxx"
#include "../MIDIWriter.hxx"

using namespace MIDI;

int main(int argc, char** argv)
{
	char* filein;
	char* fileout;

	switch(argc)
	{
		case 1:
			filein="../test.mid";
			fileout="midi_out.mid";
			break;
		case 2:
			filein=argv[1];
			fileout="midi_out.mid";
			break;
		case 3:
			filein=argv[1];
			fileout=argv[2];
			break;
	}

	Song s;
	Reader r(filein);
	Writer w(fileout);
	r.Read(s);
	w.Write(s);
	return 0;
}

// END

