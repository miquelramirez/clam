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


#include "PushFlowControl.hxx"
#include "Processing.hxx"
#include "OutPort.hxx"
#include "InPort.hxx"
#include "Network.hxx"

namespace CLAM
{

PushFlowControl::PushFlowControl( int frameSize )
	: FlowControl (frameSize)
{
}

void PushFlowControl::ProcessingAddedToNetwork( Processing & added )
{
	NetworkTopologyChanged();

	ConfigurePorts( added );
	if (added.GetInPorts().Size() == 0) // if it's a generator
		mGenerators.push_back( &added );
}

void PushFlowControl::ProcessingRemovedFromNetwork( Processing & removed )
{
	NetworkTopologyChanged();

	if (removed.GetInPorts().Size() == 0) // if it's a generator
		mGenerators.remove( &removed );
}
void PushFlowControl::DoProcessings()
{
	std::list< Processing* > toDo(mGenerators);
	std::list< Processing* > done;

	while (!toDo.empty())
	{
		// pop the next processing
		Processing * next = *(toDo.begin()); // the first
		toDo.pop_front();

		if(next->CanConsumeAndProduce())
		{
			next->Do();
			done.push_back(next);
		}
		AddNewPossibleProcessingsToDo(next, toDo, done);
	}
}


void PushFlowControl::AddNewPossibleProcessingsToDo(
	Processing * producer, 
	std::list<Processing*> & toDo,
	std::list<Processing*> & executed )
{
	
	// for each out port of the processing already executed
	OutPortRegistry::Iterator itOutPort;
	
	for (itOutPort=producer->GetOutPorts().Begin(); 
	     itOutPort!=producer->GetOutPorts().End(); 
	     itOutPort++)
	{
		Network::InPortsList consumers;
		consumers = mNetwork->GetInPortsConnectedTo( **itOutPort );
		
		Network::InPortsList::iterator itInPort;

		for (itInPort=consumers.begin(); itInPort!=consumers.end(); itInPort++)
		{
			Processing * proc = (*itInPort)->GetProcessing();
			if (proc->CanConsumeAndProduce())				
				toDo.push_back( proc );
		}
	}
}

} // namespace CLAM


