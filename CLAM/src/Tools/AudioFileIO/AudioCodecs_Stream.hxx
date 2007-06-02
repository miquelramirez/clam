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
#include "Array.hxx"
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

		virtual void SetFOI( const AudioFile& ) = 0;
		virtual void PrepareReading()   = 0;
		virtual void PrepareWriting()   = 0;
		virtual void PrepareReadWrite() = 0;
		virtual void Dispose()          = 0;
		
		void DeactivateStrictStreaming();
		void ActivateStrictStreaming();
		bool StrictStreaming() const;

		bool ReadData( int channel, TData* ptr, TSize howmany );
		bool ReadData( int* channels, int nchannels,
			       TData** samples, TSize howmany );

		void WriteData( int channel, const TData* ptr, TSize howmany );
		void WriteData( int* channels, int nchannels,
				TData** const samples, TSize howmany );

		bool WasSomethingRead() const;
		


	protected:
		virtual void DiskToMemoryTransfer() = 0;
		virtual void MemoryToDiskTransfer() = 0;

		bool AllChannelsConsumed();
		void ResetConsumedChannels();
		void MarkAllChannelsAsConsumed();

		bool AllChannelsProduced();
		void ResetProducedChannels();
		void MarkAllChannelsAsProduced();

		void SetChannels( TSize nChannels );

	protected:
		TSize               mChannels;
		std::vector<bool>   mChannelsConsumed;
		std::vector<bool>   mChannelsProduced;
		bool                mStrictStreaming;
		DataArray           mInterleavedData;
		DataArray           mInterleavedDataOut;
		bool                mEOFReached;
		TSize               mFramesToRead;
		TSize               mFramesToWrite;
		TSize               mFramesLastRead;

	private:
		void CheckForFileReading( TSize samplesToRead );
		void PrepareFileWriting( TSize samplesToWrite );

		static bool  HandleReAllocation( DataArray& buffer, TSize newSize );

	};


	inline bool Stream::WasSomethingRead() const
	{
		return mFramesLastRead != 0;
	}

	// inline methods
	inline void Stream::ActivateStrictStreaming()
	{
		mStrictStreaming = true;
	}

	inline void Stream::DeactivateStrictStreaming()
	{
		mStrictStreaming = false;
	}

	inline bool Stream::StrictStreaming() const
	{
		return mStrictStreaming;
	}

}

}

#endif // AudioCodecs_Stream.hxx

