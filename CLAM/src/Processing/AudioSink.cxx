#include "AudioSink.hxx"
#include "ProcessingFactory.hxx"
#include "Audio.hxx"

#include <iostream>

namespace CLAM
{

namespace Hidden
{
	static const char* metadata[] = {
		"key", "AudioSink",
		"category", "Audio I/O",
		"description", "AudioSink",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, AudioSink> regAudioSink("AudioSink");
	static FactoryRegistrator<ProcessingFactory, AudioSink> reg = metadata;
}
	
AudioSink::AudioSink()
	: mIn("AudioIn",this)
	, mFloatBuffer(0)
	, mDoubleBuffer(0)
	, mBufferSize(0)
{
	//After being dropped it is ready to run as it does not need any configuration at all
	SetExecState(Ready);
}

AudioSink::AudioSink(const ProcessingConfig & conf)
	: mIn("AudioIn",this)
	, mFloatBuffer(0)
	, mDoubleBuffer(0)
	, mBufferSize(0)
{
	//After being dropped it is ready to run as it does not need any configuration at all
	SetExecState(Ready);
}

AudioSink::~AudioSink()
{
}

bool AudioSink::Do()
{
	const CLAM::Audio& so=mIn.GetAudio();
	CLAM_DEBUG_ASSERT(mFloatBuffer, "No float buffer");
	CLAM_DEBUG_ASSERT(!mDoubleBuffer, "There should not be double buffer");
	const CLAM::TData * audioBuffer = so.GetBuffer().GetPtr();
	for (unsigned i=0; i<mBufferSize; i++)
		mFloatBuffer[i] = audioBuffer[i];
	mIn.Consume();
	return true;
}

void AudioSink::SetExternalBuffer( float* buf, unsigned nframes)
{
	mFloatBuffer = buf;
	mBufferSize = nframes;
	mDoubleBuffer = 0;
	mIn.SetSize(nframes);
	mIn.SetHop(nframes);
}
void AudioSink::SetExternalBuffer( double* buf, unsigned nframes)
{
	mDoubleBuffer = buf;
	mBufferSize = nframes;
	mFloatBuffer = 0;
	mIn.SetSize(nframes);
	mIn.SetHop(nframes);
}

} //namespace CLAM

