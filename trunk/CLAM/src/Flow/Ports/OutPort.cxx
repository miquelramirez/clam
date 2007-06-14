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

#include "OutPort.hxx"
#include "Processing.hxx"

namespace CLAM
{

OutPortBase::OutPortBase( const std::string & name, Processing * proc )
	: mName(name),
	  mProcessing(proc),
	mPublisher(0)
{
	if(proc)
		proc->RegisterOutPort(this);
}

OutPortBase::~OutPortBase()
{
	if (mPublisher) 
		mPublisher->UnpublishOutPort();
}

const std::string & OutPortBase::GetName()
{
	return mName;
}

Processing * OutPortBase::GetProcessing()
{
	return mProcessing;
}

OutPortBase::InPortsList::iterator OutPortBase::BeginVisuallyConnectedInPorts()
{
	return mVisuallyConnectedPorts.begin();
}

OutPortBase::InPortsList::iterator OutPortBase::EndVisuallyConnectedInPorts()
{
	return mVisuallyConnectedPorts.end();
}

void OutPortBase::SetPublisher( OutPortBase& publisher) 
{
	mPublisher = &publisher; 
};
void OutPortBase::UnsetPublisher( ) 
{
	mPublisher = 0;
};

} // namespace CLAM

