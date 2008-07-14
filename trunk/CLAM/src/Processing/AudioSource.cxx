#include "AudioSource.hxx"
#include "ProcessingFactory.hxx"
#include "Audio.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "AudioSource",
		"category", "Audio I/O",
		"description", "AudioSource",
		"icon", "source.svg",
		"embedded_svg", "source.svg",
		0
	};
	static FactoryRegistrator<ProcessingFactory, AudioSource> reg = metadata;
}

bool AudioSource::Do()
{
	CLAM::Audio& so=mOut.GetAudio();
	CLAM_DEBUG_ASSERT(mFloatBuffer, "No float buffer");
	CLAM_DEBUG_ASSERT(!mDoubleBuffer, "There should not be double buffer");
	CLAM_DEBUG_ASSERT(mBufferSize>0, "internal buffer size must be greater than 0");
	CLAM::TData * audioBuffer = so.GetBuffer().GetPtr();
	for (unsigned i=0; i<mBufferSize; i++)
		audioBuffer[i] = mFloatBuffer[i];
	mOut.Produce();
	return true;
}

void AudioSource::SetExternalBuffer( float* buf, unsigned nframes)
{
	mFloatBuffer = buf;
	mBufferSize = nframes;
	mDoubleBuffer = 0;
	mOut.SetSize(nframes);
	mOut.SetHop(nframes);
}
void AudioSource::SetExternalBuffer( double* buf, unsigned nframes)
{
	mDoubleBuffer = buf;
	mBufferSize = nframes;
	mFloatBuffer = 0;
	mOut.SetSize(nframes);
	mOut.SetHop(nframes);
}

} //namespace CLAM

