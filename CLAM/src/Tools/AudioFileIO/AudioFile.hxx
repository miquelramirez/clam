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

#ifndef __AUDIOFILE__
#define __AUDIOFILE__

#include "Component.hxx"
#include "DataTypes.hxx"
#include "Filename.hxx"
#include "AudioFileFormats.hxx"
#include "AudioFileHeader.hxx"
#include "AudioTextDescriptors.hxx"

#include <string>

namespace CLAM
{
	namespace AudioCodecs
	{
		class Codec;       // forward declaration
		class Stream; // forward declaration
	}

	class AudioFile 
		: public Component
	{
	public:

		AudioFile();
		AudioFile( const AudioFile& );
		const AudioFile& operator=( const AudioFile& );
		~AudioFile();

		void                          SetLocation( std::string uri );

		bool                          SetHeader( const AudioFileHeader& );
		const AudioFileHeader&        GetHeader() const;
		
		void                          SetTextDescriptors( const AudioTextDescriptors& );
		AudioTextDescriptors&         GetTextDescriptors();
		const AudioTextDescriptors&   GetTextDescriptors() const;

		const std::string&            GetLocation() const;
		EAudioFileKind                GetKind() const;

		bool                          IsValid() const;
		bool                          IsReadable() const;
		bool                          IsWritable() const;

		AudioCodecs::Stream*   GetStream();
		void                   LoadFrom( Storage& storage);
		void                   StoreOn( Storage& storage ) const;

		const char*            GetClassName() const;

	protected:
		void                   ResolveCodec();
		void                   SetKind( EAudioFileKind newKind );

	protected:
		std::string           mLocation;
		EAudioFileKind        mKind;
		AudioCodecs::Codec*   mActiveCodec;
		AudioFileHeader       mHeaderData;
		AudioTextDescriptors  mTextDescriptors;

	};


// inlines

	inline void AudioFile::SetKind( EAudioFileKind newKind )
	{
		mKind = newKind;
	}

	inline void AudioFile::SetTextDescriptors( const AudioTextDescriptors& txtDesc )
	{
		mTextDescriptors = txtDesc;
	}

	inline AudioTextDescriptors& AudioFile::GetTextDescriptors()
	{
		return mTextDescriptors;
	}

	inline const AudioTextDescriptors& AudioFile::GetTextDescriptors() const
	{
		return mTextDescriptors;
	}
	
}




#endif // AudioFile.hxx
