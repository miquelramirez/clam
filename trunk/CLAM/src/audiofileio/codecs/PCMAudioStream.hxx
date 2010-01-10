/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __PCMAudioStream__
#define __PCMAudioStream__

#if USE_SNDFILE != 1
#error USE_SNDFILE was not set to 1 in your settings.cfg file, but you are including files that require this. Please fix your settings.cfg
#endif

#include "AudioCodecs_Stream.hxx"
#include <sndfile.h>
#include <string>

namespace CLAM
{

namespace AudioCodecs
{

	class PCMAudioStream : public Stream
	{
	public:
		PCMAudioStream( const AudioFile& file );
		~PCMAudioStream();

		void PrepareReading();
		void PrepareWriting();
		void Dispose();
		void SeekTo(long unsigned framePosition);

	protected:
		void DiskToMemoryTransfer();
		void MemoryToDiskTransfer();

	private:
		SNDFILE*            mFileHandle;
		SF_INFO             mNativeFileParams;
		std::string         mName;
	};

}

}
#endif // PCMAudioStream.hxx

