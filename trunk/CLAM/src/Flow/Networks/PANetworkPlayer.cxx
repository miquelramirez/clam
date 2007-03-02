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
{
	InitClient();
}

PANetworkPlayer::~PANetworkPlayer()
{
	CloseStream();

	//Terminate
	Pa_Terminate();
}

void PANetworkPlayer::InitClient()
{
	mPreferredBufferSize=512;
	mSamplingRate=44100;
	mPortAudioStream=NULL;

	ControlIfPortAudioError( Pa_Initialize() );
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
	return true;
}

std::string PANetworkPlayer::NonWorkingReason() const
{
	return "Still not implemented";
}

void PANetworkPlayer::OpenStream(const Network& net)
{
	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		std::string processingType = it->second->GetClassName();
		if ( processingType == "AudioSource" )
		{
			if (false && mSources.size() == 2 )
			{
				std::cout << "WARNING: more than two AudioSources detected, ignoring '" << it->first << "'" << std::endl;
				continue;
			}
				
			//Get Processing address
			mSources.push_back( (AudioSource*)it->second );
		}
		else if ( processingType == "AudioSink" )
		{
			if (false && mSinks.size() == 2 )
			{
				std::cout << "WARNING: more than two AudioSinks detected, ignoring '" << it->first << "'" << std::endl;
				continue;
			}
			//Get Processing address
			mSinks.push_back( (AudioSink*)it->second );
		}
	}

	//Create configuration for input&output and then register the stream
	PaStreamParameters inputParameters;
	inputParameters.device = Pa_GetDefaultInputDevice(); /* default output device */
	CLAM_ASSERT ( inputParameters.device!=paNoDevice, "PortAudio Error: getting default input device");
	inputParameters.channelCount = mSources.size();       /* stereo output */
	inputParameters.sampleFormat = paFloat32 | paNonInterleaved ; /* 32 bit floating point output, having non-interleaved samples*/
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowOutputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	PaStreamParameters * inParams = mSources.size() ? &inputParameters : 0;

	PaStreamParameters outputParameters;
	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	CLAM_ASSERT ( outputParameters.device!=paNoDevice, "PortAudio Error: getting default output device");
	outputParameters.channelCount = mSinks.size();       /* stereo output */
	outputParameters.sampleFormat = paFloat32 | paNonInterleaved ; /* 32 bit floating point output, having non-interleaved samples */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	PaStreamParameters * outParams = mSinks.size() ? &outputParameters : 0;
	
	// TODO: Gracefull error, please
	//CLAM_ASSERT ( inParams || outParams,
	//		"PortAudio Error: no input or output ports defined at all");
	// While gracefull error not here: lets allow PA to start.
	if (!inParams && !outParams)
	{
		outputParameters.channelCount = 1;
		outParams = &outputParameters;
	}
	
	ControlIfPortAudioError(
		Pa_OpenStream(
			&mPortAudioStream,
			inParams,
			outParams,
			double(mSamplingRate),
			mPreferredBufferSize,
			paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			ProcessCallback,
			this )
		);
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

	if ( mPortAudioStream!=NULL )
		ControlIfPortAudioError( Pa_CloseStream( mPortAudioStream ) );
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
	OpenStream( GetNetwork() );
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

void PANetworkPlayer::ControlIfPortAudioError(int result)
{
	//If everything ok, return now
	if( result == paNoError ) return;

	std::ostringstream msg;
	msg <<"PortAudio Error #"<<result<<": "<< Pa_GetErrorText( result )<<std::flush;

	CLAM_ASSERT(false, msg.str().c_str());
}

} //end namespace CLAM

