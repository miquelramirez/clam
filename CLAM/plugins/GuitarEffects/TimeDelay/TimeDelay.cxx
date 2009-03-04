/*
 * Copyright (c) 2009 Hernán Ordiales <audiocode@uint8.com.ar>
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

#include "TimeDelay.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "TimeDelay",
		"category", "Plugins",
		"description", "Time Delay ",
		0
	};
	static FactoryRegistrator<ProcessingFactory, TimeDelay> reg = metadata;
}

bool TimeDelay::ConcreteConfigure(const ProcessingConfig& config) {

	CopyAsConcreteConfig( mConfig, config );

	TSize maxDelay = mConfig.GetMaxDelay();

	mDelay.SetBounds(0,maxDelay);
	mDelay.SetDefaultValue(0);
	mDelay.DoControl(0);

	mBuffer.DelayInitParams(44100,512,maxDelay);
	mBuffer.SetDelay( int(mDelay.GetLastValue())  );

	return true;
}

bool TimeDelay::Do(const Audio& in, Audio& out)
{
	mBuffer.DelayInitParams( in.GetSampleRate(), in.GetSize(), mConfig.GetMaxDelay() ); //checks buffer configuration
	mBuffer.DelayAdjustment( int(mDelay.GetLastValue()) );

	//Reading audio from the input (buffering)
	mBuffer.WriteAudio(in);

	//Writing delayed audio in the output
	if (mBuffer.ReadyToPlay())
		mBuffer.ReadAudio(out); //means there is enough data in the buffer to start playing
	else
	{
		DataArray& outb = out.GetBuffer();
		for (TSize i=0;i<in.GetSize();i++) outb[i] = 0.; //zero as output
	}
	return true;
}

}
