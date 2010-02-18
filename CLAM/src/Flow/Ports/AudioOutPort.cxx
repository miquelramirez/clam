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

#include "AudioOutPort.hxx"

namespace CLAM
{

AudioOutPort::AudioOutPort( const std::string & name, Processing * proc )
	: OutPort<TData>(name,proc)
{
	const int size = 512; // arbitrary default value. It makes more sense that size==1 like generic ports.
	// now set the same as AudioOut default config
	SetSize(size); 
	SetHop(size);
	SetSampleRate(44100);
}

AudioOutPort::~AudioOutPort()
{
}

Audio & AudioOutPort::GetAudio()
{
	mAudio.GetBuffer().SetPtr( &(mRegion[0]), mRegion.Size() );
	return mAudio;
}

void AudioOutPort::SetSampleRate( TData sampleRate )
{
	mAudio.SetSampleRate( sampleRate );
}

TData AudioOutPort::GetSampleRate()
{
	return mAudio.GetSampleRate();
}

Audio & AudioOutPort::GetLastWrittenAudio( int offset )
{
	CLAM_DEBUG_ASSERT( (0 <= offset) && (offset <= GetSize()), 
		"AudioOutPort::GetLastWrittenData - Index out of bounds" );

	mAudio.GetBuffer().SetPtr( &(mRegion.GetLastWrittenData(offset)), mRegion.Size() );
	return mAudio;
}

} // namespace CLAM

