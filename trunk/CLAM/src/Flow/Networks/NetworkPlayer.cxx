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

namespace CLAM
{

void NetworkPlayer::CollectSourcesAndSinks()
{
	_sources.clear();
	_sinks.clear();
	Network & net = GetNetwork();
	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		std::string processingType = it->second->GetClassName();
		if ( processingType == "AudioSource" )
			_sources.push_back( (AudioSource*)it->second );
		else if ( processingType == "AudioSink" )
			_sinks.push_back( (AudioSink*)it->second );
	}
}
std::string NetworkPlayer::SourcesAndSinksToString()
{

	std::string sourceNames;
	std::string sinkNames;
	Network & net = GetNetwork();

	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		std::string processingType = it->second->GetClassName();
		if ( processingType == "AudioSource" )
			sourceNames += " * source:\t"+it->first+"\n";
		else if ( processingType == "AudioSink" )
			sinkNames += " * sink:\t"+it->first+"\n";
	}
	return (sourceNames+sinkNames);
}

} //namespace

