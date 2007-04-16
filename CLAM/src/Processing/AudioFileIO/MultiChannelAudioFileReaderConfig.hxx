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

#ifndef __MULTICHANNELAUDIOFILEREADERCONFIG__
#define __MULTICHANNELAUDIOFILEREADERCONFIG__

#include "ProcessingConfig.hxx"
#include "AudioInFilename.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"

namespace CLAM
{
	class MultiChannelAudioFileReaderConfig
		: public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE
		( MultiChannelAudioFileReaderConfig, 2, ProcessingConfig );

		/**
		 *  The file that will be the source of audio samples
		 */

		DYN_ATTRIBUTE( 0, public, AudioInFilename, SourceFile );

		/**
		 *  Tells the processing which n0, n1, ..., nf channels
		 *  to load from the file. Note that an output port will be
		 *  created for each channel index specified in the array.
		 *  
		 *  By default, this attribute is not added, and the processing
		 *  will understand that all the channels present in the file
		 *  must be loaded.
		 */
		DYN_ATTRIBUTE( 1, public, Array<TIndex>, SelectedChannels );

	protected:
		
		void DefaultInit();
	};
}


#endif // MultiChannelAudioFileReaderConfig.hxx

