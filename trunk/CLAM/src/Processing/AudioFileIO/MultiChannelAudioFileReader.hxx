/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __MULTICHANNELAUDIOFILEREADER__
#define __MULTICHANNELAUDIOFILEREADER__

#include "Processing.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"
#include "Audio.hxx"
#include "AudioOutPortTmpl.hxx"
#include <vector>

namespace CLAM
{
	class MultiChannelAudioFileReader 
		: public Processing
	{
	public:
		MultiChannelAudioFileReader();
		MultiChannelAudioFileReader( const ProcessingConfig& cfg );

		~MultiChannelAudioFileReader();

		virtual const char* GetClassName() const;
		virtual const ProcessingConfig& GetConfig() const;
		virtual bool Do();
		bool ModifiesPortsAndControlsAtConfiguration();
	
	protected: // methods
		
		virtual bool ConcreteConfigure( const ProcessingConfig& cfgObject );
		virtual bool ConcreteStart();
		virtual bool ConcreteStop();
		
		void DestroyOldOutputs();
		
	protected: // attributes

		typedef std::vector< OutPortTmpl<Audio>* >  OutputVector;
	  typedef std::vector< Audio* > OutRefsVector;

		MultiChannelAudioFileReaderConfig   mConfig;
		OutputVector                        mOutputs;
		Array< TData* >                     mSamplesMatrix;
		AudioCodecs::Stream*                mNativeStream;
		TTime                               mCurrentBeginTime;
		TTime                               mDeltaTime;
		bool                                mEOFReached;
	};
}

#endif // MultiChannelAudioFileReader.hxx
