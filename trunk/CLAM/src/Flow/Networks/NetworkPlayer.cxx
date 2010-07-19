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
#include "ControlSource.hxx"
#include "ControlSink.hxx"

namespace CLAM
{

void NetworkPlayer::CacheSourcesAndSinks()
{
	_audioSources.clear();
	Network::Processings sources = GetSources();
	for (Network::Processings::const_iterator it = sources.begin(); it != sources.end(); ++it)
	{
		Processing * processing = *it;
		std::string processingName = _network->GetNetworkId(processing);
		unsigned nPorts = processing->GetNOutPorts();
		for (unsigned i=0; i<nPorts; i++)
		{
			std::string portName = processingName;
			if (nPorts > 1)
				portName +=  "_"+processing->GetOutPort(i).GetName();
			_audioSources.push_back(ExportedPort(processing,i, portName));
		}
	}
	_audioSinks.clear();
	Network::Processings sinks = GetSinks();
	for (Network::Processings::const_iterator it = sinks.begin(); it != sinks.end(); ++it)
	{
		Processing * processing = *it;
		std::string processingName = _network->GetNetworkId(processing);
		unsigned nPorts = processing->GetNInPorts();
		for (unsigned i=0; i<nPorts; i++)
		{
			std::string portName = processingName;
			if (nPorts > 1)
				portName +="_"+processing->GetInPort(i).GetName();
			_audioSinks.push_back(ExportedPort(processing,i, portName));
		}
	}
	_controlSources.clear();
	Network::Processings controlSources = GetControlSources();
	for (Network::Processings::iterator it=controlSources.begin(); it!=controlSources.end(); it++)
	{
		Processing * processing = *it;
		std::string processingName = _network->GetNetworkId(processing);
		unsigned nControls = processing->GetNOutControls();
		for (unsigned i=0; i<nControls; i++)
		{
			std::string portName = processingName;
			if (nControls > 1)
				portName +="_"+processing->GetOutControl(i).GetName();
			_controlSources.push_back(ExportedPort(processing,i, portName));
		}
	}
	_controlSinks.clear();
	Network::Processings controlSinks = GetControlSinks();
	for (Network::Processings::iterator it=controlSinks.begin(); it!=controlSinks.end(); it++)
	{
		Processing * processing = *it;
		std::string processingName = _network->GetNetworkId(processing);
		unsigned nControls = processing->GetNInControls();
		for (unsigned i=0; i<nControls; i++)
		{
			std::string portName = processingName;
			if (nControls > 1)
				portName +="_"+processing->GetInControl(i).GetName();
			_controlSinks.push_back(ExportedPort(processing,i,portName));
		}
	}
}

std::string NetworkPlayer::SourcesAndSinksToString()
{
	CacheSourcesAndSinks();
	std::string result;
	for (unsigned i=0; i<_audioSources.size(); i++)
		result += " * source:\t"+SourceName(i)+"\n";
	for (unsigned i=0; i<_audioSinks.size(); i++)
		result += " * sink:\t"+SinkName(i)+"\n";
	return result;
}

void NetworkPlayer::ReadControlSource(unsigned source, const float * data)
{
	if (not data) return;
	ControlSource * processing =
		(ControlSource*) _controlSources[source].processing;
	unsigned port = _controlSources[source].port;
	processing->Do( *data , port);
}

void NetworkPlayer::FeedControlSink(unsigned sink, float * data)
{
	if (not data) return;
	ControlSink * processing =
		(ControlSink*) _controlSinks[sink].processing;
	unsigned port = _controlSinks[sink].port;
	*data = processing->GetControlValue(port);
	
}

void NetworkPlayer::SetSourceBuffer(unsigned source, const float * data, unsigned nframes)
{
	Processing * processing = _audioSources[source].processing;
	unsigned port = _audioSources[source].port;
	SetExternalBuffer<AudioSource>(processing, data, nframes, port);
	SetExternalBuffer<AudioBufferSource>(processing, data, nframes, port);
}
void NetworkPlayer::SetSinkBuffer(unsigned sink, float * data, unsigned nframes)
{
	Processing * processing = _audioSinks[sink].processing;
	unsigned port = _audioSinks[sink].port;
	SetExternalBuffer<AudioSink>(processing, data, nframes, port);
	SetExternalBuffer<AudioBufferSink>(processing, data, nframes, port);
}
void NetworkPlayer::SetSinkFrameSize(unsigned sink, unsigned frameSize)
{
	unsigned port = _audioSinks[sink].port;
	Processing * processing = _audioSinks[sink].processing;
	SetFrameAndHopSizeIf<AudioSink>(processing,frameSize,port);
	SetFrameAndHopSizeIf<AudioBufferSink>(processing,frameSize,port);
}
void NetworkPlayer::SetSourceFrameSize(unsigned source, unsigned frameSize)
{
	unsigned port = _audioSources[source].port;
	Processing * processing = _audioSources[source].processing;
	SetFrameAndHopSizeIf<AudioSource>(processing,frameSize,port);
	SetFrameAndHopSizeIf<AudioBufferSource>(processing,frameSize,port);
}

} //namespace

