#include "PAAudioStream.hxx"
#include "PortAudioUtils.hxx"

namespace CLAM
{
	PAAudioStream::PAAudioStream( PAAudioStreamConfig& cfg )
		: mStream( NULL )
	{
		mConfig = cfg;
	}

	PAAudioStream::~PAAudioStream()
	{
		PaError err = Pa_CloseStream( mStream );
		CHECK_PA_ERROR( "Error closing stream: ", err );
	}

	void PAAudioStream::Start( )
	{
		SetupStream();

		PaError err = Pa_StartStream( mStream );
		CHECK_PA_ERROR( "Error starting stream: ", err );
	}

	void PAAudioStream::Stop()
	{
		PaError err = Pa_StopStream( mStream );
		CHECK_PA_ERROR( "Error stopping stream: ", err );
	}
}

