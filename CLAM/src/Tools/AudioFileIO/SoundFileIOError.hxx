#ifndef __SoundFileIOError__
#define __SoundFileIOError__

class SoundFileIOError
{
public:
	SoundFileIOError(char* str):mStr(str) {}
	char* mStr;
};

#endif
