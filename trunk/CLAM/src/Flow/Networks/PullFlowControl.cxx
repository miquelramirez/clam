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


#include "PullFlowControl.hxx"
#include "Processing.hxx"
#include "OutPort.hxx"
#include "InPort.hxx"
#include "Network.hxx"

namespace CLAM
{

PullFlowControl::PullFlowControl( int frameSize )
	: FlowControl (frameSize)
{
}

void PullFlowControl::ProcessingAddedToNetwork( Processing & added )
{
	NetworkTopologyChanged();

	if ( std::string(added.GetClassName()) == std::string("AudioSource") )
	{
		mSources.push_back( &added );
		return;
	}
	if ( std::string(added.GetClassName()) == std::string("AudioSink") )
	{
		mSinks.push_back( &added );
		return;
	}
	mNormalProcessings.push_back ( &added );
}

void PullFlowControl::ProcessingRemovedFromNetwork( Processing & removed )
{
	NetworkTopologyChanged();
	if ( std::string(removed.GetClassName()) == std::string("AudioSource") )
	{
		mSources.remove( &removed );
		return;
	}
	if ( std::string(removed.GetClassName()) == std::string("AudioSink") )
	{
		mSinks.push_back( &removed );
		return;
	}
	mNormalProcessings.push_back ( &removed );
}

void PullFlowControl::Do()
{
	// by now it have nothing of pulling
	// a naive approach: do sources, do normal processings, do sinks
	for (ProcessingList::iterator it=mSources.begin(); it!=mSources.end(); it++)
	{
		Processing* proc = *it;
		CLAM_ASSERT(proc->CanConsumeAndProduce(), "a Source should be able to produce");
		proc->Do();
		std::cerr << "Do: "<<proc->GetClassName() << std::endl;
	}
	ProcessingList pendingSinks(mSinks);
	while (!pendingSinks.empty())
	{
		bool noProcessingRun = true;
		for (ProcessingList::iterator it=mNormalProcessings.begin(); it!=mNormalProcessings.end(); it++)
		{
			Processing* proc = *it;
			if (proc->CanConsumeAndProduce() )
			{
				std::cerr << "Do: "<<proc->GetClassName() << std::endl;
				noProcessingRun = false;
				proc->Do();
			}
			else
			{
				std::cerr << "could NOT Do: "<<proc->GetClassName() 
					<< " port size: "<<proc->GetInPort("in").GetSize() << std::endl;
				
			}
		}
		for (ProcessingList::iterator it=pendingSinks.begin(); it!=pendingSinks.end(); )
		{
			Processing* proc = *it;
			if (!proc->CanConsumeAndProduce())
			{
				it++;
				continue;
			}
			std::cerr << "Do: "<<proc->GetClassName() << std::endl;
			proc->Do();
			it = pendingSinks.erase(it);
			noProcessingRun = false;
		}
		if (noProcessingRun) 
		{
			std::cerr << "Bad luck all stuck" << std::endl;
			break;
		}
	}
}


} // namespace CLAM


