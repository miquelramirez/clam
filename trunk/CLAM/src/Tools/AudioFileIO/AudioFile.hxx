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

#include "DataTypes.hxx"
#include "AudioFileFormats.hxx"
#include "AudioFileHeader.hxx"
#include "AudioTextDescriptors.hxx"
#include "Filename.hxx"

namespace CLAM
{
	namespace AudioCodecs
	{
		class Codec;       // forward declaration
		class Stream; // forward declaration
	}

	/** @ingroup AudioFileIO */
	class AudioFile
	{
	public:

		AudioFile();
		AudioFile( const AudioFile& );
		const AudioFile& operator=( const AudioFile& );
		virtual ~AudioFile();

		const Filename & GetLocation() const { return mLocation; }
		const AudioFileHeader & GetHeader() const { return mHeaderData; }
		const AudioTextDescriptors &   GetTextDescriptors() const { return mTextDescriptors; }
		AudioTextDescriptors & GetTextDescriptors() { return mTextDescriptors; }

		void SetTextDescriptors( const AudioTextDescriptors & descriptors ) { mTextDescriptors=descriptors; }

		EAudioFileKind                GetKind() const;

		bool                          IsValid() const;
		bool                          IsReadable() const;
		bool                          IsWritable() const;

		AudioCodecs::Stream*   GetStream();

	protected:
		//! Usually the header is set into the file using CreateNew or OpenExistig (public) methods 
		bool SetHeader( const AudioFileHeader& );
		void ResolveCodec();

		void ActivateCodec();
		void ResetHeaderData();

		Filename mLocation;
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
	};

	class AudioFileTarget : public AudioFile
	{
	public:
		bool CreateNew(const std::string &location,
				const AudioFileHeader &header);
	};


}




#endif // AudioFile.hxx

