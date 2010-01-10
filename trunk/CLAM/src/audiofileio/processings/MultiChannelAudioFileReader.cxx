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
#include "ProcessingFactory.hxx"


namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "MultiChannelAudioFileReader",
		"category", "Audio File I/O",
		"description", "MultiChannelAudioFileReader",
		0
	};
	static FactoryRegistrator<ProcessingFactory, MultiChannelAudioFileReader> reg = metadata;
}
	
	
	MultiChannelAudioFileReader::MultiChannelAudioFileReader()
		: mTimeOutput( "Current Time Position", this)
		, mNativeStream( NULL )
	{
		Configure(MultiChannelAudioFileReaderConfig());
	}

	MultiChannelAudioFileReader::MultiChannelAudioFileReader( const ProcessingConfig& cfg )
		: mTimeOutput( "Current Time Position", this)
		, mNativeStream( NULL )
	{
		Configure( cfg );
	}

	MultiChannelAudioFileReader::~MultiChannelAudioFileReader()
	{
		if ( mNativeStream )
			delete mNativeStream;
		ResizePorts(0);
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

	//TODO remove Do() and Do(vector<Audio>) duplication
	bool MultiChannelAudioFileReader::Do( std::vector<Audio>& outputs )
	{
		typedef std::vector<Audio> OutputVec;

		if ( !AbleToExecute() )
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
			allOutputsSameSize &= ( sizeTmp == i->GetSize() );
		}


		CLAM_ASSERT( allOutputsSameSize, "Outputs sizes differ!" );

		// build the samples matrix
		int j = 0;
		for ( OutputVec::iterator i = outputs.begin(); i != outputs.end(); i++ )
			mSamplesMatrix[ j++ ] = (*i).GetBuffer().GetPtr();

		// read the data
		if ( !mEOFReached && !mIsPaused ) 
		{
			mEOFReached = mNativeStream->ReadData( 
				&mSelectedChannels[0],
				mSelectedChannels.size(),
				&mSamplesMatrix[0],
				sizeTmp );
		}
		else
		{
			if ( mEOFReached ) 
				mCurrentBeginTime = GetHeader().GetLength()/1000;
			for (unsigned i = 0; i != mSamplesMatrix.size(); i++ ) 
				memset ((void *)mSamplesMatrix[i], 0, sizeTmp*sizeof(TData));
		}
		// Audio 'simple meta-data' setup
		for ( OutputVec::iterator i = outputs.begin();
				i != outputs.end(); i++ )
		{
			(*i).SetSampleRate( mAudioFile.GetHeader().GetSampleRate() );
			(*i).SetBeginTime( mCurrentBeginTime );
		}
		
		
		if ( !mEOFReached && !mIsPaused )
		{
		        mDeltaTime = TData(sizeTmp)/mAudioFile.GetHeader().GetSampleRate()*1000;
		        mCurrentBeginTime += mDeltaTime;
		}
		mTimeOutput.SendControl( mCurrentBeginTime );
		
		return !mEOFReached;
	}
	//TODO remove Do() and Do(vector<Audio>) duplication
	bool MultiChannelAudioFileReader::Do()
	{
		if ( !AbleToExecute() )
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

		for ( OutputVector::iterator i = _outputs.begin();
		      i!= _outputs.end();
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
		
		if ( !mEOFReached && !mIsPaused ) 
		{
			mEOFReached = mNativeStream->ReadData(
				&mSelectedChannels[0],
				mSelectedChannels.size(),
				&mSamplesMatrix[0],
				sizeTmp );
		}
		else 
		{
			if ( mEOFReached ) 
				mCurrentBeginTime = GetHeader().GetLength()/1000;
			for (unsigned i = 0; i != mSamplesMatrix.size(); i++) 
			{
				memset ((void *)mSamplesMatrix[i], 0, sizeTmp*sizeof(TData));
		  	}
		}

		// Audio 'simple meta-data' setup
		
		for ( OutRefsVector::iterator i = outRefs.begin();
				i != outRefs.end(); i++ )
		{
			(*i)->SetSampleRate( mAudioFile.GetHeader().GetSampleRate() );
			(*i)->SetBeginTime( mCurrentBeginTime );
		}
		
		if (!mEOFReached && !mIsPaused)
		{
			mDeltaTime = TData(sizeTmp)/mAudioFile.GetHeader().GetSampleRate()*1000;
			mCurrentBeginTime += mDeltaTime;
		}
			
		mTimeOutput.SendControl( mCurrentBeginTime );
		
		for ( OutputVector::iterator i = _outputs.begin();
		      i!= _outputs.end(); i++ )
		{	
			(*i)->Produce();
		}

		return !mEOFReached;
	}

	bool MultiChannelAudioFileReader::ConcreteConfigure( const ProcessingConfig& cfgObj )
	{
		CopyAsConcreteConfig( mConfig, cfgObj );

		if ( !mConfig.HasSourceFile() )
		{
			AddConfigErrorMessage("No 'source file' was specified in the configuration!");
			return false;
		}

		if ( mConfig.GetSourceFile() == "" )
		{
			AddConfigErrorMessage("No file selected");
			return false;
		}

		mAudioFile.OpenExisting(mConfig.GetSourceFile());
		if ( !mAudioFile.IsReadable() )
		{
			AddConfigErrorMessage("The audio file '" + mConfig.GetSourceFile() + "' could not be opened!");
			return false;
		}

		if ( !mConfig.HasSelectedChannels() )
		{
			mSelectedChannels.resize( mAudioFile.GetHeader().GetChannels() );

			ResizePorts(mSelectedChannels.size());

			for (unsigned i = 0; i < mSelectedChannels.size(); i++ )
			{
				mSelectedChannels[ i ] = i;
			}

			mSamplesMatrix.resize( mSelectedChannels.size() );

		}
		else
		{
			// Checking selected channels validity
			const Array<TIndex> & selectedChannels = mConfig.GetSelectedChannels();
			mSelectedChannels.assign(
				selectedChannels.GetPtr(),
				selectedChannels.GetPtr()+selectedChannels.Size());

			if ( mSelectedChannels.size() != mAudioFile.GetHeader().GetChannels() )
			{
				return AddConfigErrorMessage(
					"The configuration asked for more channels than the audio file has.");
			}

			int maxChannels = mAudioFile.GetHeader().GetChannels();

			for (unsigned i = 0; i < mSelectedChannels.size(); i++ )
				if ( mSelectedChannels[i] < 0
				     || mSelectedChannels[i] >= maxChannels )
				{
					return AddConfigErrorMessage(
						"Invalid channel index in configuration!");
				}

			ResizePorts(0);

			for (unsigned i = 0; i < mSelectedChannels.size(); i++ )
			{
				std::stringstream sstr;
				sstr << mSelectedChannels[i];

				_outputs.push_back(
					new AudioOutPort( "Channel #" + sstr.str(), this )
					);
			}

			mSamplesMatrix.resize( maxChannels );
		}

		mNativeStream = mAudioFile.GetStream();

		if (not mNativeStream )
		{
			return AddConfigErrorMessage("Could not get a valid audio file stream!");
		}

		return true;
	}

	bool MultiChannelAudioFileReader::ConcreteStart()
	{
		if (mNativeStream == NULL) 
			mNativeStream = mAudioFile.GetStream();
		mNativeStream->PrepareReading();
		mCurrentBeginTime = 0.0;
		mEOFReached = false;
		mIsPaused = false;

		return true;
	}

	bool MultiChannelAudioFileReader::ConcreteStop()
	{
		mNativeStream->Dispose();
		delete mNativeStream;
		mNativeStream = NULL;

		return true;
	}
	void MultiChannelAudioFileReader::ResizePorts(unsigned nPorts)
	{
		const std::string nameBase = "Channel #";
		for (unsigned i=_outputs.size(); i<nPorts; i++)
		{
			std::ostringstream nameStream;
			nameStream << nameBase << i;
			AudioOutPort * port = new AudioOutPort( nameStream.str(), this);
			_outputs.push_back( port );
		}
		for (unsigned i=nPorts; i<_outputs.size(); i++)
			delete _outputs[i];
		_outputs.resize(nPorts);
/*
		const unsigned portSize = BackendBufferSize();
		for (unsigned i=0; i<_outputs.size(); i++)
		{
			_outputs[i]->SetSize( portSize );
			_outputs[i]->SetHop( portSize );
		}
*/
	}

}

