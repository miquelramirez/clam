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

#include "InPort.hxx"
#include "OutPort.hxx"
#include "Processing.hxx"

namespace CLAM
{

InPortBase::InPortBase( const std::string & name, Processing * proc )
	: mAttachedOutPort(0),
	  mName(name),
	  mProcessing(proc)
{
	if(proc)
		proc->PublishInPort(this);
}

InPortBase::~InPortBase()
{
}

OutPortBase * InPortBase::GetAttachedOutPort() 
{
	return mAttachedOutPort;
}

void InPortBase::SetAttachedOutPort( OutPortBase* out )
{
	mAttachedOutPort = out;
}

const std::string & InPortBase::GetName()
{
	return mName;
}

Processing * InPortBase::GetProcessing()
{
	return mProcessing;
}

void InPortBase::Disconnect()
{	
	CLAM_DEBUG_ASSERT(mAttachedOutPort, "InPortBase::Disconnect() - InPort is not connected" );
	mAttachedOutPort->DisconnectFromIn( *this );
}

} // namespace CLAM

