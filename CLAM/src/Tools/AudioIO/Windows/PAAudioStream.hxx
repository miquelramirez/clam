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

#ifndef __PAAUDIOSTREAM__
#define __PAAUDIOSTREAM__

#include <portaudio.h>
#include "PortAudioUtils.hxx"
#include "DoubleBuffer.hxx"

namespace CLAM
{

class PAAudioStreamConfig
{
public:

	PAAudioStreamConfig()
		: mSampleRate( 44100 ), mNChannels(2), 
		  mCallback( NULL )
	{
	}

	~PAAudioStreamConfig()
	{
	}

	inline void SetSampleRate( unsigned srate )
	{
		mSampleRate = srate;
	}

	inline unsigned GetSampleRate() const
	{
		return mSampleRate;
	}

	inline void SetChannelNumber( unsigned char nchann )
	{
		mNChannels = nchann;
	}

	inline unsigned char GetChannelNumber() const
	{
		return mNChannels;
	}

	inline void SetCallback( PortAudioCallback  cb )
	{
		mCallback = cb;
	}

	inline PortAudioCallback GetCallback( ) const
	{
		return mCallback;
	}

	inline void SetInputDblBuffer( DoubleBuffer* buffer )
	{
		mBuffer.mInputDblBuff = buffer;
	}

	inline DoubleBuffer* GetInputDblBuffer()
	{
		return mBuffer.mInputDblBuff;
	}

	inline void SetOutputDblBuffer( DoubleBuffer* buffer )
	{
		mBuffer.mOutputDblBuff = buffer;
	}

	inline DoubleBuffer* GetOutputDblBuffer()
	{
		return mBuffer.mOutputDblBuff;
	}

	inline FullDuplexDoubleBuffer* GetDblBuffer()
	{
		return &mBuffer;
	}

	inline void SetDeviceID( PaDeviceIndex devID )
	{
		mDevID = devID;
	}

	inline PaDeviceIndex GetDeviceID( ) const
	{
		return mDevID;
	}

private:
	
	unsigned                 mSampleRate;
	unsigned char            mNChannels;
	PaDeviceIndex               mDevID;
	PortAudioCallback        mCallback;
	FullDuplexDoubleBuffer   mBuffer;

};

class PAAudioStream
{
public:

	PAAudioStream( PAAudioStreamConfig& );

	void Start();

	void Stop();

	~PAAudioStream();

protected:

	virtual void SetupStream() = 0;

	
	PAAudioStreamConfig  mConfig;
	PaStream*     mStream;
};

}

#endif

