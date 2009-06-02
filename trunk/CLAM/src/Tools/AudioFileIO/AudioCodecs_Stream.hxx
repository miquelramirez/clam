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

#ifndef __AUDIOCODECS_STREAM__
#define __AUDIOCODECS_STREAM__

#include "DataTypes.hxx"
#include "DataTypes.hxx"
#include <vector>

namespace CLAM
{

	class AudioFile;

namespace AudioCodecs
{
	/**
	 * Streams provide I/O to and from an audio file of a given format.
	 * You can get a stream by using a Codec.
	 * To add support for a different format you must subclass both
	 * the Codec and the Stream.
	 * @todo Document Stream methods
	 */
	class Stream
	{
	public:
		Stream();
		virtual ~Stream();
		/// Open the stream in read mode
		virtual void PrepareReading()   = 0;
		/// Open the stream in write mode
		virtual void PrepareWriting()   = 0;
		/// Close the stream
		virtual void Dispose()          = 0;
		/// Moves the current position to framePosition.
		/// If such functionality is not supported by the stream it will be ignored.
		virtual void SeekTo(unsigned long framePosition) {}
		/// Returns the current frame position
		virtual unsigned long GetFramePosition() const {return mFramePosition;}
	protected:
		/// Move data from the file to mInterleavedData
		virtual void DiskToMemoryTransfer() = 0;
		/// Move data from mInterleavedData to the file
		virtual void MemoryToDiskTransfer() = 0;

	public:
		/// Read a picked channel
		bool ReadData( int channel, TData* buffer, unsigned nFrames );
		/// Read all the channels
		bool ReadData( TData** buffers, unsigned nFrames );
		/// Read many picked channels
		bool ReadData( int* channels, int nchannels,
		               TData** buffers, unsigned nFrames );

		/// Read a picked channel (Do it has sense?)
		void WriteData( int channel, const TData* buffer, unsigned nFrames );
		/// Read all the channels
		void WriteData( TData** const buffers, unsigned nFrames );
		/// Read many picked channels (Do it has sense?)
		void WriteData( int* channels, int nchannels,
		                TData** const buffers, unsigned nFrames );

	protected:
		void SetChannels( unsigned nChannels );

	protected:
		unsigned            mChannels;
		std::vector<TData>  mInterleavedData;
		bool                mEOFReached;
		unsigned            mFramesLastRead;
		unsigned long       mFramePosition;

	private:
		void AdjustInterleavedBuffer( unsigned nFrames );
	};
}

}

#endif // AudioCodecs_Stream.hxx

