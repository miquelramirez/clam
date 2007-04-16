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

#ifndef __MULTICHANNELAUDIOFILEWRITERCONFIG__
#define __MULTICHANNELAUDIOFILEWRITERCONFIG__

#include "ProcessingConfig.hxx"
#include "AudioOutFilename.hxx"
#include "DataTypes.hxx"

namespace CLAM
{
	/**
	 *  Configuration class for the CLAM::MultiChannelAudioFileWriter 
	 *  processing object.
	 *
	 *  @see MultiChannelAudioFileWriter
	 */

	class MultiChannelAudioFileWriterConfig
		: public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE
		( MultiChannelAudioFileWriterConfig, 3, ProcessingConfig );
		
		/**
		 *  The file where samples are to be stored
		 */

		DYN_ATTRIBUTE( 0, public, AudioOutFilename, TargetFile );
		DYN_ATTRIBUTE( 1, public, TData, SampleRate );
		DYN_ATTRIBUTE( 2, public, unsigned, NChannels );

	protected:

		void DefaultInit();
	};
}

#endif // MultiChannelAudioFileWriterConfig.hxx

