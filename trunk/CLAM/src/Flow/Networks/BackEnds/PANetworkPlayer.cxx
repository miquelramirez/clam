
#include "PANetworkPlayer.hxx"
#include "PushFlowControl.hxx"

#include "AudioSink.hxx"
#include "AudioSource.hxx"

#include <pthread.h>


namespace CLAM
{


int PANetworkPlayer::ProcessCallback (
	const void *inputBuffers,
	void *outputBuffers,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData
	)
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

namespace {

void displayPADevices()
{
	int howManiApis = Pa_GetHostApiCount();
	int defaultApi = Pa_GetDefaultHostApi();
	std::cout << "Default API " << defaultApi << std::endl;
	for (int api=0; api<howManiApis; api++)
	{
		const PaHostApiInfo * apiInfo = Pa_GetHostApiInfo( api );
		std::cout
			<< (api==defaultApi?"* ":"  ")
			<< apiInfo->name
			<< " (" << api << ")"
			<< std::endl;
		for (int device=0; device<apiInfo->deviceCount; device++)
		{
			int fullDevice = Pa_HostApiDeviceIndexToDeviceIndex(api, device);
			const PaDeviceInfo * deviceInfo = Pa_GetDeviceInfo(fullDevice);
			std::cout << "\t"
				<< " (" << fullDevice << "/" << device << ") "
				<< (fullDevice == Pa_GetDefaultInputDevice()? "*<": "  ")
				<< (fullDevice == Pa_GetDefaultOutputDevice()? "*>": "  ")
				<< (fullDevice == apiInfo->defaultInputDevice?"*<":"  ")
				<< (fullDevice == apiInfo->defaultOutputDevice?"*>":"  ")
				<< deviceInfo->name
				<< " Inputs: " << deviceInfo->maxInputChannels
				<< " Outputs: " << deviceInfo->maxOutputChannels
				<< std::endl;
		}
	}
}

}

PANetworkPlayer::PANetworkPlayer()
	: mPreferredBufferSize(paFramesPerBufferUnspecified)
	, mSamplingRate(44100)
	, mPortAudioStream(0)
	, mError(paNoError)
{
}

PANetworkPlayer::~PANetworkPlayer()
{
	Stop();
}

void PANetworkPlayer::Start()
{
	if (IsPlaying()) return;
	if (IsPaused())
	{
		BePlaying();
		return;
	}
	if (CheckPaError(Pa_Initialize())) return;
	displayPADevices();

	CacheSourcesAndSinks();

	int nInChannels = GetNSources();
	int nOutChannels = GetNSinks();

	PaHostApiTypeId apiTryList[] = {
		paDirectSound,
		paMME,
		paASIO,
		paSoundManager,
		paCoreAudio,
		paALSA,
		paAL,
		paBeOS,
		paWDMKS,
		paJACK,
//		paWASAPI,
//		paAudioScienceHPI,
		paOSS,
		paInDevelopment
	};

//	int defaultApi = Pa_GetDefaultHostApi();
//	const PaHostApiInfo * apiInfo = Pa_GetHostApiInfo( defaultApi );
	const PaHostApiInfo * apiInfo = 0;
	for (unsigned i=0; apiTryList[i]!=paInDevelopment; i++)
	{
		PaHostApiIndex apiIndex = Pa_HostApiTypeIdToHostApiIndex(apiTryList[i]);
		std::cerr << apiIndex << std::endl;
		if (apiIndex<0) continue;
		apiInfo = Pa_GetHostApiInfo( apiIndex );
		std::cerr << "Portaudio Chosen API: " << apiInfo->name << " " << apiIndex << std::endl;
		break;
	}
	CLAM_ASSERT(apiInfo, "PortAudio: No API available.");

	//Create configuration for input&output and then register the stream
	PaStreamParameters inputParameters;
	PaStreamParameters * inParams = 0;
	if (nInChannels)
	{
		inputParameters.device = apiInfo->defaultInputDevice;
		if ( inputParameters.device == paNoDevice )
		{
			mErrorMessage = "No free default input device";
			std::cerr << "PortAudio Error: " << mErrorMessage << std::endl;
			return;
		}
		const PaDeviceInfo * info = Pa_GetDeviceInfo( inputParameters.device );
		std::cerr << "PortAudio: Chosen Input: " << info->name << std::endl;
		if (nInChannels > Pa_GetDeviceInfo( inputParameters.device )->maxInputChannels)
		{
			mErrorMessage = "Too many input channels for the default device";
			std::cerr << "PortAudio Error: " << mErrorMessage << std::endl;
			return;
		}
		inputParameters.channelCount = nInChannels;
		inputParameters.sampleFormat = paFloat32 | paNonInterleaved ; /* 32 bit floating point output, having non-interleaved samples*/
		inputParameters.suggestedLatency = info->defaultLowOutputLatency;
		inputParameters.hostApiSpecificStreamInfo = NULL;
		inParams = &inputParameters;
	}

	PaStreamParameters outputParameters;
	PaStreamParameters * outParams = 0;
	if (nOutChannels)
	{
		outputParameters.device = apiInfo->defaultOutputDevice;
		if ( outputParameters.device == paNoDevice )
		{
			mErrorMessage = "No free default output device";
			std::cerr << "PortAudio Error: " << mErrorMessage << std::endl;
			return;
		}
		const PaDeviceInfo * info = Pa_GetDeviceInfo( outputParameters.device );
		std::cerr << "PortAudio: Chosen Output: " << info->name << std::endl;
		if (nOutChannels > Pa_GetDeviceInfo( outputParameters.device )->maxOutputChannels)
		{
			mErrorMessage = "Too many output channels for the default device";
			std::cerr << "PortAudio Error: " << mErrorMessage << std::endl;
			return;
		}
		outputParameters.channelCount = nOutChannels;
		outputParameters.sampleFormat = paFloat32 | paNonInterleaved ; /* 32 bit floating point output, having non-interleaved samples */
		outputParameters.suggestedLatency = info->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;
		outParams = &outputParameters;
	}

	CLAM_ASSERT(!mPortAudioStream, "Portaudio: Previous stream not closed");
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
		))
	{
		mErrorMessage = "Audio i/o devices requirements not fullfilled";
		return;
	}

	BePlaying();
	const PaStreamInfo * streamInfo = Pa_GetStreamInfo(mPortAudioStream);
	std::cout << "Sample rate: " << streamInfo->sampleRate << std::endl;
	std::cout << "Input latency: " << streamInfo->inputLatency << std::endl;
	std::cout << "Output latency: " << streamInfo->outputLatency << std::endl;

	mNeedsPriority=true;
	Pa_StartStream( mPortAudioStream );
}

