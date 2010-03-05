/*
 * Copyright (c) 2001-2007 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "NetworkPlayer.hxx"
#include "AudioSource.hxx"
#include "AudioBufferSource.hxx"
#include "AudioSink.hxx"
#include "AudioBufferSink.hxx"

namespace CLAM
{

std::string NetworkPlayer::SourcesAndSinksToString()
{
	CacheSourcesAndSinks();
	std::string result;
	for (unsigned i=0; i<_exportedSources.size(); i++)
		result += " * source:\t"+SourceName(i)+"\n";
	for (unsigned i=0; i<_exportedSinks.size(); i++)
		result += " * sink:\t"+SinkName(i)+"\n";
	return result;
}

void NetworkPlayer::SetSourceBuffer(unsigned source, const float * data, unsigned nframes)
{
	Processing * processing = _exportedSources[source].processing;
	unsigned port = _exportedSources[source].port;
	SetExternalBuffer<AudioSource>(processing, data, nframes, port);
	SetExternalBuffer<AudioBufferSource>(processing, data, nframes, port);
}
void NetworkPlayer::SetSinkBuffer(unsigned sink, float * data, unsigned nframes)
{
	Processing * processing = _exportedSinks[sink].processing;
	unsigned port = _exportedSinks[sink].port;
	SetExternalBuffer<AudioSink>(processing, data, nframes, port);
	SetExternalBuffer<AudioBufferSink>(processing, data, nframes, port);
}
void NetworkPlayer::SetSinkFrameSize(unsigned sink, unsigned frameSize)
{
	unsigned port = _exportedSinks[sink].port;
	Processing * processing = _exportedSinks[sink].processing;
	SetFrameAndHopSizeIf<AudioSink>(processing,frameSize,port);
	SetFrameAndHopSizeIf<AudioBufferSink>(processing,frameSize,port);
}
void NetworkPlayer::SetSourceFrameSize(unsigned source, unsigned frameSize)
{
	unsigned port = _exportedSources[source].port;
	Processing * processing = _exportedSources[source].processing;
	SetFrameAndHopSizeIf<AudioSource>(processing,frameSize,port);
	SetFrameAndHopSizeIf<AudioBufferSource>(processing,frameSize,port);
}

} //namespace

