#ifndef __SoundFileIOError__
#define __SoundFileIOError__

namespace CLAM {

class ErrSoundFileIO
{
public:
	ErrSoundFileIO(char* str):mStr(str) {}
	char* mStr;
};

};//CLAM

#endif
