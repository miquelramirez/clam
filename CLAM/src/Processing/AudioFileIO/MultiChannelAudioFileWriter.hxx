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

#ifndef __MULTICHANNELAUDIOFILEWRITER__
#define __MULTICHANNELAUDIOFILEWRITER__

#include "MultiChannelAudioFileWriterConfig.hxx"
#include "Processing.hxx"
#include "AudioFile.hxx"
#include <vector>
#include "Array.hxx"

namespace CLAM
{
	class AudioInPort;
	class Audio;

	class MultiChannelAudioFileWriter
		: public Processing
	{
	public:
		MultiChannelAudioFileWriter();
		MultiChannelAudioFileWriter( const ProcessingConfig& cfg );

		~MultiChannelAudioFileWriter();

		virtual const char* GetClassName() const;
		virtual const ProcessingConfig& GetConfig() const;
		virtual bool  Do();
		virtual bool  Do( std::vector<Audio>& inputs );
		bool ModifiesPortsAndControlsAtConfiguration();

	protected: 
		
		virtual bool ConcreteConfigure( const ProcessingConfig& cfgObject );
		virtual bool ConcreteStart();
		virtual bool ConcreteStop();

		void DestroyOldInputs();

	protected:
		typedef std::vector< AudioInPort* > VectorOfInputs;
		typedef std::vector< const Audio* > InputsRefVector;
		
		MultiChannelAudioFileWriterConfig    mConfig;
		VectorOfInputs                       mInputs;
		AudioCodecs::Stream*                 mNativeStream;
		Array<TIndex>                        mChannelsToWrite;
		Array<TData*>                        mSamplesMatrix;
		AudioFileTarget                      mAudioFile;
	};
}

#endif // MultiChannelAudioFileWriter.hxx
