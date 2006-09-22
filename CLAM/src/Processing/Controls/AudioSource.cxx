#include "AudioSource.hxx"
#include "Factory.hxx"
#include "Audio.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<AudioSource> regtAudioSource( "AudioSource" );
	}
	
	AudioSource::AudioSource()
		: mOut("AudioOut",this)
	{
		//After being dropped it is ready to run as it does not need any configuration at all
		mExecState=Ready;
	}

	AudioSource::AudioSource(const ProcessingConfig & conf)
		: mOut("AudioOut",this)
	{
		//After being dropped it is ready to run as it does not need any configuration at all
		mExecState=Ready;
	}

	AudioSource::~AudioSource()
	{
	}

	bool AudioSource::Do()
	{
		return true;
	}

	bool AudioSource::Do( TData* buf, int nframes)
	{		
		if (!mOut.CanProduce())
		{
			//std::cout << "_outport cant produce" << std::endl;
			return true;
		}

		CLAM::Audio& so=mOut.GetAudio();

		//We assume that external audio type is the same as ours, range (-1,1) in a float
		std::copy(buf, buf+nframes, so.GetBuffer().GetPtr());
		mOut.Produce();

		return true;
	}
} //namespace CLAM
