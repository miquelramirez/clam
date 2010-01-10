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

#ifndef __MpegBitstream__
#define __MpegBitstream__

#if USE_MAD != 1
#error USE_MAD was not set to 1 in your settings.cfg file, but you are including files that require this. Please fix your settings.cfg
#endif

#include <mad.h>
#include <cstdio>
#include "DataTypes.hxx"

namespace CLAM
{

namespace AudioCodecs
{
	class MpegBitstream
	{
		struct mad_stream  mBitstream;
		struct mad_frame   mCurrentFrame;
		struct mad_synth   mMpegSynth;
		mad_timer_t        mStreamTimer;

		unsigned char*     mInputBuffer;
		static const int   mInputBufferSize;
		unsigned long      mBufferFileOffset; ///< The offset of the buffer in the file

		bool               mFatalError;
		FILE*              mpFile;

	public:
		MpegBitstream();
		~MpegBitstream();

		void   Init();
		void   Init(FILE*);
		TTime  Finish();
		bool   EOS();
		bool   FatalError();
		bool   NextFrame();
		void   SynthesizeCurrent();
		bool   EnsureEnoughBufferData();
		unsigned long CurrentFrameFileOffset() const;
		struct mad_frame& CurrentFrame();
		struct mad_synth&  CurrentSynthesis();
		struct mad_stream& StreamState();
	};

	inline struct mad_stream& MpegBitstream::StreamState()
	{
		return mBitstream;
	}
}
		

}


#endif // MpegBitstream.hxx

