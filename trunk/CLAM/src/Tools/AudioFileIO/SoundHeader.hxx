#ifndef __SoundHeader__
#define __SoundHeader__

namespace CLAM {

class SoundHeader
{
public:
	int   mSamplerate;
	int   mChannels;
	int   mBytesPerSample;
	int   mSampleWidth;
	short mFormatTag;
	SoundHeader(int samplerate = 44100,int channels = 1,int sampleWidth = 16, short mFormatTag = 0x0001);
};

};//CLAM
#endif
