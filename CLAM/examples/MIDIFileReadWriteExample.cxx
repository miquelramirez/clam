#include <iostream>
#include "MIDIFileReader.hxx"
#include "MIDIFileWriter.hxx"

using namespace CLAM;

int main(int argc, char** argv)
{
	const char* filein=0;
	const char* fileout=0;

	switch(argc)
	{
		case 1:
			filein="../../../../src/Tools/MIDIIO/File/test.mid";
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

	MIDISong song;
	MIDIFileIOConfig cfg;
	cfg.SetFileName(filein);
	MIDIFileReader reader(cfg);

	std::cout << "reading song from " << filein << std::endl;

	reader.Start();
	reader.Do(song);
	reader.Stop();

	cfg.SetFileName("original_file.mid");
	MIDIFileWriter writer(cfg);

	std::cout << "storing a copy in original_file.mid" << std::endl; 

	writer.Start();
	writer.Do(song);
	writer.Stop();

	std::cout << "increasing all notes two tones..." << std::endl;
	for(int i=0; i < song.GetNumberOfTracks(); i++)
	{
		for(int j=0; j < song.GetTracks()[i].GetTrackMelody().GetNumberOfNotes(); j++)
		{
			if(song.GetTracks()[i].GetChannels()[j]==9) continue;
			int key = song.GetTracks()[i].GetTrackMelody().GetNoteArray()[j].GetKey();
			song.GetTracks()[i].GetTrackMelody().GetNoteArray()[j].SetKey(key+4);
		}
	}

	cfg.SetFileName(fileout);
	writer.Configure(cfg);

	std::cout << "storing the modified song in " << fileout << std::endl;
	
	writer.Start();
	writer.Do(song);
	writer.Stop();

	return 0;
}

// END

