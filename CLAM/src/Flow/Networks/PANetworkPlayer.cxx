#include "PANetworkPlayer.hxx"

#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"

namespace CLAM
{

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

	SetNetwork( *( new Network() ) );

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
		if ( std::string("AudioSource") == std::string(it->second->GetClassName()) )
		{
			//Make sure all frame sizes are the same
			AudioSource* gen=(AudioSource*)it->second;
			gen->SetFrameAndHopSize( mClamBufferSize );
				
			//Using PortAudio we only accept 2 channels max
			if (false && mReceiverList.size() == 2 )
			{
				std::cout << "WARNING: more than two AudioSources detected, ignoring '" << it->first << "'" << std::endl;
				continue;
			}
				
			//Get Processing address
			mReceiverList.push_back( (AudioSource*)it->second );
		}
		else if ( std::string("AudioSink") == std::string(it->second->GetClassName()) )
		{
			//Make sure all frame sizes are the same
			AudioSink* sink=(AudioSink*)it->second;
			sink->SetFrameAndHopSize(mClamBufferSize);

			//Using PortAudio we only accept 2 channels max
			if (false && mSenderList.size() == 2 )
			{
				std::cout << "WARNING: more than two AudioSinks detected, ignoring '" << it->first << "'" << std::endl;
				continue;
			}
			//Get Processing address
			mSenderList.push_back( (AudioSink*)it->second );
		}
	}

	//Create configuration for input&output and then register the stream
	PaStreamParameters inputParameters;
	inputParameters.device = Pa_GetDefaultInputDevice(); /* default output device */
	CLAM_ASSERT ( inputParameters.device!=paNoDevice, "PortAudio Error: getting default input device");
	inputParameters.channelCount = mReceiverList.size();       /* stereo output */
	inputParameters.sampleFormat = paFloat32 | paNonInterleaved ; /* 32 bit floating point output, having non-interleaved samples*/
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowOutputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	PaStreamParameters * inParams = mReceiverList.size() ? &inputParameters : 0;

	PaStreamParameters outputParameters;
	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	CLAM_ASSERT ( outputParameters.device!=paNoDevice, "PortAudio Error: getting default output device");
	outputParameters.channelCount = mSenderList.size();       /* stereo output */
	outputParameters.sampleFormat = paFloat32 | paNonInterleaved ; /* 32 bit floating point output, having non-interleaved samples */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	PaStreamParameters * outParams = mSenderList.size() ? &outputParameters : 0;
	
	// TODO: Gracefull error, please
	//CLAM_ASSERT ( inParams || outParams,
	//		"PortAudio Error: no input or output ports defined at all");
	// Till not cracefull error: lets allow PA to start.
	if (!inParams && !outParams)
		outParams = &outputParameters;
	
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
	DoInPorts( (float**) inputBuffers, framesPerBuffer);
	
	//for (int stepcount=0; stepcount < (int(nframes)/int(mClamBufferSize)); stepcount++)
	GetNetwork().Do();

	DoOutPorts( (float**) outputBuffers, framesPerBuffer);
}

void PANetworkPlayer::DoInPorts(float** input, unsigned long nframes)
{
	int i=0;
	
	for ( PAOutPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++ )
	{
		(*it)->Do( input[i++], nframes );

	}
}

void PANetworkPlayer::DoOutPorts(float** output, unsigned long nframes)
{
	int i=0;
	for (PAInPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
	{
		(*it)->Do(output[i++], nframes);
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

	std::ostringstream msg;
	msg <<"PortAudio Error #"<<result<<": "<< Pa_GetErrorText( result )<<std::flush;

	CLAM_ASSERT(false, msg.str().c_str());
}

} //end namespace CLAM

