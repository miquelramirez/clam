/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _STREAM_BUFFER_H
#error DO NOT include this file directly. It is included from StreamBuffer.hxx
#endif

#ifndef _AUDIO_STREAM_BUFFER_H
#define _AUDIO_STREAM_BUFFER_H

#include "Audio.hxx"

namespace CLAM {

	template<class B>
	class StreamBuffer<Audio,B> : public StreamBuffer<TData,B> {

		TData mSampleRate;

	public:

		StreamBuffer(unsigned int max_window_size = 0)
			: StreamBuffer<TData,B>(max_window_size),
			  mSampleRate(0.0)
		{}

		StreamBuffer(const StreamBuffer& b)
			: StreamBuffer<TData,B>(b),
			  mSampleRate(0.0)
		{}

		void SetSampleRate(TData sr) {  mSampleRate = sr; }

		void SetPrototype(const Audio& proto) {  mSampleRate = proto.GetSampleRate(); }

		template<class REGION>
		void GetAndActivate(REGION* r, Audio &a)
		{
			a.SetSampleRate(mSampleRate);
			StreamBuffer<TData,B>::GetAndActivate(r,a.GetBuffer());
		}

	};

}

#endif