void PANetworkPlayer::Stop()
{
	if (IsStopped()) return;
	if ( mPortAudioStream )
	{
		Pa_StopStream( mPortAudioStream );
		CheckPaError( Pa_CloseStream( mPortAudioStream ) );
		mPortAudioStream=0;
	}
	BeStopped();
	Pa_Terminate();
}

bool PANetworkPlayer::IsWorking()
{
	return mError==paNoError;
}

std::string PANetworkPlayer::NonWorkingReason()
{
	return mErrorMessage;
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

void PANetworkPlayer::Do(const void *inputBuffers, void *outputBuffers,
	unsigned long framesPerBuffer)
{
	if (IsStopped()) return;
	if (mNeedsPriority)
	{
		mNeedsPriority = false;
	#ifdef TODO__was_WIN32
		BOOL res;
		DWORD err;

		res = SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS );
		err = GetLastError();
		res = SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_NORMAL );
		err = GetLastError();
	#else
		struct sched_param sched_param;
		int policy;

		if (pthread_getschedparam(pthread_self(), &policy, &sched_param) < 0)
			std::cerr << "Scheduler getparam failed..." << std::endl;
		sched_param.sched_priority = sched_get_priority_max(SCHED_RR)-1;
		if (!pthread_setschedparam(pthread_self(), SCHED_RR, &sched_param))
			std::cerr << "Scheduler set to Round Robin with priority "<< sched_param.sched_priority << std::endl;
	#endif
	}
	if (IsPaused())
	{
		MuteOutBuffers((float**) outputBuffers, framesPerBuffer);
		return;
	}
	DoInPorts( (float**) inputBuffers, framesPerBuffer);
	DoOutPorts( (float**) outputBuffers, framesPerBuffer);
	GetNetwork().Do();
}

void PANetworkPlayer::DoInPorts(float** input, unsigned long nframes)
{
	for(unsigned i = 0; i < GetNSources(); ++i)
	{
		SetSourceBuffer(i, input[i], nframes);
	}
}

void PANetworkPlayer::DoOutPorts(float** output, unsigned long nframes)
{
	for (unsigned i=0; i<GetNSinks(); ++i)
	{
		SetSinkBuffer(i, output[i], nframes);
	}
}

void PANetworkPlayer::MuteOutBuffers(float** output, unsigned long nframes)
{
	unsigned nSinks = GetNSinks();
	for (unsigned i=0; i<nSinks; i++)
		std::memset(output[i], 0, nframes*sizeof(float));
}

} //end namespace CLAM

