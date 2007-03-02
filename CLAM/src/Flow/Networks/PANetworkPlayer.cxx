#include "PANetworkPlayer.hxx"

#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"

namespace CLAM
{


int PANetworkPlayer::ProcessCallback (const void *inputBuffers, void *outputBuffers,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData)
{
	if (statusFlags)
	{
		if (statusFlags & paOutputUnderflow)
			std::cerr << "Portaudio backend: Output Underflow" << std::endl;
		if (statusFlags & paInputUnderflow)
			std::cerr << "Portaudio backend: Input Underflow" << std::endl;
		if (statusFlags & paOutputOverflow)
			std::cerr << "Portaudio backend: Output Overflow" << std::endl;
		if (statusFlags & paInputOverflow)
			std::cerr << "Portaudio backend: Input Overflow" << std::endl;
		if (statusFlags & paPrimingOutput)
			std::cerr << "Portaudio backend: Priming Output" << std::endl;
	}
	PANetworkPlayer* player=(PANetworkPlayer*)userData;
	
	player->Do(inputBuffers, outputBuffers, framesPerBuffer);

	return 0;
}



PANetworkPlayer::PANetworkPlayer()
	: mPreferredBufferSize(512)
	, mSamplingRate(44100)
	, mPortAudioStream(0)
	, mError(paNoError)

{
	InitClient();
}

PANetworkPlayer::~PANetworkPlayer()
{
	CloseStream();
	Pa_Terminate();
}

void PANetworkPlayer::InitClient()
{
	if (CheckPaError(Pa_Initialize())) return;

	int howManiApis = Pa_GetHostApiCount();
	int defaultApi = Pa_GetDefaultHostApi();
	for (int api=0; api<howManiApis; api++)
	{
		const PaHostApiInfo * apiInfo = Pa_GetHostApiInfo( api );
		std::cout 
			<< (api==defaultApi?"  ":"* ")
			<< apiInfo->name 
			<< std::endl;
		for (int device=0; device<apiInfo->deviceCount; device++)
		{
			int fullDevice = Pa_HostApiDeviceIndexToDeviceIndex(api, device);
			const PaDeviceInfo * deviceInfo = Pa_GetDeviceInfo(fullDevice);
			std::cout << "\t"
				<< (device == apiInfo->defaultInputDevice?"*<":"  ")
				<< (device == apiInfo->defaultOutputDevice?"*>":"  ")
				<< deviceInfo->name
				<< " Inputs: " << deviceInfo->maxInputChannels
				<< " Outputs: " << deviceInfo->maxOutputChannels
				<< std::endl;
		}
	}
}

bool PANetworkPlayer::IsWorking() const
{
	return mError==paNoError;
}

std::string PANetworkPlayer::NonWorkingReason() const
{
	return mErrorMessage;
}

void PANetworkPlayer::CollectSourcesAndSinks()
{
	mSources.clear();
	mSinks.clear();
	Network & net = GetNetwork();
	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		std::string processingType = it->second->GetClassName();
		if ( processingType == "AudioSource" )
			mSources.push_back( (AudioSource*)it->second );
		else if ( processingType == "AudioSink" )
			mSinks.push_back( (AudioSink*)it->second );
	}
}

void PANetworkPlayer::OpenStream()
{
	CollectSourcesAndSinks();

	unsigned nInChannels = mSources.size();
	unsigned nOutChannels = mSinks.size();

	//Create configuration for input&output and then register the stream
	PaStreamParameters inputParameters;
	PaStreamParameters * inParams = 0;
	if (nInChannels)
	{
		inputParameters.device = Pa_GetDefaultInputDevice(); /* default output device */
		if ( inputParameters.device == paNoDevice )
		{
			mErrorMessage = "No free default input device";
			std::cerr << "PortAudio Error: " << mErrorMessage << std::endl;
			return;
		}
		inputParameters.channelCount = nInChannels;
		inputParameters.sampleFormat = paFloat32 | paNonInterleaved ; /* 32 bit floating point output, having non-interleaved samples*/
		inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowOutputLatency;
		inputParameters.hostApiSpecificStreamInfo = NULL;
		inParams = &inputParameters;
	}

	PaStreamParameters outputParameters;
	PaStreamParameters * outParams = 0;
	if (nOutChannels)
	{
		outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
		if ( outputParameters.device == paNoDevice )
		{
			mErrorMessage = "No free default output device";
			std::cerr << "PortAudio Error: " << mErrorMessage << std::endl;
			return;
		}
		outputParameters.channelCount = nOutChannels;
		outputParameters.sampleFormat = paFloat32 | paNonInterleaved ; /* 32 bit floating point output, having non-interleaved samples */
		outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;
		outParams = &outputParameters;
	}

	if (CheckPaError(
		Pa_OpenStream(
			&mPortAudioStream,
			inParams,
			outParams,
			double(mSamplingRate),
			mPreferredBufferSize,
			paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			ProcessCallback,
			this )
		)) return;
	const PaStreamInfo * streamInfo = Pa_GetStreamInfo(mPortAudioStream);
	std::cout << "Sample rate: " << streamInfo->sampleRate << std::endl;
	std::cout << "Input latency: " << streamInfo->inputLatency << std::endl;
	std::cout << "Output latency: " << streamInfo->outputLatency << std::endl;

}

void PANetworkPlayer::CloseStream()
{
	Stop();
	
	mSources.clear();
	mSinks.clear();

	if ( mPortAudioStream )
		CheckPaError( Pa_CloseStream( mPortAudioStream ) );
}

void PANetworkPlayer::Do(const void *inputBuffers, void *outputBuffers,
		    unsigned long framesPerBuffer)
{
	DoInPorts( (float**) inputBuffers, framesPerBuffer);
	DoOutPorts( (float**) outputBuffers, framesPerBuffer);
	GetNetwork().Do();
}

void PANetworkPlayer::DoInPorts(float** input, unsigned long nframes)
{
	int i=0;
	for ( AudioSources::iterator it=mSources.begin(); it!=mSources.end(); it++ )
	{
		(*it)->SetExternalBuffer( input[i++], nframes );
	}
}

void PANetworkPlayer::DoOutPorts(float** output, unsigned long nframes)
{
	int i=0;
	for (AudioSinks::iterator it=mSinks.begin(); it!=mSinks.end(); it++)
	{
		(*it)->SetExternalBuffer(output[i++], nframes);
	}
}

void PANetworkPlayer::Start()
{
	if ( !IsStopped() )
		return;

	CloseStream();
	OpenStream();
	SetStopped(false);
	
	//PA CODE (the init order of network, ... should be decided) : activate
	Pa_StartStream( mPortAudioStream );
}

void PANetworkPlayer::Stop()
{
	if ( IsStopped() )
		return;
	
	SetStopped(true);
	
	//PA CODE (the init order of network, ... should be decided) : deactivate
	Pa_StopStream( mPortAudioStream );
}

bool PANetworkPlayer::CheckPaError(PaError result)
{
	mError = result;
	if( result == paNoError ) return false;
	mErrorMessage = Pa_GetErrorText(mError);
	std::cerr 
		<< "PortAudio Error #" << result << ": " 
		<< Pa_GetErrorText( result ) << std::endl;
	return true;
}

} //end namespace CLAM

