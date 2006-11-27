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

#include "MultiChannelAudioFileWriter.hxx"
#include "AudioCodecs_Stream.hxx"
#include "FileSystem.hxx"
#include "AudioInPort.hxx"
#include "Audio.hxx"
#include "Factory.hxx"
#include "XMLStorage.hxx" // TODO: jsut for testing


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{

	namespace detail
	{
		static ProcessingFactory::Registrator<MultiChannelAudioFileWriter> regtMultiChannelAudioFileWriter( "MultiChannelAudioFileWriter" );
	}
	
	
	MultiChannelAudioFileWriter::MultiChannelAudioFileWriter()
		: mNativeStream( NULL )
	{
		Configure(MultiChannelAudioFileWriterConfig());
	}

	MultiChannelAudioFileWriter::MultiChannelAudioFileWriter( const ProcessingConfig& cfg )
		: mNativeStream( NULL )
	{
		Configure( cfg );
	}

	MultiChannelAudioFileWriter::~MultiChannelAudioFileWriter()
	{
		if ( mNativeStream )
			delete mNativeStream;

		DestroyOldInputs();

		if ( mConfig.HasTargetFile() )
			FileSystem::GetInstance().UnlockFile( mConfig.GetTargetFile().GetLocation() );

	}

	const char* MultiChannelAudioFileWriter::GetClassName() const
	{
		return "MultiChannelAudioFileWriter";
	}

	bool MultiChannelAudioFileWriter::ModifiesPortsAndControlsAtConfiguration()
	{
		return true;
	}

	const ProcessingConfig& MultiChannelAudioFileWriter::GetConfig() const
	{
		return mConfig;
	}

	bool MultiChannelAudioFileWriter::Do( std::vector<Audio>& inputs )
	{
		typedef std::vector<Audio> InputVec;

		if ( !AbleToExecute() )
			return false;

		// Checking that all inputs have the same size
		bool  allInputsSameSize = true;
		TSize inputsSize = 0;

		inputsSize = inputs[0].GetSize();

		for ( InputVec::iterator i = inputs.begin();
		      i!= inputs.end() && allInputsSameSize;
		      i++ )
		  {
		    allInputsSameSize = ( inputsSize == (*i).GetSize() );
		  }


		CLAM_ASSERT( allInputsSameSize, "Input sizes differ!" );

		// Now, let's build the samples matrix

		int j = 0;

		for( InputVec::iterator i = inputs.begin();
		     i != inputs.end(); i++ )
		  mSamplesMatrix[ j++ ] = (*i).GetBuffer().GetPtr();

		mNativeStream->WriteData( mChannelsToWrite.GetPtr(), mChannelsToWrite.Size(),
					 mSamplesMatrix.GetPtr(), inputsSize );

		return true;

	}

	bool MultiChannelAudioFileWriter::Do()
	{
		if ( !AbleToExecute() )
			return false;

		// Checking that all inputs have the same size
		bool  allInputsSameSize = true;
		TSize inputsSize = 0;

		InputsRefVector inputsRef;

		for ( VectorOfInputs::iterator i = mInputs.begin();
		      i!= mInputs.end(); i++ )
		  {
		    inputsRef.push_back( &((*i)->GetAudio()) );
		  }

		inputsSize = inputsRef[0]->GetSize();

		for ( InputsRefVector::iterator i = inputsRef.begin();
		      i!= inputsRef.end() && allInputsSameSize;
		      i++ )
		  {
		    allInputsSameSize = ( inputsSize == (*i)->GetSize() );
		  }


		CLAM_ASSERT( allInputsSameSize, "Input sizes differ!" );

		// Now, let's build the samples matrix

		int j = 0;

		for( InputsRefVector::iterator i = inputsRef.begin();
		     i != inputsRef.end(); i++ )
		  mSamplesMatrix[ j++ ] = (*i)->GetBuffer().GetPtr();

		mNativeStream->WriteData( mChannelsToWrite.GetPtr(), mChannelsToWrite.Size(),
					 mSamplesMatrix.GetPtr(), inputsSize );

		for( VectorOfInputs::iterator i = mInputs.begin();
		     i!=mInputs.end(); i++ )
		  {
		    (*i)->Consume();
		  }

		return true;
	}

	bool MultiChannelAudioFileWriter::ConcreteConfigure( const ProcessingConfig& cfg )
	{
		if ( mConfig.HasTargetFile() )
			FileSystem::GetInstance().UnlockFile( mConfig.GetTargetFile().GetLocation() );

		CopyAsConcreteConfig( mConfig, cfg );

		AudioFile& targetFile = mConfig.GetTargetFile();
		const std::string & location =targetFile.GetLocation();
		if (location=="")
		{
			AddConfigErrorMessage("No file selected");
			return false;
		}

		unsigned nChannels = mConfig.HasNChannels()? mConfig.GetNChannels() : 1;
		if ( nChannels < 1 )
		{
			AddConfigErrorMessage("Channels should be, at least, 1.");
			return false;
		}
		AudioFileHeader header = targetFile.GetHeader();
		if (!header.HasFormat())
		{
			header.AddFormat();
			header.UpdateData();
			header.SetFormat(CLAM::EAudioFileFormat::FormatFromFilename(location) );
		}
		if (!header.HasChannels())
		{
			header.AddChannels();
			header.UpdateData();
		}
		header.SetChannels(nChannels);
		targetFile.CreateNew(location,header);
		if ( !targetFile.IsWritable() )
		{
			AddConfigErrorMessage("The format does not support such number of channels, endiannes or sampling rate.");

			return false;
		}

		if ( FileSystem::GetInstance().IsFileLocked( mConfig.GetTargetFile().GetLocation() ) )
		{
			AddConfigErrorMessage("The file '");
			AddConfigErrorMessage( mConfig.GetTargetFile().GetLocation() );
			AddConfigErrorMessage("' has been locked by another Processing");

			return false;
		}

		FileSystem::GetInstance().LockFile( mConfig.GetTargetFile().GetLocation() );


		if ( !mInputs.empty() )
			DestroyOldInputs();

		mChannelsToWrite.Resize( nChannels );
		mChannelsToWrite.SetSize( nChannels );
		mSamplesMatrix.Resize( nChannels );
		mSamplesMatrix.SetSize( nChannels );

		for ( int i = 0; i < nChannels; i++ )
		{
			mChannelsToWrite[ i ] = i;
			mSamplesMatrix[ i ] = NULL;
			std::stringstream sstr;
			sstr << i;
			
			mInputs.push_back( 
				new AudioInPort( "Channel #" + sstr.str(), this) );
		}

		mNativeStream = targetFile.GetStream();

		if ( !mNativeStream )
		{
			AddConfigErrorMessage("Could not acquire an audio file stream!");
			return false;
		}

		return true;
	}

	bool MultiChannelAudioFileWriter::ConcreteStart()
	{
		if (mNativeStream == NULL )
			mNativeStream = mConfig.GetTargetFile().GetStream();
		mNativeStream->PrepareWriting();

		return true;
	}

	bool MultiChannelAudioFileWriter::ConcreteStop()
	{
		mNativeStream->Dispose();
		delete mNativeStream;
		mNativeStream = NULL;

		return true;
	}

	void MultiChannelAudioFileWriter::DestroyOldInputs()
	{
		for ( VectorOfInputs::iterator i = mInputs.begin();
		      i != mInputs.end(); i++ )
		{
			if ( *i ) delete *i;
		}

		mInputs.clear();
		GetInPorts().Clear();
	}
}
