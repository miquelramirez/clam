/*
 * Copyright (c) 2009 Hernán Ordiales
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

#include "DelayWetDry.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "DelayWetDry",
		"category", "Plugins",
		"description", "Delay Wet/Dry",
		0
	};
	static FactoryRegistrator<ProcessingFactory, DelayWetDry> reg = metadata;
}

bool DelayWetDry::Do(const Audio& in, Audio& out)
{
	mBuffer.DelayInitParams( in.GetSampleRate(), in.GetSize(), mConfig.GetMaxDelay() ); //checks buffer configuration
	mBuffer.DelayAdjustment( int(mDelay.GetLastValue()) );

	//Reading audio from the input (buffering)
	mBuffer.WriteAudio(in);

	DataArray& inb = in.GetBuffer();
	DataArray& outb = out.GetBuffer();

	float Dry = mWetDry.GetLastValue();
	float Wet = 1. - Dry;

	//Writing delayed audio in the output
	if (mBuffer.ReadyToPlay())
	{
		mBuffer.ReadAudio(out); //means there is enough data in the buffer to start playing
		for (TSize i=0;i<in.GetSize();i++) outb[i] = outb[i]*Wet + inb[i]*Dry;
	}
	else
	{
		for (TSize i=0;i<in.GetSize();i++) outb[i] = 0.; //zero as output
	}

	return true;
}

}
