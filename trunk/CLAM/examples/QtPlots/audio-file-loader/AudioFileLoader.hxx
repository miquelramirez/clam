#ifndef _AUDIOFILELOADER_
#define _AUDIOFILELOADER_

#include <vector>
#include "Audio.hxx"

using CLAM::Audio;

class AudioFileLoader
{
public:
	AudioFileLoader();
	~AudioFileLoader();

	int Load(const char* fileName,Audio& out);
	int LoadST(const char* fileName,std::vector<Audio>& outputs);
};

#endif
