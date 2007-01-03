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


#include "StaticFlowControl.hxx"
#include "Processing.hxx"
#include "OutPort.hxx"
#include "InPort.hxx"
#include "Network.hxx"

namespace CLAM
{

StaticFlowControl::StaticFlowControl( int frameSize, StaticCycle& c )
	: FlowControl (frameSize)
{
	SetCycle(c);
}

void StaticFlowControl::SetCycle(StaticCycle& c )
{
	mCycleDef = c;
}


void StaticFlowControl::Do()
{
	std::list<Processing*>::iterator next;
	if(mCycle.size()==0)
		AddProcessingsToDo(mCycle);	
	for(next=mCycle.begin(); next!=mCycle.end(); next++)
	{
		(*next)->Do();
	}
}

void StaticFlowControl::AddProcessingsToDo(std::list<Processing*> & toDo )
{
	int i;
	std::list<StaticCycleBranch>::iterator branch;
	std::list<std::string>::iterator processing;
	for(branch=mCycleDef.begin(); branch!=mCycleDef.end(); branch++)
	{
		for(i=0;i<(*branch).nLoops;i++)
		{
			for(processing=(*branch).processingNames.begin(); processing!=(*branch).processingNames.end(); processing++)
			{
				toDo.push_back(&mNetwork->GetProcessing((*processing)));
			}
		}
	}
}

}; // namespace CLAM


