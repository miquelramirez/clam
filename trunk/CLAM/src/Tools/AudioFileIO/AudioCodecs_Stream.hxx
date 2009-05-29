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

		virtual void PrepareReading()   = 0;
		virtual void PrepareWriting()   = 0;
		virtual void Dispose()          = 0;
		
		void DeactivateStrictStreaming();

		bool ReadData( int channel, TData* buffer, unsigned nFrames );
		bool ReadData( TData** buffers, unsigned nFrames );
		bool ReadData( int* channels, int nchannels,
			       TData** buffers, unsigned nFrames );

		void WriteData( int channel, const TData* buffer, unsigned nFrames );
		void WriteData( TData** const buffers, unsigned nFrames );
		void WriteData( int* channels, int nchannels,
				TData** const buffers, unsigned nFrames );

		bool WasSomethingRead() const;

	protected:
		virtual void DiskToMemoryTransfer() = 0;
		virtual void MemoryToDiskTransfer() = 0;

		void SetChannels( unsigned nChannels );

	protected:
		unsigned            mChannels;
		bool                mStrictStreaming;
		std::vector<TData>  mInterleavedData;
		bool                mEOFReached;
		unsigned            mFramesToRead;
		unsigned            mFramesToWrite;
		unsigned            mFramesLastRead;

	private:
		void CheckForFileReading( unsigned samplesToRead );
		void PrepareFileWriting( unsigned samplesToWrite );

		static bool  HandleReAllocation( std::vector<TData>& buffer, unsigned newSize );

	};


	inline bool Stream::WasSomethingRead() const
	{
		return mFramesLastRead != 0;
	}

	// inline methods
	inline void Stream::DeactivateStrictStreaming()
	{
		mStrictStreaming = false;
	}
}

}

#endif // AudioCodecs_Stream.hxx

