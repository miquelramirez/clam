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

#ifndef __AUDIOFILE__
#define __AUDIOFILE__

#include <CLAM/ConfigurableFile.hxx>
#include "DataTypes.hxx"
#include "AudioFileFormats.hxx"
#include "AudioFileHeader.hxx"
#include "AudioTextDescriptors.hxx"

namespace CLAM
{
	namespace AudioCodecs
	{
		class Codec;       // forward declaration
		class Stream; // forward declaration
	}

	/** @ingroup AudioFileIO */
	class AudioFile : public ConfigurableFile
	{
	public:

		AudioFile();
		AudioFile( const AudioFile& );
		const AudioFile& operator=( const AudioFile& );
		virtual ~AudioFile();

		const char *GetGroupName() const { return "audio"; }

		const AudioFileHeader&        GetHeader() const;
		
		void                          SetTextDescriptors( const AudioTextDescriptors& );
		AudioTextDescriptors&         GetTextDescriptors();
		const AudioTextDescriptors&   GetTextDescriptors() const;

		EAudioFileKind                GetKind() const;

		bool                          IsValid() const;
		bool                          IsReadable() const;
		bool                          IsWritable() const;

		AudioCodecs::Stream*   GetStream();
		void                   LoadFrom( Storage& storage);
		void                   StoreOn( Storage& storage ) const;

	protected:
		//! Usually the header is set into the file using CreateNew or OpenExistig (public) methods 
		bool SetHeader( const AudioFileHeader& );
		void ResolveCodec();
		void SetKind( EAudioFileKind newKind );

		void ActivateCodec();
		void ResetHeaderData();

	private:
		EAudioFileKind        mKind;
		AudioCodecs::Codec*   mActiveCodec;
		AudioFileHeader       mHeaderData;
		AudioTextDescriptors  mTextDescriptors;

	};

	class AudioFileSource : public AudioFile
	{
	public:
		void OpenExisting(const std::string &location);
		bool GetWriteMode() { return false; }
		const char* GetClassName() const;
		const FileFormatFilterList &GetFormatFilterList() const;
	protected:
		void LocationUpdated();
	};

	class AudioFileTarget : public AudioFile
	{
	public:
		bool CreateNew(const std::string &location,
				const AudioFileHeader &header);
		bool GetWriteMode() { return true; }
		const char* GetClassName() const;
		const FileFormatFilterList &GetFormatFilterList() const;
	protected:
		void LocationUpdated();
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
