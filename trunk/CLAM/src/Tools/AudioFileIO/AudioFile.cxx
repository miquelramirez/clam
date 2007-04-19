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

		const std::string &location = mLocation;
		if ( !AudioCodecs::Codec::FileExists( location ) )
		{		
			mKind = EAudioFileKind::eUnknown;
			return;
		}

		if ( AudioCodecs::PCMCodec::Instantiate().IsReadable( location ) )
		{
			mKind = EAudioFileKind::ePCM;
			mActiveCodec = &AudioCodecs::PCMCodec::Instantiate();
		}
#if USE_OGGVORBIS == 1
		else if ( AudioCodecs::OggVorbisCodec::Instantiate().IsReadable( location ) )
		{
			mKind = EAudioFileKind::eOggVorbis;
			mActiveCodec = &AudioCodecs::OggVorbisCodec::Instantiate();		
		}
#endif		
#if USE_MAD == 1
		else if ( AudioCodecs::MpegCodec::Instantiate().IsReadable( location ) )
		{
			mKind = EAudioFileKind::eMpeg;
			mActiveCodec = &AudioCodecs::MpegCodec::Instantiate();
		}
#endif
		else
			mKind = EAudioFileKind::eUnknown;		
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
			if ( mKind == EAudioFileKind::ePCM )
			{
				if ( mHeaderData.GetFormat() == EAudioFileFormat::eWAV )
					mHeaderData.SetEncoding( EAudioFileEncoding::ePCM_24 );
				else if ( mHeaderData.GetFormat() == EAudioFileFormat::eAIFF )
					mHeaderData.SetEncoding( EAudioFileEncoding::ePCM_24 );
				else
					return false;
			}
			else if ( mKind == EAudioFileKind::eOggVorbis )
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
		return mActiveCodec && mActiveCodec->IsReadable(mLocation);
	}

	bool AudioFile::IsWritable() const
	{
		return mActiveCodec && mActiveCodec->IsWritable(mLocation, mHeaderData );
	}

	AudioCodecs::Stream*  AudioFile::GetStream()
	{
		if ( !mActiveCodec ) return 0;
		return mActiveCodec->GetStreamFor( *this );
	}

	void AudioFile::ActivateCodec()
	{
		if (!mActiveCodec) return;
		mActiveCodec->RetrieveHeaderData( mLocation, mHeaderData );
		mActiveCodec->RetrieveTextDescriptors( mLocation, mTextDescriptors );				
	}
		
	void AudioFile::ResetHeaderData()
	{
		mHeaderData.RemoveAll();
		mHeaderData.UpdateData();
	}

	/* =============================================================== */

	void AudioFileSource::OpenExisting(const std::string &location)
	{
		mLocation = location;
		ResolveCodec();
		ActivateCodec();
		if ( mKind == EAudioFileKind::eUnknown )
			ResetHeaderData();
	}

	/* =============================================================== */

	bool AudioFileTarget::CreateNew(const std::string &location,
			const AudioFileHeader &header)
	{
		mLocation = location;
		return SetHeader(header);
	}

}

