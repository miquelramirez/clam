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
		, mFloatBuffer(0)
		, mDoubleBuffer(0)
		, mBufferSize(0)
	{
		//After being dropped it is ready to run as it does not need any configuration at all
		mExecState=Ready;
	}

	AudioSource::AudioSource(const ProcessingConfig & conf)
		: mOut("AudioOut",this)
		, mFloatBuffer(0)
		, mDoubleBuffer(0)
		, mBufferSize(0)
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

	void AudioSource::SetExternalBuffer( float* buf, unsigned nframes)
	{
		mFloatBuffer = buf;
		mBufferSize = nframes;
		mDoubleBuffer = 0;
	}
	void AudioSource::SetExternalBuffer( double* buf, unsigned nframes)
	{
		mDoubleBuffer = buf;
		mBufferSize = nframes;
		mFloatBuffer = 0;
	}

	bool AudioSource::Do( double* buf, int nframes)
	{		
		if (!mOut.CanProduce())
			return true;

		CLAM::Audio& so=mOut.GetAudio();
		for (int i=0; i<nframes; i++)
			so.GetBuffer().GetPtr()[i] = buf[i];

		mOut.Produce();
		return true;
	}
	bool AudioSource::Do( float* buf, int nframes)
	{		
		if (!mOut.CanProduce())
			return true;

		CLAM::Audio& so=mOut.GetAudio();
		for (int i=0; i<nframes; i++)
			so.GetBuffer().GetPtr()[i] = buf[i];

		mOut.Produce();
		return true;
	}
} //namespace CLAM
