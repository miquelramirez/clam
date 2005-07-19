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

#include "MultiChannelAudioFileReader.hxx"
#include "AudioCodecs_Stream.hxx"
#include "AudioOutPort.hxx"
#include "Factory.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<MultiChannelAudioFileReader> regtMultiChannelAudioFileReader( "MultiChannelAudioFileReader" );
	}
	
	
	MultiChannelAudioFileReader::MultiChannelAudioFileReader()
		: mNativeStream( NULL )
	{
	}

	MultiChannelAudioFileReader::MultiChannelAudioFileReader( const ProcessingConfig& cfg )
		: mNativeStream( NULL )
	{
		Configure( cfg );
	}

	MultiChannelAudioFileReader::~MultiChannelAudioFileReader()
	{
		if ( mNativeStream )
			delete mNativeStream;
		
		DestroyOldOutputs();
	}

	const char* MultiChannelAudioFileReader::GetClassName() const
	{
		return "MultiChannelAudioFileReader";
	}

	bool MultiChannelAudioFileReader::ModifiesPortsAndControlsAtConfiguration()
	{
		return true;
	}

	const ProcessingConfig& MultiChannelAudioFileReader::GetConfig() const
	{
		return mConfig;
	}

	bool MultiChannelAudioFileReader::Do( std::vector<Audio>& outputs )
	{
		typedef std::vector<Audio> OutputVec;

		if ( !AbleToExecute() )
			return false;

		if ( mEOFReached )
			return false;
		
		// Check all outputs sizes
		bool allOutputsSameSize = true;
		
		TSize sizeTmp = 0;

		// MRJ: We have to keep internally references to
		// the Audio objects yield by Flow Control, since
		// the GetData operation not just returns a reference
		// to writable/readable data, but also performs
		// several checks ( as well as advancing reading/writing
		// zones, etc. )
		// TODO: update this code, because GetData doesn't modifies state anymore
		
		sizeTmp = outputs[0].GetSize();	
		
		for( OutputVec::iterator i = outputs.begin();
		     i!= outputs.end(); i++ )
		  {
		    allOutputsSameSize = ( sizeTmp == (*i).GetSize() );
		  }


		CLAM_ASSERT( allOutputsSameSize, "Outputs sizes differ!" );

		// build the samples matrix

		int j = 0;
		for ( OutputVec::iterator i = outputs.begin();
		      i != outputs.end(); i++ )
			mSamplesMatrix[ j++ ] = (*i).GetBuffer().GetPtr();

		// read the data
		
		mEOFReached = mNativeStream->ReadData( mConfig.GetSelectedChannels().GetPtr(),
						       mConfig.GetSelectedChannels().Size(),
						       mSamplesMatrix.GetPtr(),
						       sizeTmp );

		if ( mNativeStream->WasSomethingRead() )
		{
			// Audio 'simple meta-data' setup
			
			for ( OutputVec::iterator i = outputs.begin();
			      i != outputs.end(); i++ )
			{
				(*i).SetSampleRate( mConfig.GetSourceFile().GetHeader().GetSampleRate() );
				(*i).SetBeginTime( mCurrentBeginTime );
			}
			
			
			mDeltaTime = TData(sizeTmp) / mConfig.GetSourceFile().GetHeader().GetSampleRate();
			mCurrentBeginTime += mDeltaTime;
			
		}
		
		return mNativeStream->WasSomethingRead();

		
	}

	bool MultiChannelAudioFileReader::Do()
	{
		if ( !AbleToExecute() )
			return false;

		if ( mEOFReached )
			return false;
		
		// Check all outputs sizes
		bool allOutputsSameSize = true;
		
		TSize sizeTmp = 0;

		// MRJ: We have to keep internally references to
		// the Audio objects yield by Flow Control, since
		// the GetData operation not just returns a reference
		// to writable/readable data, but also performs
		// several checks ( as well as advancing reading/writing
		// zones, etc. )
		// TODO: update this code, because GetData doesn't modifies state anymore
		OutRefsVector outRefs;

		for ( OutputVector::iterator i = mOutputs.begin();
		      i!= mOutputs.end();
		      i++ )
		  outRefs.push_back( &((*i)->GetAudio()) );
		
		
		sizeTmp = outRefs[0]->GetSize();	
		
		for( OutRefsVector::iterator i = outRefs.begin();
		     i!= outRefs.end(); i++ )
		  {
		    allOutputsSameSize = ( sizeTmp == (*i)->GetSize() );
		  }


		CLAM_ASSERT( allOutputsSameSize, "Outputs sizes differ!" );

		// build the samples matrix

		int j = 0;
		for ( OutRefsVector::iterator i = outRefs.begin();
		      i != outRefs.end(); i++ )
			mSamplesMatrix[ j++ ] = (*i)->GetBuffer().GetPtr();

		// read the data
		
		mEOFReached = mNativeStream->ReadData( mConfig.GetSelectedChannels().GetPtr(),
						       mConfig.GetSelectedChannels().Size(),
						       mSamplesMatrix.GetPtr(),
						       sizeTmp );

		if ( mNativeStream->WasSomethingRead() )
		{
			// Audio 'simple meta-data' setup
			
			for ( OutRefsVector::iterator i = outRefs.begin();
			      i != outRefs.end(); i++ )
			{
				(*i)->SetSampleRate( mConfig.GetSourceFile().GetHeader().GetSampleRate() );
				(*i)->SetBeginTime( mCurrentBeginTime );
			}
			
			
			mDeltaTime = TData(sizeTmp) / mConfig.GetSourceFile().GetHeader().GetSampleRate();
			mCurrentBeginTime += mDeltaTime;
			
		}
		
		for ( OutputVector::iterator i = mOutputs.begin();
		      i!= mOutputs.end(); i++ )
		{	
			(*i)->Produce();
		}

		return mNativeStream->WasSomethingRead();
	}

	bool MultiChannelAudioFileReader::ConcreteConfigure( const ProcessingConfig& cfgObj )
	{
		CopyAsConcreteConfig( mConfig, cfgObj );

		if ( !mConfig.HasSourceFile() )			
		{
			AddConfigErrorMessage("No 'source file' was specified in the configuration!");

			return false;
		}

		if ( !mConfig.GetSourceFile().IsReadable() )
		{
			AddConfigErrorMessage("The source file could not be opened!");

			return false;
		}

		if ( !mConfig.HasSelectedChannels() )
		{
			mConfig.AddSelectedChannels();
			mConfig.UpdateData();

			Array< TIndex >& channelsToRead  = mConfig.GetSelectedChannels();

			channelsToRead.Resize( mConfig.GetSourceFile().GetHeader().GetChannels() );
			channelsToRead.SetSize( mConfig.GetSourceFile().GetHeader().GetChannels() );

			DestroyOldOutputs();

			for ( int i = 0; i < channelsToRead.Size(); i++ )
			{
				std::stringstream sstr;
				sstr << i;

				mOutputs.push_back(
					new AudioOutPort( "Channel #" + sstr.str(), this )
					);

				channelsToRead[ i ] = i;
			}

			mSamplesMatrix.Resize( channelsToRead.Size() );
			mSamplesMatrix.SetSize( channelsToRead.Size() );

		}
		else
		{
			// Checking selected channels validity
			Array< TIndex >& channelsToRead =
				mConfig.GetSelectedChannels();

			if ( channelsToRead.Size() != mConfig.GetSourceFile().GetHeader().GetChannels() )
			{
				AddConfigErrorMessage("There are not so many channels in the source file. Check configuration");
				return false;
			}

			int maxChannels = mConfig.GetSourceFile().GetHeader().GetChannels();

			for ( int i = 0; i < channelsToRead.Size(); i++ )
				if ( channelsToRead[i] < 0
				     || channelsToRead[i] >= maxChannels )
				{
					AddConfigErrorMessage("Invalid channel index in configuration!");
					return false;
				}
				     

			DestroyOldOutputs();

			for ( int i = 0; i < channelsToRead.Size(); i++ )
			{
				std::stringstream sstr;
				sstr << channelsToRead[i];

				mOutputs.push_back(
					new AudioOutPort( "Channel #" + sstr.str(), this )
					);
			}

			mSamplesMatrix.Resize( maxChannels );
			mSamplesMatrix.SetSize( maxChannels );
		}

		mNativeStream = mConfig.GetSourceFile().GetStream();

		if (!mNativeStream )
		{
			AddConfigErrorMessage("Could not get a valid audio file stream!");
			return false;
		}

		return true;
	}

	void MultiChannelAudioFileReader::DestroyOldOutputs()
	{
		for( OutputVector::iterator i = mOutputs.begin();
		     i != mOutputs.end(); i++ )
			if( *i ) delete *i;

		mOutputs.clear();
		GetOutPorts().Clear();
	}

	bool MultiChannelAudioFileReader::ConcreteStart()
	{
		mNativeStream->PrepareReading();
		mCurrentBeginTime = 0.0;
		mEOFReached = false;

		return true;
	}

	bool MultiChannelAudioFileReader::ConcreteStop()
	{
		mNativeStream->Dispose();
		delete mNativeStream;
		mNativeStream = NULL;

		return true;
	}
}
