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

#include "MpegBitstream.hxx"
#include "Assert.hxx"
#include <cstring>
#include <iostream>

namespace CLAM
{

namespace AudioCodecs
{
	// MRJ: This number was extracted from Underbit's own
	// mp3 player code ( mad-0.14.2b2, player.c:86 ). As they
	// say: 40000 bytes is worth 2.5 secs of audio on a stream
	// with 128 kpbs, 1s with 320 kbps
	const int MpegBitstream::mInputBufferSize = 5*8192;
	
	MpegBitstream::MpegBitstream( FILE* bitstream )
		: mpFile( bitstream )
	{
		mInputBuffer = new unsigned char[mInputBufferSize];
	}

	MpegBitstream::MpegBitstream()
	{
		mInputBuffer = new unsigned char[mInputBufferSize];
	}
	
	MpegBitstream::~MpegBitstream()
	{
		if ( mInputBuffer )
			delete [] mInputBuffer;
	}

	void MpegBitstream::Init( FILE* fp )
	{
		mpFile = fp;
		Init();
	}

	void MpegBitstream::Init()
	{
		mad_stream_init( &mBitstream );
		mad_frame_init( &mCurrentFrame );
		mad_synth_init( &mMpegSynth );
		mad_timer_reset( &mStreamTimer );
		mFatalError = false;
	}

	TTime MpegBitstream::Finish()
	{
		mad_synth_finish( &mMpegSynth );
		mad_frame_finish( &mCurrentFrame );
		mad_stream_finish( &mBitstream );

		return (TTime)mad_timer_count( mStreamTimer, MAD_UNITS_MILLISECONDS );
	}

	bool MpegBitstream::EOS()
	{
		if ( feof( mpFile ) ) // no more frames
			return true;
		return false;
				
	}

	bool MpegBitstream::NextFrame()
	{
		bool validFrameFound = false;

		while( !validFrameFound && !FatalError() )
		{
			// the first condition ( mStream.buffer == NULL ) handles the first time we 
			// seek a new Mpeg frame since the stream object does not have a buffer attached.			
			// Last time we tried to decode a frame, there wasn't enough data on the buffer,
			// so we must re-read from the file.

			if ( mBitstream.buffer == NULL 
			     || mBitstream.error == MAD_ERROR_BUFLEN ) 
			{

				TSize readSize, remaining;
				unsigned char*  readStart;
			
				if ( mBitstream.next_frame != NULL )
				{
					remaining = mBitstream.bufend - mBitstream.next_frame;
					memmove( mInputBuffer, mBitstream.next_frame, remaining );
					readStart = mInputBuffer+remaining;
					readSize = mInputBufferSize - remaining;
				}
				else
				{
					readSize = mInputBufferSize;
					readStart = mInputBuffer;
					remaining = 0;
				}
				
				TSize readbytes = fread( readStart, sizeof(unsigned char), readSize, mpFile );
			
				if ( readbytes == 0 ) // Nothing read
					return false;
				else if ( readbytes < readSize ) // Less bytes than expected were read
				{
					CLAM_DEBUG_ASSERT( readStart + readbytes + MAD_BUFFER_GUARD <=
							   mInputBuffer + mInputBufferSize,
							   "Whoops! no room left for buffer guard bytes" );
					unsigned char* startPadding = readStart + readbytes;

					for ( int i = 0; i < MAD_BUFFER_GUARD; i++ )
						startPadding[i] = 0;

					readSize = readbytes + MAD_BUFFER_GUARD;
				}
				else
					readSize = readbytes;

			
				mad_stream_buffer( &mBitstream, mInputBuffer, readSize+remaining );
				mBitstream.error = MAD_ERROR_NONE;
			}
			
			
			if (mad_frame_decode( &mCurrentFrame, &mBitstream )==-1 ) // error
			{
				// some *recoverable* error occured
				if ( !MAD_RECOVERABLE( mBitstream.error ) )
					if ( mBitstream.error != MAD_ERROR_BUFLEN )					
						mFatalError = true;								
			}
			else // frame was decoded right
			{
				// we signal that a good frame has been decoded
				validFrameFound = true;
				// we add this frame duration to the bitstream internal timer
				mad_timer_add( &mStreamTimer, mCurrentFrame.header.duration );
			}


		}

		return validFrameFound;

	}

	bool MpegBitstream::FatalError()
	{
		return mFatalError || ferror(mpFile)!=0;
	}

	bool MpegBitstream::SynthesizeCurrent()
	{
		mad_synth_frame( &mMpegSynth, &mCurrentFrame );

		return true;
	}

	struct mad_frame& MpegBitstream::CurrentFrame()
	{
		return mCurrentFrame;
	}

	struct mad_synth& MpegBitstream::CurrentSynthesis()
	{
		return mMpegSynth;
	}
	
}

}

