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

#include "MonoAudioFileWriter.hxx"
#include "AudioCodecs_Stream.hxx"
#include "FileSystem.hxx"

namespace CLAM
{
	MonoAudioFileWriter::MonoAudioFileWriter()
		: mInput( "Samples Write", this ),
		  mOutStream( NULL )
	{
	}

	MonoAudioFileWriter::MonoAudioFileWriter( const ProcessingConfig& cfg )
		: mInput( "Samples Write", this ),
		  mOutStream( NULL )
	{
		Configure( cfg );
	}

	MonoAudioFileWriter::~MonoAudioFileWriter()
	{
		if ( mOutStream )
			delete mOutStream;
		if ( mConfig.HasTargetFile() )
			FileSystem::GetInstance().UnlockFile( mConfig.GetTargetFile().GetLocation() );

	
	}

	const char* MonoAudioFileWriter::GetClassName() const
	{
		return "MonoAudioFileWriter";
	}
	
	const ProcessingConfig& MonoAudioFileWriter::GetConfig() const
	{
		return mConfig;
	}

	bool MonoAudioFileWriter::Do()
	{
		bool result = Do( mInput.GetAudio() );
		mInput.Consume();
		return result;		
	}
	
	bool MonoAudioFileWriter::Do( const Audio & data )
	{
		mOutStream->WriteData( 0, data.GetBuffer().GetPtr(), data.GetSize() );
		return true;
	}

	bool MonoAudioFileWriter::ConcreteConfigure( const ProcessingConfig& cfg )
	{
		if ( mConfig.HasTargetFile() )
			FileSystem::GetInstance().UnlockFile( mConfig.GetTargetFile().GetLocation() );

		CopyAsConcreteConfig( mConfig, cfg );

		AudioFile& targetFile = mConfig.GetTargetFile();

		if ( targetFile.GetHeader().GetChannels() != 1 ) // this is the 'mono' file writer...
		{
			AddConfigErrorMessage("Too many channels!");
			return false;
		}

		if ( !targetFile.IsWritable() )
		{
			AddConfigErrorMessage("There is an incompatibility between the 'Format', 'Encoding' and 'Endianess'  "
				"configuration parameter values");
			return false;
		}

		if ( FileSystem::GetInstance().IsFileLocked( mConfig.GetTargetFile().GetLocation() ) )
		{
			AddConfigErrorMessage("File: ");
			AddConfigErrorMessage(mConfig.GetTargetFile().GetLocation() );
			AddConfigErrorMessage(" has been locked by another Processing");

			return false;
		}

		FileSystem::GetInstance().LockFile( mConfig.GetTargetFile().GetLocation() );

		mOutStream = targetFile.GetStream();

		if ( !mOutStream )
		{
			AddConfigErrorMessage("Could not get a valid audio file stream!");
			return false;			
		}

		return true;
	}

	bool MonoAudioFileWriter::ConcreteStart()
	{
		mOutStream->PrepareWriting();
		
		return true;
	}

	bool MonoAudioFileWriter::ConcreteStop()
	{
		mOutStream->Dispose();
		delete mOutStream;
		mOutStream = NULL;

		return true;
	}
}
