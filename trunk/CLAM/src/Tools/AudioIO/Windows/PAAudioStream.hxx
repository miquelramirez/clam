#ifndef __PAAUDIOSTREAM__
#define __PAAUDIOSTREAM__

#include <portaudio.h>
#include "PortAudioUtils.hxx"
#include "DoubleBuffer.hxx"

namespace CLAM
{

class PAAudioStreamConfig
{
public:

	PAAudioStreamConfig()
		: mSampleRate( 44100 ), mNChannels(2), 
		  mCallback( NULL )
	{
	}

	~PAAudioStreamConfig()
	{
	}

	inline void SetSampleRate( unsigned srate )
	{
		mSampleRate = srate;
	}

	inline unsigned GetSampleRate() const
	{
		return mSampleRate;
	}

	inline void SetChannelNumber( unsigned char nchann )
	{
		mNChannels = nchann;
	}

	inline unsigned char GetChannelNumber() const
	{
		return mNChannels;
	}

	inline void SetCallback( PortAudioCallback  cb )
	{
		mCallback = cb;
	}

	inline PortAudioCallback GetCallback( ) const
	{
		return mCallback;
	}

	inline void SetInputDblBuffer( DoubleBuffer* buffer )
	{
		mBuffer.mInputDblBuff = buffer;
	}

	inline DoubleBuffer* GetInputDblBuffer()
	{
		return mBuffer.mInputDblBuff;
	}

	inline void SetOutputDblBuffer( DoubleBuffer* buffer )
	{
		mBuffer.mOutputDblBuff = buffer;
	}

	inline DoubleBuffer* GetOutputDblBuffer()
	{
		return mBuffer.mOutputDblBuff;
	}

	inline FullDuplexDoubleBuffer* GetDblBuffer()
	{
		return &mBuffer;
	}

	inline void SetDeviceID( PaDeviceID devID )
	{
		mDevID = devID;
	}

	inline PaDeviceID GetDeviceID( ) const
	{
		return mDevID;
	}

private:
	
	unsigned                 mSampleRate;
	unsigned char            mNChannels;
	PaDeviceID               mDevID;
	PortAudioCallback        mCallback;
	FullDuplexDoubleBuffer   mBuffer;

};

class PAAudioStream
{
public:

	PAAudioStream( PAAudioStreamConfig& );

	void Start();

	void Stop();

	~PAAudioStream();

protected:

	virtual void SetupStream() = 0;

	
	PAAudioStreamConfig  mConfig;
	PortAudioStream*     mStream;
};

}

#endif
