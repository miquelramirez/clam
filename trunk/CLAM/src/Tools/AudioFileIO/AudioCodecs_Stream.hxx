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

	private:
		void CheckForFileReading( TSize samplesToRead );
		void PrepareFileWriting( TSize samplesToWrite );

		static bool  HandleReAllocation( DataArray& buffer, TSize newSize );

	};

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
