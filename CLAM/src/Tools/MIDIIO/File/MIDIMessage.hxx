/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 * UNIVERSITAT POMPEU FABRA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * MIDIFileReader C++ classes
 * This code is part of the CLAM library, but also usable stand-alone.
 * Maarten de Boer <mdeboer@iua.upf.es>
 *
 */
#ifndef __MIDIMessage__
#define __MIDIMessage__

#include "MIDIDataTypes.hxx"

namespace MIDI
{

	class Message
	/* an ordinary MIDI message contains between 2 and 4 bytes. reserve
	** space for the maximum, 4. */
	{
	friend class Event;
	public:
		Message()
		{
			mData.mStatus = 0;
			mData.mData1 = 0;
			mData.mData2 = 0;
			mData.mData3 = 0;
		}

		Message(Byte status,Byte data1,Byte data2 = 0,Byte data3 = 0)
		{
			mData.mStatus = status;
			mData.mData1 = data1;
			mData.mData2 = data2;
			mData.mData3 = data3;
		}

		Byte operator [] (int i) const { return mVal[i]; }

	private:
		union
		{
			struct
			{
				Byte mStatus;
				Byte mData1;
				Byte mData2;
				Byte mData3;
			} mData;

			Byte mVal[4];
		};
	};

}

#endif

