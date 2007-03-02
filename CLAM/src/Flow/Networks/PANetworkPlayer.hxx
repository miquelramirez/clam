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

typedef std::vector<AudioSource*> AudioSources;
typedef std::vector<AudioSink*> AudioSinks;

class PANetworkPlayer : public NetworkPlayer
{
	int mPreferredBufferSize;
	int mSamplingRate;

	AudioSources mSources;
	AudioSinks mSinks;
	
	PaStream * mPortAudioStream;
	PaError mError;
	std::string mErrorMessage;

public:
	//When created in the prototyper
	PANetworkPlayer(const std::string & networkFile);
	//When created in neteditor
	PANetworkPlayer();
	virtual ~PANetworkPlayer();

	// base class (virtual) interface:
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
	void CollectSourcesAndSinks();
	inline bool CheckPaError(PaError result);
	//Buffer copying methods
	void Do(const void *inputBuffers, void *outputBuffers, unsigned long framesPerBuffer);
	void DoInPorts(float** input, unsigned long nframes);
	void DoOutPorts(float** output, unsigned long nframes);
	//PA stream manipulation methods
	void OpenStream();
	void CloseStream();
	void InitClient();
};

} //end namespace CLAM

#endif
