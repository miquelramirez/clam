#include <stdlib.h>
#include "AudioFileLoader.hxx"
#include "DPAudio.hxx"

int main()
{
	Audio audio;
	AudioFileLoader fLoader;
	
	// get audio data
	int err = fLoader.Load("../../data/birthday.wav",audio);
	if(err)
	{
		printf("\'birthday.wav\' audio file not found!\n");
		exit(1);
	}

	CLAM::VM::PlotAudio(audio); 

	return 0;
}


// END
