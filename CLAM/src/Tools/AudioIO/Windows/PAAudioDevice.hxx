#ifndef __PAAUDIODEVICE__
#define __PAAUDIODEVICE__

#include "AudioIO.hxx"
#include <portaudio.h>
#include "PortAudioUtils.hxx"
#include "PAAudioInputStream.hxx"
#include "PAAudioOutputStream.hxx"
#include "PAAudioFullDuplexStream.hxx"

#include "DoubleBuffer.hxx"

namespace CLAM
{

extern AudioDeviceList *sPAAudioDeviceList;

class PAAudioDevice : public AudioDevice
{
	enum IOModalities
	{
		eFullDuplex=0,
		eHalfDuplexIn,
		eHalfDuplexOut,
		eNoneYet
	};

public:

	PAAudioDevice( const std::string& str, PaDeviceID devID );
	~PAAudioDevice( );

	void Start() throw( ErrPortAudio );
	void Stop() throw ( ErrPortAudio );
	void Read( Audio& samples, const int channelID );
	void Write( const Audio& samples, const  int channelID );
	
	virtual void GetInfo( AudioDevice::TInfo& );
	
protected:

	void SetupMonoOutputStream() throw( ErrPortAudio );
	void SetupStereoOutputStream() throw( ErrPortAudio );
	void SetupMultiOutputStream() throw( ErrPortAudio );
	void SetupMonoInputStream() throw( ErrPortAudio );
	void SetupStereoInputStream() throw( ErrPortAudio );
	void SetupMultiInputStream() throw( ErrPortAudio );
	void SetupStereoFullDuplexStream() throw( ErrPortAudio );

private:

	unsigned                 mNChannelsWritten;
	bool                     mChannelsWritten[256];

	unsigned                 mNChannelsRead;
	bool                     mChannelsRead[256];

	unsigned                 mWriteBuffSize;
	unsigned                 mReadBuffSize;

	PaDeviceID               mDevID;

	DoubleBuffer             mInputIntBuffer;
	DoubleBuffer             mOutputIntBuffer;

	PAAudioInputStream*      mInStream;
	PAAudioOutputStream*     mOutStream;
	PAAudioFullDuplexStream* mFullDuplexStream;

	IOModalities             mIOModel ;

	bool                     mStarted;
};


class PAAudioDeviceList : public AudioDeviceList
{
private:
	static PAAudioDeviceList mDevices;
	
	PAAudioDeviceList();

	std::vector< PaDeviceID > mDevIDs;

protected:

	void EnumerateAvailableDevices();

public:

	virtual ~PAAudioDeviceList();

	inline std::string DefaultDevice()
	{
		return mAvailableDevices[0];
	}

	AudioDevice* Create( const std::string& name, const std::string& device );
	
};

}

#endif // PAAudioDevice.hxx
