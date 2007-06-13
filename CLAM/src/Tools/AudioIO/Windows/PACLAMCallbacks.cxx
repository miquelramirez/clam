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

#include "PACLAMCallbacks.hxx"
#include "DoubleBuffer.hxx"
#include "CLAM_windows.h"
#include "PAAudioStream.hxx"
#undef GetClassName
#include <iostream>

namespace CLAM
{
	int monoOutCallback( const void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 const PaStreamCallbackTimeInfo *timeInfo, 
             PaStreamCallbackFlags statusFlags, void *userData )
	{
		DoubleBuffer* dblBuff = ( DoubleBuffer* ) userData;
		static unsigned short last_time_token = 1;
		static unsigned short time_token = 1;

		WaitForSingleObject( dblBuff->mBackBufferReady, INFINITE );

		short* dst = (short*) outputBuffer;
		short* src = (short*) dblBuff->mBackBuffer;
		unsigned bufflen = dblBuff->mLen;

		// assuming one frame, already converted from float
		// to short

		while ( bufflen-- )
			{
				*dst++ = *src++;
			}

		ResetEvent( dblBuff->mBackBufferReady );

		return 0;
	}
	
	int stereoOutCallback( const void *inputBuffer, void *outputBuffer, 
						   unsigned long framesPerBuffer, 
						   const PaStreamCallbackTimeInfo *timeInfo, 
               PaStreamCallbackFlags statusFlags, void *userData )
	{
		DoubleBuffer* dblBuff = ( DoubleBuffer* ) userData;


		WaitForSingleObject( dblBuff->mBackBufferReady, INFINITE );
	
		short* dst = (short*) outputBuffer;
		short* src = (short*) dblBuff->mBackBuffer;
		

		// assuming two frame, already converted from float
		// to short

		// We unroll the loop 

		unsigned bufflen = 	dblBuff->mLen >> 1; // division by two

		while ( bufflen-- )
			{
				*dst++ = *src++;
				*dst++ = *src++;
			}
		
		ResetEvent( dblBuff->mBackBufferReady );

		return 0;
	}
	
	int multiOutCallback( const void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 const PaStreamCallbackTimeInfo *timeInfo, 
             PaStreamCallbackFlags statusFlags, void *userData )
	{
		DoubleBuffer* dblBuff = ( DoubleBuffer* ) userData;

		WaitForSingleObject( dblBuff->mBackBufferReady, INFINITE );
		
		short* dst = (short*) outputBuffer;
		short* src = (short*) dblBuff->mBackBuffer;
				
		// assuming one frame, already converted from float
		// to short
		// no loop unrolling here :(
		unsigned bufflen = dblBuff->mLen;
		while ( bufflen-- )
		{
			*dst++ = *src++;
		}

		ResetEvent( dblBuff->mBackBufferReady );
		
		return 0;

	}
	
	int monoInCallback( const void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 const PaStreamCallbackTimeInfo *timeInfo, 
             PaStreamCallbackFlags statusFlags, void *userData )
	{
    DoubleBuffer* dblBuff = ((PAAudioStreamConfig*) userData)->GetInputDblBuffer();

		short* src = (short*) inputBuffer;
		short* dst = (short*) dblBuff->mFrontBuffer;

		unsigned bufflen = dblBuff->mLen;

		while ( bufflen-- )
			*dst++ = *src++;

    dblBuff->SwapBuffers();
		
		return 0;
	}

	int stereoInCallback( const void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 const PaStreamCallbackTimeInfo *timeInfo, 
             PaStreamCallbackFlags statusFlags, void *userData )
	{
    DoubleBuffer* dblBuff = ((PAAudioStreamConfig*) userData)->GetInputDblBuffer();

		short* src = (short*) inputBuffer;
		short* dst = (short*) dblBuff->mFrontBuffer;


		unsigned bufflen = dblBuff->mLen >> 1;


		while ( bufflen-- )
			{
				*dst++ = *src++;
				*dst++ = *src++;
			}

		dblBuff->SwapBuffers();
		return 0;
	}

	int multiInCallback( const void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 const PaStreamCallbackTimeInfo *timeInfo, 
             PaStreamCallbackFlags statusFlags, void *userData )
	{
    PAAudioStreamConfig *stream = (PAAudioStreamConfig*) userData;

		short* dst = (short*) stream->GetInputDblBuffer()->mFrontBuffer;
    unsigned long bufferSize = framesPerBuffer;
    bufferSize *= (stream->GetChannelNumber() << 1); // 2 bytes per sample

    memcpy(dst, inputBuffer, bufferSize);
		stream->GetInputDblBuffer()->SwapBuffers();

		return paContinue;
	}

	int stereoFDCallback( const void *inputBuffer, void *outputBuffer, 
					 unsigned long framesPerBuffer, 
					 const PaStreamCallbackTimeInfo *timeInfo, 
           PaStreamCallbackFlags statusFlags, void *userData )
	{

		FullDuplexDoubleBuffer* buff = ( FullDuplexDoubleBuffer*) userData;

		short* src = (short*) inputBuffer;
		short* dst = (short*) buff->mInputDblBuff->mFrontBuffer;


		unsigned bufflen = buff->mInputDblBuff->mLen >> 1;


		while ( bufflen-- )
			{
				*dst++ = *src++;
				*dst++ = *src++;
			}

		buff->mInputDblBuff->SwapBuffers();

		WaitForSingleObject( buff->mOutputDblBuff->mBackBufferReady, INFINITE );

		//buff->mOutputDblBuff->mBackBufferReady = false;

		dst = (short*) outputBuffer;
		src = (short*) buff->mInputDblBuff->mBackBuffer;
		

		// assuming two frame, already converted from float
		// to short

		// We unroll the loop 

		bufflen = 	buff->mOutputDblBuff->mLen >> 1; // division by two

		while ( bufflen-- )
			{
				*dst++ = *src++;
				*dst++ = *src++;
			}

		ResetEvent( buff->mOutputDblBuff->mBackBufferReady );

		return 0;

	}

}

