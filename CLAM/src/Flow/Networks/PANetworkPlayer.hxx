#ifndef _PORTAUDIO_NETWORK_PLAYER_HXX_
#define _PORTAUDIO_NETWORK_PLAYER_HXX_

#include <iostream>
#include <string>
#include "NetworkPlayer.hxx"

#include "ExternGenerator.hxx"
#include "ExternSink.hxx"

#include <portaudio.h>

namespace CLAM
{

int portaudio_process (const void *, void *, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void *);

typedef std::vector<ExternGenerator*> PAOutPortList;
typedef std::vector<ExternSink*> PAInPortList;

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
	
	void InitClient();
	
	//PA stream manipulation methods
	void OpenStream(const Network& net);
	void CloseStream();

	//Buffer copying methods
	void Do(const void *inputBuffers, void *outputBuffers, unsigned long framesPerBuffer);
	void DoInPorts(TData** input, unsigned long nframes);
	void DoOutPorts(TData** output, unsigned long nframes);
	
	virtual void Start();
	virtual void Stop();

	inline void ControlIfPortAudioError(int result);
};

} //end namespace CLAM

#endif
