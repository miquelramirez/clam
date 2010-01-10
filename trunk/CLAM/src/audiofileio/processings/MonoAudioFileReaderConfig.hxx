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

#ifndef __MONOAUDIOFILEREADERCONFIG__
#define __MONOAUDIOFILEREADERCONFIG__
#include <typeinfo>

#include "ProcessingConfig.hxx" // For CLAM::ProcessingConfig interface
#include "AudioInFilename.hxx" // For CLAM::AudioInFilename interface
#include "DataTypes.hxx" // For common CLAM types

namespace CLAM
{
	/**
	 *  Configuration class for the CLAM::MonoAudioFileReader processing
	 *  object.
	 *
	 *  @see MonoAudioFileReader
	 */
	class MonoAudioFileReaderConfig 
		: public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( MonoAudioFileReaderConfig, 3, ProcessingConfig );

		/**
		 *  The file that will be the source of audio samples
		 */
		DYN_ATTRIBUTE( 0, public, AudioInFilename, SourceFile );
		
		/**
		 *  Tells the MonoAudioFileReader processing to extract
		 *  the channel with the given index. Note that if the provided
		 *  index is out of the range of channels available for the
		 *  source file processing configuration will fail.
		 *
		 *  By default this value is set to zero.
		 */
		DYN_ATTRIBUTE( 1, public, TIndex, SelectedChannel );
		DYN_ATTRIBUTE( 2, public, bool, Loop );
		
		~MonoAudioFileReaderConfig();
	protected:
		void DefaultInit();
	};
	
}

#endif // MonoAudioFileReaderConfig.hxx

