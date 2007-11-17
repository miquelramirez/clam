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
		0
	};
	//static FactoryRegistrator<ProcessingFactory, AudioSource> regAudioSource("AudioSource");
	static FactoryRegistrator<ProcessingFactory, AudioSource> reg = metadata;
}
	
AudioSource::AudioSource() 
	: mOut("AudioOut",this)
	, mFloatBuffer(0)
	, mDoubleBuffer(0)
	, mBufferSize(0)
{
	//After being dropped it is ready to run as it does not need any configuration at all
	SetExecState(Ready);
}

AudioSource::AudioSource(const ProcessingConfig & conf)
	: mOut("AudioOut",this)
	, mFloatBuffer(0)
	, mDoubleBuffer(0)
	, mBufferSize(0)
{
	//After being dropped it is ready to run as it does not need any configuration at all
	SetExecState(Ready);
}

AudioSource::~AudioSource()
{
}

bool AudioSource::Do()
{
	CLAM::Audio& so=mOut.GetAudio();
	CLAM_DEBUG_ASSERT(mFloatBuffer, "No float buffer");
	CLAM_DEBUG_ASSERT(!mDoubleBuffer, "There should not be double buffer");
	CLAM::TData * audioBuffer = so.GetBuffer().GetPtr();
	for (unsigned i=0; i<mBufferSize; i++)
		audioBuffer[i] = mFloatBuffer[i];
	mOut.Produce();
	std::cout << "   AudioSource Done. Size :" << mOut.GetSize() << std::endl;
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

