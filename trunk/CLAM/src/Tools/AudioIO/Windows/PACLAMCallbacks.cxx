#include "PACLAMCallbacks.hxx"
#include "DoubleBuffer.hxx"
#include "CLAM_windows.h"
#undef GetClassName
#include <iostream>

namespace CLAM
{
	int monoOutCallback( void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 PaTimestamp outTime, void *userData )
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
	
	int stereoOutCallback( void *inputBuffer, void *outputBuffer, 
						   unsigned long framesPerBuffer, 
						   PaTimestamp outTime, void *userData )
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
	
	int multiOutCallback( void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						  PaTimestamp outTime, void *userData )
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
	
	int monoInCallback( void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 PaTimestamp outTime, void *userData )
	{
		DoubleBuffer* dblBuff = (DoubleBuffer*) userData;

		short* src = (short*) inputBuffer;
		short* dst = (short*) dblBuff->mFrontBuffer;

		unsigned bufflen = dblBuff->mLen;

		while ( bufflen-- )
			*dst++ = *src++;

		dblBuff->SwapBuffers();
		
		return 0;
	}

	int stereoInCallback( void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 PaTimestamp outTime, void *userData )
	{
		DoubleBuffer* dblBuff = (DoubleBuffer*) userData;

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

	int multiInCallback( void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 PaTimestamp outTime, void *userData )
	{
		DoubleBuffer* dblBuff = (DoubleBuffer*) userData;

		short* src = (short*) inputBuffer;
		short* dst = (short*) dblBuff->mFrontBuffer;

		unsigned bufflen = dblBuff->mLen;


		while ( framesPerBuffer-- )
			{
				*dst++ = *src++;
			}

		dblBuff->SwapBuffers();
		
		return 0;
		
	}

	int stereoFDCallback(void *inputBuffer, void *outputBuffer, 
					 unsigned long framesPerBuffer, 
					 PaTimestamp outTime, void *userData )
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

		buff->mOutputDblBuff->mBackBufferReady = false;

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
