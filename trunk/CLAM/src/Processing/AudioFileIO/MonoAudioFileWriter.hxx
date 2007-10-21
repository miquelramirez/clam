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

#ifndef MonoAudioFileWriter_hxx
#define MonoAudioFileWriter_hxx

#include "Processing.hxx"
#include "MonoAudioFileWriterConfig.hxx"
#include "AudioInPort.hxx"
#include "AudioFile.hxx"

namespace CLAM
{
	namespace AudioCodecs { class Stream;}

	class MonoAudioFileWriter
		: public Processing
	{
	public:
		MonoAudioFileWriter();
		MonoAudioFileWriter( const ProcessingConfig& cfg );

		virtual const char* GetClassName() const;
		virtual const ProcessingConfig& GetConfig() const;
		virtual bool Do();
		virtual bool Do( const CLAM::Audio & data );

		virtual ~MonoAudioFileWriter();

	protected: // methods
		
		virtual bool ConcreteConfigure( const ProcessingConfig& cfgObj );
		virtual bool ConcreteStart();
		virtual bool ConcreteStop();

	protected: // attributes
		
		MonoAudioFileWriterConfig  mConfig;
		AudioInPort 		       mInput;
		AudioCodecs::Stream*       mOutStream;
		AudioFileTarget            mFile;
	};
}

#endif // MonoAudiOFileWriter.hxx

