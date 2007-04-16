/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __SoundFileIOError__
#define __SoundFileIOError__

#include "ErrProcessingObj.hxx"

namespace CLAM {

	class ErrSoundFileIO : public ErrProcessingObj
	{
	public:
		ErrSoundFileIO() throw();
		ErrSoundFileIO( const char* msg ) throw();
		ErrSoundFileIO( const ErrSoundFileIO& ) throw();

		virtual ~ErrSoundFileIO() throw();
	};

	class UnavailableSoundFile : public ErrSoundFileIO
	{
	public:
		UnavailableSoundFile() throw();
		UnavailableSoundFile( const char* msg ) throw();
		UnavailableSoundFile( const UnavailableSoundFile& ) throw();

		virtual ~UnavailableSoundFile() throw();
		
	};

	class UnsupportedSoundFileSampleEncoding : public ErrSoundFileIO
	{
	public:
		UnsupportedSoundFileSampleEncoding() throw();
		UnsupportedSoundFileSampleEncoding( const char* msg ) throw();
		UnsupportedSoundFileSampleEncoding( const UnsupportedSoundFileSampleEncoding& ) throw();

		virtual ~UnsupportedSoundFileSampleEncoding() throw();
		
	};
	
	class UnsupportedSoundFileFormat : public ErrSoundFileIO
	{
	public:
		UnsupportedSoundFileFormat() throw();
		UnsupportedSoundFileFormat( const char* msg ) throw();
		UnsupportedSoundFileFormat( const UnsupportedSoundFileFormat& ) throw();

		virtual ~UnsupportedSoundFileFormat() throw();

	};

};//CLAM

#endif

