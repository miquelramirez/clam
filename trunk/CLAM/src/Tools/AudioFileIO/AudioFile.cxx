/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "AudioFile.hxx"
#include "Assert.hxx"
#include "PCMCodec.hxx"
#include "OggVorbisCodec.hxx"
#include "MpegCodec.hxx"

namespace CLAM
{
	AudioFile::AudioFile()
		: mLocation( "" ), mKind( EAudioFileKind::eUnknown ), mActiveCodec( NULL )
	{
	}

	AudioFile::AudioFile( const AudioFile& obj )
	{
		mLocation = obj.mLocation;
		mActiveCodec = obj.mActiveCodec;
		mKind = obj.mKind;
		mHeaderData = obj.mHeaderData;
		mTextDescriptors = obj.mTextDescriptors;
	}

	const AudioFile& AudioFile::operator=( const AudioFile& obj )
	{
		mLocation = obj.mLocation;
		mActiveCodec = obj.mActiveCodec;
		mKind = obj.mKind;
		mHeaderData = obj.mHeaderData;
		mTextDescriptors = obj.mTextDescriptors;

		return *this;
	}

	AudioFile::~AudioFile()
	{
	}
	void AudioFile::ResolveCodec()
	{
		mActiveCodec = NULL;

		if ( AudioCodecs::Codec::FileExists( mLocation ) )
		{		
			if ( AudioCodecs::PCMCodec::Instantiate().IsReadable( mLocation ) )
			{
				SetKind( EAudioFileKind::ePCM );
				mActiveCodec = & AudioCodecs::PCMCodec::Instantiate();
			}
			else if ( AudioCodecs::OggVorbisCodec::Instantiate().IsReadable( mLocation ) )
			{
				SetKind( EAudioFileKind::eOggVorbis );
				mActiveCodec = & AudioCodecs::OggVorbisCodec::Instantiate();		
				
			}
			else if ( AudioCodecs::MpegCodec::Instantiate().IsReadable( mLocation ) )
			{
				SetKind( EAudioFileKind::eMpeg );
				mActiveCodec = &AudioCodecs::MpegCodec::Instantiate();
			}
			else
				SetKind( EAudioFileKind::eUnknown );		
		}
		else
			SetKind( EAudioFileKind::eUnknown );
	}

	void AudioFile::SetLocation( std::string uri )
	{
		mLocation = uri;
		VerifyLocation();
	}

	void AudioFile::VerifyLocation()
	{
		ResolveCodec();			
		
		if ( mActiveCodec != NULL )
		{
			mActiveCodec->RetrieveHeaderData( mLocation, mHeaderData );
			
			if ( GetKind() == EAudioFileKind::eOggVorbis ||
			     GetKind() == EAudioFileKind::eMpeg )
			{
				mActiveCodec->RetrieveTextDescriptors( mLocation, mTextDescriptors );				
			}
		}
		
		if ( GetKind() == EAudioFileKind::eUnknown )
		{
			mHeaderData.RemoveAll();
			mHeaderData.UpdateData();
		}

	}


	const AudioFileHeader& AudioFile::GetHeader() const
	{
		return mHeaderData;
	}

	const std::string & AudioFile::GetLocation() const
	{
		return mLocation;
	}

	bool AudioFile::SetHeader( const AudioFileHeader& newHeader )
	{
		mHeaderData.AddAll();
		mHeaderData.UpdateData();
		
		if ( !newHeader.HasSampleRate() )
			return false;
		else
			mHeaderData.SetSampleRate( newHeader.GetSampleRate() );

		if ( !newHeader.HasChannels() )
			mHeaderData.SetChannels( 1 );
		else
			mHeaderData.SetChannels( newHeader.GetChannels() );

		if ( !newHeader.HasFormat() )
			return false;
		else
		{
			if ( newHeader.GetFormat() < EAudioFileFormat::eVorbisMk1 )
			{
				mKind = EAudioFileKind::ePCM;
				mActiveCodec = &AudioCodecs::PCMCodec::Instantiate();
			}
			else if ( newHeader.GetFormat() == EAudioFileFormat::eVorbisMk1 )
			{
				mKind = EAudioFileKind::eOggVorbis;
				mActiveCodec= &AudioCodecs::OggVorbisCodec::Instantiate();
			}
			else
				CLAM_ASSERT( false, "The Enum contained and invalid value!" );
		}

		if ( !newHeader.HasFormat() )
		{
			if ( GetKind() == EAudioFileKind::ePCM )
			{
				if ( mHeaderData.GetFormat() == EAudioFileFormat::eWAV )
					mHeaderData.SetEncoding( EAudioFileEncoding::ePCM_24 );
				else if ( mHeaderData.GetFormat() == EAudioFileFormat::eAIFF )
					mHeaderData.SetEncoding( EAudioFileEncoding::ePCM_24 );
				else
					return false;
			}
			else if ( GetKind() == EAudioFileKind::eOggVorbis )
			{
				mHeaderData.SetEncoding( EAudioFileEncoding::eDefault );
			}
			else
				return false;
		}
		else
			mHeaderData.SetFormat( newHeader.GetFormat() );

		if ( !newHeader.HasEndianess() )
			mHeaderData.SetEndianess( EAudioFileEndianess::eDefault );
		else
			mHeaderData.SetEndianess( newHeader.GetEndianess() );
		
		return true;
	}

	EAudioFileKind AudioFile::GetKind() const
	{
		return mKind;
	}

	bool AudioFile::IsValid() const
	{
		return mActiveCodec == NULL;
	}

	bool AudioFile::IsReadable() const
	{
		if (mActiveCodec == NULL )
			return false;
		
		return mActiveCodec->IsReadable(mLocation);
	}

	bool AudioFile::IsWritable() const
	{
		if ( mActiveCodec == NULL )
			return false;

		return mActiveCodec->IsWritable(mLocation, GetHeader() );
	}

	AudioCodecs::Stream*  AudioFile::GetStream()
	{
		if ( !mActiveCodec )
			return NULL;
		
		return mActiveCodec->GetStreamFor( *this );
	}

	const char* AudioFile::GetClassName() const
	{
		return "AudioFile";
	}

	void AudioFile::LoadFrom( Storage& storage )
	{

		CLAM::XMLAdapter< Filename > xmlLocation( mLocation, "URI", true );
		storage.Load( xmlLocation );
		VerifyLocation();
		
		CLAM::XMLComponentAdapter xmlHeader( mHeaderData, "Header", true );
		storage.Load( xmlHeader );

		CLAM::XMLComponentAdapter xmlTxtDescriptors( mTextDescriptors, "TextualDescriptors", true );
		storage.Load( xmlTxtDescriptors );
	}

	void AudioFile::StoreOn( Storage& storage ) const
	{
		CLAM::XMLAdapter< Filename > xmlLocation( mLocation, "URI", true );
		storage.Store( xmlLocation );

		CLAM::XMLComponentAdapter xmlHeader( mHeaderData, "Header", true );
		storage.Store( xmlHeader );
		

		CLAM::XMLComponentAdapter xmlTxtDescriptors( mTextDescriptors, "TextualDescriptors", true );
		storage.Store( xmlTxtDescriptors );
	}

}
