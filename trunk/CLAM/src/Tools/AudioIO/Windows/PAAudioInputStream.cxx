#include <portaudio.h>
#include "PAAudioInputStream.hxx"
#include "PortAudioUtils.hxx"
#include "Assert.hxx"

namespace CLAM
{

	void PAAudioInputStream::SetupStream()
	{
		PaError errval;

//		CheckConsistency();

		errval = Pa_OpenStream( 
							   &mStream,
							   mConfig.GetDeviceID(),
							   mConfig.GetChannelNumber(),
							   paInt16,
							   NULL,
							   paNoDevice,
							   0,
							   paInt16,
							   NULL,
							   mConfig.GetSampleRate(),
							   mConfig.GetInputDblBuffer()->GetSize()/mConfig.GetChannelNumber(),
							   0,
							   0,
							   mConfig.GetCallback(),
							   mConfig.GetInputDblBuffer() );

		CHECK_PA_ERROR( "Error opening the stream: ", errval );
	}

	void PAAudioInputStream::CheckConsistency()
	{
		const PaDeviceInfo* devnfo = Pa_GetDeviceInfo( mConfig.GetDeviceID() );
		
		if ( devnfo == NULL )
			throw ErrPortAudio("Error opening stream\nThe device id is not valid");

		// MRJ: Let's check if the capabilities of the selected
		// device meet the requirements expressed in the stream
		// configuration object

		// I don't like much this but I don't see clearly the
		// utility of using CLAM_ASSERT's beyond saving me
		// of typing the conditionals... Besides, this could be
		// a recoverable error on some scenarios, such as a 
		// plugin host

		// Check number of channels
	//	if ( mConfig.GetChannelNumber() > devnfo->maxInputChannels )
	//		throw ErrPortAudio("Error opening the stream:\nNumber of input channels requested is not supported by the device\n" );

		// Check the sample rate. Let's determine wether
		// the requested sample rate is supported

		unsigned supportedSr;
		bool isSupported = false;

		for ( int k = devnfo->numSampleRates - 1; k >= 0; k-- )
			{
				supportedSr = (unsigned)devnfo->sampleRates[k];
				if ( mConfig.GetSampleRate() == supportedSr )
					isSupported = true;
			}

		// Requested sample rate is not supported
		if ( !isSupported ) 
			throw ErrPortAudio( "Error opening the stream:\nRequested Sample rate not supported by the device" );

		// These ones are asserts
		CLAM_ASSERT( mConfig.GetDblBuffer()!=NULL, "The double buffer for the stream cannot be nil!" );
		CLAM_ASSERT( mConfig.GetCallback()!=NULL, "The callback for the stream is nil!" );
	}

}
