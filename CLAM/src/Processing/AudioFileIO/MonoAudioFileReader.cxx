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
#include "ProcessingFactory.hxx"


namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "MonoAudioFileReader",
		"category", "Audio File I/O",
		"description", "MonoAudioFileReader",
		0
	};
	static FactoryRegistrator<ProcessingFactory, MonoAudioFileReader> reg = metadata;
}
	
	
	MonoAudioFileReader::MonoAudioFileReader( const ProcessingConfig& cfg )
		: mOutput( "Samples Read", this )
		, mTimeOutput( "Current Time Position", this)
		, mNativeStream( NULL )
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
			AddConfigErrorMessage("The audio file '" + location + "' could not be opened");
			return false;
		}


		if ( mConfig.GetSelectedChannel() < 0
		     || mConfig.GetSelectedChannel() >= mAudioFile.GetHeader().GetChannels() )
		{
			AddConfigErrorMessage("The channel selected for reading does not exist");
			return false;
		}

		mNativeStream = mAudioFile.GetStream();
		return true;
	}

	bool MonoAudioFileReader::ConcreteStart()
	{
		if ( not mNativeStream )
			mNativeStream = mAudioFile.GetStream();

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


	bool MonoAudioFileReader::Do( Audio & output )		
	{
		TData * outputBuffer = output.GetBuffer().GetPtr();
		const unsigned outputSize = output.GetSize();

		if ( not mEOFReached ) 
		{
			mEOFReached = mNativeStream->ReadData(mConfig.GetSelectedChannel(), outputBuffer, outputSize);
		}
		else 
		{
			mCurrentBeginTime = GetHeader().GetLength();// /1000;
			memset ((void *)outputBuffer, 0, outputSize*sizeof(TData));
		}
		output.SetBeginTime( mCurrentBeginTime );

		if ( not mEOFReached )
		{
			TTime deltaTime = output.GetSize() / mAudioFile.GetHeader().GetSampleRate()*1000;
			mCurrentBeginTime += deltaTime;
		}
		mTimeOutput.SendControl( mCurrentBeginTime / 1000 );
		output.SetSampleRate( mAudioFile.GetHeader().GetSampleRate() );
		
		if ( not mEOFReached ) return true; // Still in the middle
		if ( not mConfig.GetLoop() ) return false; // End reached, not looping

		// Looping TODO: This could imply some expensive operations
		ConcreteStop();
		ConcreteStart();
		return true;
	}
}

