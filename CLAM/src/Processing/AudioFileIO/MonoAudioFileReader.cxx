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

#include "MonoAudioFileReader.hxx"
#include "AudioCodecs_Stream.hxx"

namespace CLAM
{
	MonoAudioFileReader::MonoAudioFileReader()
		: mOutput( "Samples read", this, 1 ),
		  mNativeStream( NULL )
	{
		
	}

	MonoAudioFileReader::MonoAudioFileReader( const ProcessingConfig& cfg )
		: mOutput( "Samples read", this, 1 ),
		  mNativeStream( NULL )
	{
		Configure( cfg );
	}

	MonoAudioFileReader::~MonoAudioFileReader()
	{
		if ( mNativeStream )
			delete mNativeStream;
	}

	const char* MonoAudioFileReader::GetClassName() const
	{
		return "MonoAudioFileReader";
	}

	const ProcessingConfig& MonoAudioFileReader::GetConfig() const
	{
		return mConfig;
	}

	bool MonoAudioFileReader::ConcreteConfigure( const ProcessingConfig& cfgObject )
	{
		CopyAsConcreteConfig( mConfig, cfgObject );

		if ( !mConfig.HasSourceFile() )
		{
			mStatus = "The provided config object lacked the field ";
			mStatus += "'SourceFile'";

			return false;
		}

		// Check that the given file can be opened
		if ( ! mConfig.GetSourceFile().IsReadable() )
		{
			mStatus = "The Source file could not be opened";
			return false;
		}


		if ( mConfig.GetSelectedChannel() < 0
		     || mConfig.GetSelectedChannel() >= mConfig.GetSourceFile().GetHeader().GetChannels() )
		{
			mStatus = "The channel selected for reading does not exist";
			return false;
		}


		mNativeStream = mConfig.GetSourceFile().GetStream();

		if ( !mNativeStream ) // For some reason a stream could not be acquired
		{
			mStatus = "Could not get a valid audio file stream!";
			return false;
		}

		mNativeStream->DeactivateStrictStreaming();

		return true;
	}

	bool MonoAudioFileReader::ConcreteStart()
	{
		mNativeStream->PrepareReading();
		mCurrentBeginTime = 0.0;
		mEOFReached = false;
		
		return true;
	}

	bool MonoAudioFileReader::ConcreteStop()
	{
		mNativeStream->Dispose();
		
		return true;
	}

	bool MonoAudioFileReader::Do()
	{
		if ( !AbleToExecute() )
			return false;

		if ( mEOFReached )
			return false;

		Audio& outputSamples = mOutput.GetData();

		mEOFReached = mNativeStream->ReadData( mConfig.GetSelectedChannel(),
						      outputSamples.GetBuffer().GetPtr(),
						      outputSamples.GetSize() );

		outputSamples.SetBeginTime( mCurrentBeginTime );
		mDeltaTime = outputSamples.GetSize() / mConfig.GetSourceFile().GetHeader().GetSampleRate();
		mCurrentBeginTime += mDeltaTime;
		outputSamples.SetSampleRate( mConfig.GetSourceFile().GetHeader().GetSampleRate() );

		mOutput.LeaveData();
		return true;
	}
	
}
