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

#include "AudioPlayer.hxx"

#include "Audio.hxx"
#include "AudioIO.hxx"
#include "AudioOut.hxx"
#include "AudioManager.hxx"

#include "DataTypes.hxx"

#include "Err.hxx"

using namespace CLAM;

AudioPlayer* AudioPlayer::sCurrentPlayer = NULL;

AudioPlayer::AudioPlayer( Audio* audio, SigSlot::Slotv0& slot, TTime t0 ) 
	: mAudioReference( audio ), mT0( t0 )
{
	mCancel = false;
	sCurrentPlayer = this;

	mRequestStop.Connect( slot );

	pthread_create( &mThread, 0, sPlayingThreadSafe, this );
}

AudioPlayer::~AudioPlayer(  ) 
{
	mCancel = true ;
	pthread_join( mThread, 0 );
	delete mAudioReference;
}

void AudioPlayer::PlayingThreadSafe(  )
{
	TSize bufferSize=512;
	AudioManager audioManager( (int)mAudioReference->GetSampleRate(), 4096 );
	
	AudioIOConfig mOutCfgL;
	AudioIOConfig mOutCfgR;
	AudioOut mOutputL;
	AudioOut mOutputR;

	mOutCfgL.SetChannelID( 0 );
	mOutCfgR.SetChannelID( 1 );
		
	mOutputL.Configure( mOutCfgL );
	mOutputR.Configure( mOutCfgR );
	
	Audio tmpAudioBuffer;
	tmpAudioBuffer.SetSize(bufferSize);
	TSize dataSize = mAudioReference->GetSize();
	AudioManager::Current().Start();
	
	// first sample calculation
	TIndex firstSample = 
		((mT0*1000. - mAudioReference->GetBeginTime())/(mAudioReference->GetEndTime()-mAudioReference->GetBeginTime()))*((TTime)mAudioReference->GetSize()-1.);

	CLAM_ASSERT( firstSample >= 0, "Bad sample index!" );
	CLAM_ASSERT( firstSample < mAudioReference->GetSize(), "Bad sample index!" );

	mOutputL.Start();
	mOutputR.Start();
	for( TIndex i=firstSample; i<dataSize && !mCancel; i+=bufferSize )
	{
		mAudioReference->GetAudioChunk( i, i + tmpAudioBuffer.GetSize(), tmpAudioBuffer, false );
		mOutputR.Do( tmpAudioBuffer );
		mOutputL.Do( tmpAudioBuffer );
	}

	if( !mCancel ) // True if we finished to play all the Audio
		mRequestStop.Emit(  );
}

void* AudioPlayer::sPlayingThreadSafe(void* ptr)
{
 	((AudioPlayer*)ptr)->PlayingThreadSafe();

	return NULL;
}

void AudioPlayer::StopFromGUIThread(  )
{
	if( sCurrentPlayer )
	{
		delete sCurrentPlayer;
		sCurrentPlayer = NULL;
	}
}

