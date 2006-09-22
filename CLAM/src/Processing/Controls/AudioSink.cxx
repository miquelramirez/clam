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

	bool AudioSink::Do( TData* buf, int nframes)
	{
		if (!mIn.CanConsume())
		{
			//std::cout << "_inport cant consume" << std::endl;
			return true;
		}

		const CLAM::Audio& so=mIn.GetAudio();
		
		//We assume that external audio type is the same as ours, range (-1,1) in a float
		std::copy(so.GetBuffer().GetPtr(),so.GetBuffer().GetPtr()+nframes, buf);
		mIn.Consume();

		return true;
	}
} //namespace CLAM
