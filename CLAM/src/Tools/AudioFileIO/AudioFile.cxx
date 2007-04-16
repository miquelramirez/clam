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

#include "AudioFile.hxx"
#include "Assert.hxx"
#include "PCMCodec.hxx"
#if USE_OGGVORBIS == 1
#  include "OggVorbisCodec.hxx"
#endif

#if USE_MAD == 1
#  include "MpegCodec.hxx"
#endif

namespace CLAM
{
	AudioFile::AudioFile()
		: mKind( EAudioFileKind::eUnknown )
		, mActiveCodec( NULL )
	{
	}

	AudioFile::AudioFile( const AudioFile& obj ) : ConfigurableFile(obj)
	{
		mActiveCodec = obj.mActiveCodec;
		mKind = obj.mKind;
		mHeaderData = obj.mHeaderData;
		mTextDescriptors = obj.mTextDescriptors;
	}

	const AudioFile& AudioFile::operator=( const AudioFile& obj )
	{
		ConfigurableFile::operator=(obj);
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

		const std::string &location = GetLocation();
		if ( !AudioCodecs::Codec::FileExists( location ) )
		{		
			SetKind( EAudioFileKind::eUnknown );
			return;
		}

		if ( AudioCodecs::PCMCodec::Instantiate().IsReadable( location ) )
		{
			SetKind( EAudioFileKind::ePCM );
			mActiveCodec = &AudioCodecs::PCMCodec::Instantiate();
		}
#if USE_OGGVORBIS == 1
		else if ( AudioCodecs::OggVorbisCodec::Instantiate().IsReadable( location ) )
		{
			SetKind( EAudioFileKind::eOggVorbis );
			mActiveCodec = &AudioCodecs::OggVorbisCodec::Instantiate();		
		}
#endif		
#if USE_MAD == 1
		else if ( AudioCodecs::MpegCodec::Instantiate().IsReadable( location ) )
		{
			SetKind( EAudioFileKind::eMpeg );
			mActiveCodec = &AudioCodecs::MpegCodec::Instantiate();
		}
#endif
		else
			SetKind( EAudioFileKind::eUnknown );		
	}

	const AudioFileHeader& AudioFile::GetHeader() const
	{
		return mHeaderData;
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

		if ( newHeader.GetFormat() < EAudioFileFormat::eVorbisMk1 )
		{
			mKind = EAudioFileKind::ePCM;
			mActiveCodec = &AudioCodecs::PCMCodec::Instantiate();
		}
		else if ( newHeader.GetFormat() == EAudioFileFormat::eVorbisMk1 )
		{
			mKind = EAudioFileKind::eOggVorbis;
#if USE_OGGVORBIS==1		
			mActiveCodec= &AudioCodecs::OggVorbisCodec::Instantiate();
#endif		
		}
		else
			CLAM_ASSERT( false, "The Enum contained and invalid value!" );

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
		
		return mActiveCodec->IsReadable(GetLocation());
	}

	bool AudioFile::IsWritable() const
	{
		if ( mActiveCodec == NULL )
			return false;

		return mActiveCodec->IsWritable(GetLocation(), GetHeader() );
	}

	AudioCodecs::Stream*  AudioFile::GetStream()
	{
		if ( !mActiveCodec )
			return NULL;
		
		return mActiveCodec->GetStreamFor( *this );
	}

	void AudioFile::LoadFrom( Storage& storage )
	{
		ConfigurableFile::LoadFrom(storage);

		CLAM::XMLComponentAdapter xmlHeader( mHeaderData, "Header", true );
		storage.Load( xmlHeader );

		CLAM::XMLComponentAdapter xmlTxtDescriptors( mTextDescriptors, "TextualDescriptors", true );
		storage.Load( xmlTxtDescriptors );
	}

	void AudioFile::StoreOn( Storage& storage ) const
	{
		ConfigurableFile::StoreOn(storage);

		CLAM::XMLComponentAdapter xmlHeader( mHeaderData, "Header", true );
		storage.Store( xmlHeader );
		

		CLAM::XMLComponentAdapter xmlTxtDescriptors( mTextDescriptors, "TextualDescriptors", true );
		storage.Store( xmlTxtDescriptors );
	}

void AudioFile::ActivateCodec()
{
	if ( NULL == mActiveCodec )
		return;

	mActiveCodec->RetrieveHeaderData( GetLocation(), mHeaderData );
	
	if ( GetKind() == EAudioFileKind::eOggVorbis ||
	     GetKind() == EAudioFileKind::eMpeg )
	{
		mActiveCodec->RetrieveTextDescriptors( GetLocation(), mTextDescriptors );				
	}
}
	
void AudioFile::ResetHeaderData()
{
	mHeaderData.RemoveAll();
	mHeaderData.UpdateData();
}

/* =============================================================== */

void AudioFileSource::OpenExisting(const std::string &location)
{
	SetLocation(location);
}

const char* AudioFileSource::GetClassName() const
{
	return "AudioFileSource";
}
void AudioFileSource::LocationUpdated()
{
	ResolveCodec();			
	ActivateCodec();
	if ( GetKind() == EAudioFileKind::eUnknown )
		ResetHeaderData();
}

const FileFormatFilterList &AudioFileSource::GetFormatFilterList() const
{
	return EAudioFileFormat::ReadableFormats();
}

/* =============================================================== */

bool AudioFileTarget::CreateNew(const std::string &location,
		const AudioFileHeader &header)
{
	SetLocation(location);
	return SetHeader(header);
}

const char* AudioFileTarget::GetClassName() const
{
	return "AudioFileTarget";
}
void AudioFileTarget::LocationUpdated()
{
	EAudioFileFormat format(
			EAudioFileFormat::FormatFromFilename(GetLocation()));
	AudioFileHeader header;
	header.SetValues(44100, 1, format);
	SetHeader(header);
}
const FileFormatFilterList &AudioFileTarget::GetFormatFilterList() const
{
	return EAudioFileFormat::WritableFormats();
}

}

