#ifndef __SoundFileIOError__
#define __SoundFileIOError__

class ErrSoundFileIO
{
public:
	ErrSoundFileIO(char* str):mStr(str) {}
	char* mStr;
};

#endif
