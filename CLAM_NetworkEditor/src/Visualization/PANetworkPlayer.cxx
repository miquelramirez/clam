#include "PANetworkPlayer.hxx"

#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"

namespace CLAM
{

//PA CODE
inline int portaudio_process (const void *inputBuffers, void *outputBuffers,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData)
{
	PANetworkPlayer* player=(PANetworkPlayer*)userData;
	player->Do(inputBuffers, outputBuffers, framesPerBuffer);

	return 0;
}

PANetworkPlayer::PANetworkPlayer(const std::string & networkFile)
{
	InitClient();

#ifdef USE_OSC
	SetNetwork( *( new OSCEnabledNetwork() ) );
#else
	SetNetwork( *( new Network() ) );
#endif

	PushFlowControl * fc = new PushFlowControl(mClamBufferSize);
	GetNetwork().AddFlowControl( fc );

	XmlStorage::Restore(GetNetwork(),networkFile);
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
	mClamBufferSize=512;
	mClamFrameRate=44100;
	mPortAudioStream=NULL;

	ControlIfPortAudioError( Pa_Initialize() );
}

void PANetworkPlayer::OpenStream(const Network& net)
{
	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		if ( std::string("ExternGenerator") == std::string(it->second->GetClassName()) )
		{
			//Make sure all frame sizes are the same
			ExternGenerator* gen=(ExternGenerator*)it->second;
			gen->SetFrameAndHopSize( mClamBufferSize );
				
			//Using PortAudio we only accept 2 channels max
			if ( mReceiverList.size() == 2 )
			{
				std::cout << "WARNING: more than two ExternGenerators detected, ignoring '" << it->first << "'" << std::endl;
				continue;
			}
				
			//Get Processing address
			mReceiverList.push_back( (ExternGenerator*)it->second );
		}
		else if ( std::string("ExternSink") == std::string(it->second->GetClassName()) )
		{
			//Make sure all frame sizes are the same
			ExternSink* sink=(ExternSink*)it->second;
			sink->SetFrameAndHopSize(mClamBufferSize);

			//Using PortAudio we only accept 2 channels max
			if ( mSenderList.size() == 2 )
			{
				std::cout << "WARNING: more than two ExternSinks detected, ignoring '" << it->first << "'" << std::endl;
				continue;
			}
			//Get Processing address
			mSenderList.push_back( (ExternSink*)it->second );
		}
	}

	//Create configuration for input&output and then register the stream
	PaStreamParameters inputParameters, outputParameters, *inParams, *outParams;

	inputParameters.device = Pa_GetDefaultInputDevice(); /* default output device */
	CLAM_ASSERT ( inputParameters.device!=paNoDevice, "PortAudio Error: getting default input device");
			
	inputParameters.channelCount = mReceiverList.size();       /* stereo output */
	inputParameters.sampleFormat = paFloat32 | paNonInterleaved ; /* 32 bit floating point output, having non-interleaved samples*/
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowOutputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	CLAM_ASSERT ( outputParameters.device!=paNoDevice, "PortAudio Error: getting default output device");
	
	outputParameters.channelCount = mSenderList.size();       /* stereo output */
	outputParameters.sampleFormat = paFloat32 | paNonInterleaved ; /* 32 bit floating point output, having non-interleaved samples */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	if ( mReceiverList.size() == 0 ) inParams=NULL;
	else inParams=&inputParameters;
	
	if ( mSenderList.size() == 0 ) outParams=NULL;
	else outParams=&outputParameters;

	CLAM_ASSERT ( mReceiverList.size()!=0 && mSenderList.size()!=0,
			"PortAudio Error: no input or output ports defined at all");
	CLAM_ASSERT ( mReceiverList.size()==mSenderList.size(),
			"PortAudio Error: number of input ports must be equal to number of output ports");
	
	ControlIfPortAudioError(
		Pa_OpenStream(
			&mPortAudioStream,
			inParams,
			outParams,
			double(mClamFrameRate),
			mClamBufferSize,			
			paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			portaudio_process,
			this )
		);

}

void PANetworkPlayer::CloseStream()
{
	Stop();
	
	mReceiverList.clear();
	mSenderList.clear();

	if ( mPortAudioStream!=NULL )
		ControlIfPortAudioError( Pa_CloseStream( mPortAudioStream ) );
}

void PANetworkPlayer::Do(const void *inputBuffers, void *outputBuffers,
		    unsigned long framesPerBuffer)
{
	DoInPorts( (TData**) inputBuffers, framesPerBuffer);
	
	//for (int stepcount=0; stepcount < (int(nframes)/int(mClamBufferSize)); stepcount++)
	GetNetwork().Do();

	DoOutPorts( (TData**) outputBuffers, framesPerBuffer);
}

void PANetworkPlayer::DoInPorts(TData** input, unsigned long nframes)
{
	int i=0;
	
	for ( PAOutPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++ )
	{
		//Retrieve PA buffer location
		//Tell the ExternGenerator to put PA's buffer info into CLAM
		(*it)->Do( input[i], nframes );
		i++;

	}
}

void PANetworkPlayer::DoOutPorts(TData** output, unsigned long nframes)
{
	int i=0;
	
	for (PAInPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
	{
		//Retrieve PA buffer location
		//Tell the ExternGenerator to put CLAM's buffer info PA
		(*it)->Do(output[i], nframes);
		i++;
	}
}

void PANetworkPlayer::Start()
{
	if ( !IsStopped() )
		return;

	if ( IsModified() )
	{
		CloseStream();
		OpenStream( GetNetwork() );
	}
	
	SetStopped(false);
	
	GetNetwork().Start();

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
	
	GetNetwork().Stop();
}

void PANetworkPlayer::ControlIfPortAudioError(int result)
{
	//If everything ok, return now
	if( result == paNoError ) return;

	//If there has been an error, inform and quit!
	std::cerr <<"PortAudio Error #"<<result<<": "<< Pa_GetErrorText( result )<<std::endl;

	CLAM_ASSERT(false, "ASSERT!");
	exit(result);
}

} //end namespace CLAM

