#include "SoundHeader.hxx"

using namespace CLAM;

SoundHeader::SoundHeader(int samplerate,int channels,int sampleWidth)
{
	mSamplerate = samplerate;
	mChannels = channels;
	mSampleWidth = sampleWidth;
	mBytesPerSample = (mSampleWidth+7)>>3;
}

