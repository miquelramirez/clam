/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <portaudio.h>
#include "PAAudioFullDuplexStream.hxx"
#include "PortAudioUtils.hxx"
#include "Assert.hxx"

namespace CLAM
{

	void PAAudioFullDuplexStream::SetupStream()
	{
		PaError errval;

//		CheckConsistency();

		errval = Pa_OpenStream( 
							   &mStream,
							   mConfig.GetDeviceID(),
							   mConfig.GetChannelNumber(),
							   paInt16,
							   NULL,
							   mConfig.GetDeviceID(),
							   mConfig.GetChannelNumber(),
							   paInt16,
							   NULL,
							   mConfig.GetSampleRate(),
							   mConfig.GetDblBuffer()->GetSize()/mConfig.GetChannelNumber(),
							   0,
							   0,
							   mConfig.GetCallback(),
							   mConfig.GetDblBuffer() );

		CHECK_PA_ERROR( "Error opening the stream: ", errval );
	}

	void PAAudioFullDuplexStream::CheckConsistency()
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

