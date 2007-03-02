#ifndef _PORTAUDIO_NETWORK_PLAYER_HXX_
#define _PORTAUDIO_NETWORK_PLAYER_HXX_

#include <iostream>
#include <string>
#include "NetworkPlayer.hxx"

#include "AudioSource.hxx"
#include "AudioSink.hxx"

#include <portaudio.h>

namespace CLAM
{

typedef std::vector<AudioSource*> PAOutPortList;
typedef std::vector<AudioSink*> PAInPortList;

class PANetworkPlayer : public NetworkPlayer
{
	int mClamBufferSize, mClamFrameRate;

	PAOutPortList mReceiverList;
	PAInPortList mSenderList;
	
	//PA CODE : declare client (stream)
	PaStream * mPortAudioStream;

public:
	//When created in the prototyper
	PANetworkPlayer(const std::string & networkFile);
	//When created in neteditor
	PANetworkPlayer();
	virtual ~PANetworkPlayer();

	bool IsWorking() const;
	std::string NonWorkingReason() const;
	
	virtual bool IsCallbackBased() const { return true; }
	virtual void Start();
	virtual void Stop();
	
private:
	static int ProcessCallback (
							const void *inputBuffers,
							void *outputBuffers,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);
	inline void ControlIfPortAudioError(int result);
	//Buffer copying methods
	void Do(const void *inputBuffers, void *outputBuffers, unsigned long framesPerBuffer);
	void DoInPorts(float** input, unsigned long nframes);
	void DoOutPorts(float** output, unsigned long nframes);
	//PA stream manipulation methods
	void OpenStream(const Network& net);
	void CloseStream();
	void InitClient();
};

} //end namespace CLAM

#endif
