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

#ifndef __MpegAudioStream__
#define __MpegAudioStream__


#include "AudioCodecs_Stream.hxx"
#include "MpegBitstream.hxx"
#include <deque>
#include <string>

namespace CLAM
{

namespace AudioCodecs
{
	class MpegAudioStream : public Stream
	{
	public:
		MpegAudioStream( const AudioFile& file );
		~MpegAudioStream();

		void PrepareReading();
		void PrepareWriting();
		void Dispose();
		void SeekTo(unsigned long framePosition);

	protected:
		void DiskToMemoryTransfer();
		void MemoryToDiskTransfer();

	private:
		void ConsumeDecodedSamples();

	private:
		std::string   mName;
		FILE*         mpHandle;
		MpegBitstream mBitstream;
		int           mEncodedSampleRate;
		unsigned      _mp3Frame;

		static const TSize                    mMaxDecodedBlockSize;
		std::vector<std::deque<mad_fixed_t> > mDecodeBuffer;
		TSize                                 mSamplesDecoded;
		std::vector<unsigned long> _seekCache;
	};
}

}


#endif // MpegAudioStream.hxx

