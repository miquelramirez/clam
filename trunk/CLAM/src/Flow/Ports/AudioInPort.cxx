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

#include "AudioInPort.hxx"

namespace CLAM
{

AudioInPort::AudioInPort( const std::string & name, Processing * proc )
	: InPort<TData>(name,proc)
{
	const int size = 512; // arbitrary default value. It makes more sense that size==1 like generic ports.
	SetSize(size); 
	SetHop(size);
}

AudioInPort::~AudioInPort()
{
}

const Audio & AudioInPort::GetAudio()
{
	mAudio.GetBuffer().SetPtr( &(mRegion[0]), mRegion.Size() );
	return mAudio;
}

void AudioInPort::SetSampleRate( TData sampleRate )
{
	mAudio.SetSampleRate( sampleRate );
}

TData AudioInPort::GetSampleRate()
{
	return mAudio.GetSampleRate();
}

} // namespace CLAM

