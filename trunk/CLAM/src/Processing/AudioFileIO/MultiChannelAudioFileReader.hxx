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

#ifndef MultiChannelAudioFileReader_hxx
#define MultiChannelAudioFileReader_hxx

#include "Processing.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"
#include "AudioFile.hxx"
#include "Audio.hxx"
#include "OutControl.hxx"
#include <vector>

namespace CLAM
{

	class AudioOutPort;
	
	class MultiChannelAudioFileReader : public Processing
	{
	public:
		MultiChannelAudioFileReader();
		MultiChannelAudioFileReader( const ProcessingConfig& cfg );

		~MultiChannelAudioFileReader();

		virtual const char* GetClassName() const;
		virtual const ProcessingConfig& GetConfig() const;
		virtual bool Do();
		virtual bool Do( std::vector<Audio>& outputs );
		bool ModifiesPortsAndControlsAtConfiguration();
		const Array<TIndex> & GetSelectedChannels() const {return mSelectedChannels;}
		const AudioFileHeader & GetHeader() const { return mAudioFile.GetHeader(); }
		const AudioTextDescriptors & GetTextDescriptors () const { return mAudioFile.GetTextDescriptors(); }
		const EAudioFileCodec GetCodec() const { return mAudioFile.GetCodec(); }
		
	        void Pause() { mIsPaused = (mIsPaused)?false:true; }; //TODO change to Qt boolean interface (pau)
	
	protected: // methods
		
		virtual bool ConcreteConfigure( const ProcessingConfig& cfgObject );
		virtual bool ConcreteStart();
		virtual bool ConcreteStop();
		
		void DestroyOldOutputs();
		
	protected: // attributes

		typedef std::vector< AudioOutPort* >  OutputVector;
		typedef std::vector< Audio* > OutRefsVector;

		MultiChannelAudioFileReaderConfig   mConfig;
		OutputVector                        mOutputs;
		FloatOutControl                          mTimeOutput;
		Array< TIndex >                     mSelectedChannels;
		Array< TData* >                     mSamplesMatrix;
		AudioCodecs::Stream*                mNativeStream;
		TTime                               mCurrentBeginTime;
		TTime                               mDeltaTime;
		bool                                mEOFReached;
		bool                                mIsPaused;
		AudioFileSource                     mAudioFile;
	};
}

#endif // MultiChannelAudioFileReader.hxx

