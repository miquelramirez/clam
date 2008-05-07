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
#include "PAAudioInputStream.hxx"
#include "PortAudioUtils.hxx"
#include "Assert.hxx"

#include <iostream>

namespace CLAM
{

	void PAAudioInputStream::SetupStream()
	{
		PaError errval;

		CheckConsistency();

    PaStreamParameters in_stream_params;
    in_stream_params.device = mConfig.GetDeviceID();
    in_stream_params.channelCount = mConfig.GetChannelNumber();
    in_stream_params.sampleFormat = paInt16;
    in_stream_params.suggestedLatency = Pa_GetDeviceInfo(in_stream_params.device)->defaultLowInputLatency;
    in_stream_params.hostApiSpecificStreamInfo = NULL;

		errval = Pa_OpenStream( 
							   &mStream,
							   &in_stream_params,
                 NULL,
							   mConfig.GetSampleRate(),
							   mConfig.GetInputDblBuffer()->GetSize()/mConfig.GetChannelNumber(),
							   0,
							   mConfig.GetCallback(),
							   &mConfig );

		CHECK_PA_ERROR( "Error opening the stream: ", errval );
	}

	void PAAudioInputStream::CheckConsistency() throw (ErrPortAudio)
	{
		const PaDeviceInfo* devnfo = Pa_GetDeviceInfo( mConfig.GetDeviceID() );
		
		if ( devnfo == NULL )
			throw ErrPortAudio("Error opening stream\nThe device id is not valid");

    PaStreamParameters params;
    params.device = mConfig.GetDeviceID();
    params.channelCount = mConfig.GetChannelNumber();
    params.sampleFormat = paInt16;
    params.suggestedLatency = 0;
    params.hostApiSpecificStreamInfo = 0;

    bool isSupported = (Pa_IsFormatSupported(&params, 0, mConfig.GetSampleRate()) ? false : true);

		// Requested sample rate is not supported
		if ( !isSupported ) 
			throw ErrPortAudio( "Error opening the stream:\nRequested Sample rate not supported by the device" );

		// These ones are asserts
		CLAM_ASSERT( mConfig.GetDblBuffer()!=NULL, "The double buffer for the stream cannot be nil!" );
		CLAM_ASSERT( mConfig.GetCallback()!=NULL, "The callback for the stream is nil!" );
	}

}

