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
	CLAM_DEBUG_ASSERT(!mFloatBuffer || !mDoubleBuffer, "AudioSource: Just one buffer should be set");
	CLAM_DEBUG_ASSERT(mFloatBuffer || mDoubleBuffer, "AudioSource: No external buffer set");
	CLAM_DEBUG_ASSERT(mBufferSize>0, "AudioSource: internal buffer size must be greater than 0");
	CLAM::TData * audioBuffer = so.GetBuffer().GetPtr();
	if (mFloatBuffer)
		for (unsigned i=0; i<mBufferSize; i++)
			audioBuffer[i] = mFloatBuffer[i];
	else
		for (unsigned i=0; i<mBufferSize; i++)
			audioBuffer[i] = mDoubleBuffer[i];
	mOut.Produce();
	return true;
}

void AudioSource::SetExternalBuffer(const float* buf, unsigned nframes)
{
	mFloatBuffer = buf;
	mBufferSize = nframes;
	mDoubleBuffer = 0;
	mOut.SetSize(nframes);
	mOut.SetHop(nframes);
}
void AudioSource::SetExternalBuffer(const double* buf, unsigned nframes)
{
	mDoubleBuffer = buf;
	mBufferSize = nframes;
	mFloatBuffer = 0;
	mOut.SetSize(nframes);
	mOut.SetHop(nframes);
}

} //namespace CLAM

