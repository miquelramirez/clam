#include "AudioSink.hxx"
#include "Factory.hxx"
#include "Audio.hxx"

#include <iostream>

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<AudioSink> regtAudioSink( "AudioSink" );
	}
	
	AudioSink::AudioSink()
		: mIn("AudioIn",this)
	{
		//After being dropped it is ready to run as it does not need any configuration at all
		mExecState=Ready;
	}

	AudioSink::AudioSink(const ProcessingConfig & conf)
		: mIn("AudioIn",this)
	{
		//After being dropped it is ready to run as it does not need any configuration at all
		mExecState=Ready;
	}

	AudioSink::~AudioSink()
	{
	}

	bool AudioSink::Do()
	{
		return true;
	}

	bool AudioSink::Do( float* buf, int nframes)
	{
		if (!mIn.CanConsume())
			return true;

		const CLAM::Audio& so=mIn.GetAudio();
		for (int i=0; i<nframes; i++)
			buf[i] = so.GetBuffer().GetPtr()[i];
	
		mIn.Consume();
		return true;
	}
	bool AudioSink::Do( double* buf, int nframes)
	{
		if (!mIn.CanConsume())
			return true;

		const CLAM::Audio& so=mIn.GetAudio();
		for (int i=0; i<nframes; i++)
			buf[i] = so.GetBuffer().GetPtr()[i];
	
		mIn.Consume();
		return true;
	}
} //namespace CLAM
