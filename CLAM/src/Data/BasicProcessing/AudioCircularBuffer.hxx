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

#ifndef	_AudioCircularBuffer_
#define _AudioCircularBuffer_

#include "Array.hxx"
#include "Audio.hxx"
#include "DataTypes.hxx"
#include "Err.hxx"
#include "CircularBuffer.hxx"

namespace CLAM {

	/** Specialization of the CircularBuffer class for the case of Audio chunks
	 *	@see CircularBuffer
	 */
	class AudioCircularBuffer:public CircularBuffer<TData>
	{
	protected:
		
	public:
 
		/** CONSTRUCTION 
		*/
		AudioCircularBuffer():CircularBuffer<TData>(){}
		
		AudioCircularBuffer(TSize bufferSize)
			:CircularBuffer<TData>(bufferSize)
		{
		
		}

		void ReadAudio(Audio& in)
		{
			CircularBuffer<TData>::Read(in.GetBuffer());
		}

		void NonCopyReadAudio(Audio& in)
		{
			CircularBuffer<TData>::NonCopyRead(in.GetBuffer());
		}
	
		void WriteAudio(const Audio& in)
		{
			CircularBuffer<TData>::Write(in.GetBuffer());
		}

		void AddAudio(const Audio& in)
		{
			CircularBuffer<TData>::Add(in.GetBuffer());
		}

		

	};
}; //end of namespace

#endif //_AudioCircularBuffer_

