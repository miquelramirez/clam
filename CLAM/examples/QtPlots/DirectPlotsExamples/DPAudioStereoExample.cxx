#include <stdlib.h>
#include "AudioFileLoader.hxx"
#include "DPAudioStereo.hxx"

int main()
{
	std::vector<Audio> channels;
	AudioFileLoader fLoader;
	
	// get audio data
	int err = fLoader.LoadST("../../data/imagine.mp3",channels);
	if(err)
	{
		printf("\'imagine.mp3\' audio file not found!\n");
		exit(1);
	}

	CLAM::VM::PlotAudioStereo(channels); 

	return 0;
}


// END
