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

#include "MonoAudioFileReader.hxx"
#include "AudioCodecs_Stream.hxx"
#include "Factory.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<MonoAudioFileReader> regtMonoAudioFileReader( "MonoAudioFileReader" );
	}
	
	
	MonoAudioFileReader::MonoAudioFileReader()
		: mOutput( "Samples Read", this  ),
		  mNativeStream( NULL )
	{
		Configure( MonoAudioFileReaderConfig() );
	}

	MonoAudioFileReader::MonoAudioFileReader( const ProcessingConfig& cfg )
		: mOutput( "Samples Read", this ),
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
			AddConfigErrorMessage("The provided config object lacked the field 'SourceFile'");
			return false;
		}

		const std::string & location = mConfig.GetSourceFile();
		if ( location == "")
		{
			AddConfigErrorMessage("No file selected");
			return false;
		}
		mAudioFile.OpenExisting(location);
		// Check that the given file can be opened
		if ( ! mAudioFile.IsReadable() )
		{
			AddConfigErrorMessage("The audio file could not be opened");
			return false;
		}


		if ( mConfig.GetSelectedChannel() < 0
		     || mConfig.GetSelectedChannel() >= mAudioFile.GetHeader().GetChannels() )
		{
			AddConfigErrorMessage("The channel selected for reading does not exist");
			return false;
		}

		mNativeStream = mAudioFile.GetStream();

		mNativeStream->DeactivateStrictStreaming();

		return true;
	}

	bool MonoAudioFileReader::ConcreteStart()
	{
		if ( mNativeStream == NULL )
		{
			mNativeStream = mAudioFile.GetStream();
			mNativeStream->DeactivateStrictStreaming();
		}

		mNativeStream->PrepareReading();
		mCurrentBeginTime = 0.0;
		mEOFReached = false;
		
		return true;
	}

	bool MonoAudioFileReader::ConcreteStop()
	{
		mNativeStream->Dispose();
		delete mNativeStream;
		mNativeStream = NULL;

		return true;
	}
	
	bool MonoAudioFileReader::Do()
	{
		bool result = Do( mOutput.GetAudio() );
		mOutput.Produce();

		return result;
	}


	
	bool MonoAudioFileReader::Do( Audio & outputSamples )		
	{
		if ( !AbleToExecute() )
			return false;

		if ( mEOFReached )
			return false;

		mEOFReached = mNativeStream->ReadData( mConfig.GetSelectedChannel(),
						       outputSamples.GetBuffer().GetPtr(),
						       outputSamples.GetSize() );
		
		outputSamples.SetBeginTime( mCurrentBeginTime );
		mDeltaTime = outputSamples.GetSize() / mAudioFile.GetHeader().GetSampleRate()*1000;
		mCurrentBeginTime += mDeltaTime;
		outputSamples.SetSampleRate( mAudioFile.GetHeader().GetSampleRate() );
		
		if ( ! mEOFReached ) return true;
		if ( ! mConfig.GetLoop() ) return false;

		// Reseting the playback to the begining
		ConcreteStop();
		mNativeStream = mAudioFile.GetStream();
		mNativeStream->DeactivateStrictStreaming();
		ConcreteStart();
		return true;
	}
	
}
