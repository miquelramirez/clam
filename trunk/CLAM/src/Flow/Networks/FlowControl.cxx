/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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


#include "FlowControl.hxx"

#include <iostream>
#include "Network.hxx"
#include "Processing.hxx"
#include "Node.hxx"

namespace CLAM
{

FlowControl::FlowControl(  int frameSize )
	:mFrameSize(frameSize),
	 mNetwork(0),
	 mNetworkChanged(false)
{
}      

void FlowControl::AttachToNetwork( Network* network)
{
	mNetwork = network;
}

void FlowControl::ConfigurePorts(Processing& toConfigure) const
{
	PublishedInPorts::Iterator itin; // todo : should be a typdef of PublishedInPorts
	for (itin = toConfigure.GetInPorts().Begin(); 
	     itin != toConfigure.GetInPorts().End(); 
	     itin++)
		(*itin)->SetParams(mFrameSize);
		
	PublishedOutPorts::Iterator itout; // todo: idem
	for (itout = toConfigure.GetOutPorts().Begin(); 
	     itout != toConfigure.GetOutPorts().End(); 
	     itout++)
		(*itout)->SetParams(mFrameSize);		
}

void FlowControl::ProcessingConfigured( Processing & configured )
{
	if(!configured.ModifiesPortsAndControlsAtConfiguration())
		return;
	ConfigurePorts( configured );
}

void FlowControl::ProcessingAddedToNetwork( Processing& added )
{
	ConfigurePorts( added );
	NetworkTopologyChanged();
}

void FlowControl::ProcessingRemovedFromNetwork( Processing& removed )
{
	NetworkTopologyChanged();
}

void FlowControl::ConfigureNode( NodeBase& toConfigure ) const
{
	toConfigure.Configure( mFrameSize );
}


void FlowControl::DoProcessings()
{
	Network::ProcessingsMap::iterator it;
	for ( it=mNetwork->BeginProcessings(); it!=mNetwork->EndProcessings(); it++ )
	{
		if (it->second->CanDoUsingPorts())
			it->second->Do();
	}
}


}
