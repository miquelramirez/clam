#ifndef __RTAAUDIODEVICE__
#define __RTAAUDIODEVICE__

#include "AudioDevice.hxx"
#include "AudioDeviceList.hxx"
#include "RtAudio.h"
#include "RtAudioUtils.hxx"

namespace CLAM
{

extern AudioDeviceList *sRtAAudioDeviceList;

class RtAAudioDevice : public AudioDevice
{
	enum IOModalities
	{
		eFullDuplex=0,
		eHalfDuplexIn,
		eHalfDuplexOut,
		eNoneYet
	};

public:

	RtAAudioDevice( const std::string& str, int devID );
	~RtAAudioDevice( );

	void Start() throw( ErrRtAudio );
	void Stop() throw ( ErrRtAudio );
	void Read( Audio& samples, const int channelID );
	void Write( const Audio& samples, const  int channelID );
	
	virtual void GetInfo( AudioDevice::TInfo& );
	
protected:

	void SetupMonoOutputStream() throw( ErrRtAudio );
	void SetupStereoOutputStream() throw( ErrRtAudio );
	void SetupMultiOutputStream() throw( ErrRtAudio );
	void SetupMonoInputStream() throw( ErrRtAudio );
	void SetupStereoInputStream() throw( ErrRtAudio );
	void SetupMultiInputStream() throw( ErrRtAudio );
	void SetupStereoFullDuplexStream() throw( ErrRtAudio );

private:

	unsigned                 mNChannelsWritten;
	bool                     mChannelsWritten[256];

	unsigned                 mNChannelsRead;
	bool                     mChannelsRead[256];

	int                      mFramesPerBuffer;

	int                      mDevID;

	short*                   mInputSamples;
	short*                   mOutputSamples;

	int                      mInputStreamId;
	int                      mOutputStreamId;

	IOModalities             mIOModel;
	RtAudio*                 mDevice;

	bool                     mStarted;
};


class RtAAudioDeviceList : public AudioDeviceList
{
private:
	static RtAAudioDeviceList mDevices;
	
	RtAAudioDeviceList();

	std::vector< int > mDevIDs;

protected:

	void EnumerateAvailableDevices() throw ( ErrRtAudio );

public:

	virtual ~RtAAudioDeviceList();

	inline std::string DefaultDevice()
	{
		return mAvailableDevices[0];
	}

	AudioDevice* Create( const std::string& name, const std::string& device );
	
};

}

#endif // RtAAudioDevice.hxx
