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
	  mProcessing(proc)
{
	if(proc)
		proc->PublishOutPort(this);
}

OutPortBase::~OutPortBase()
{
}

const std::string & OutPortBase::GetName()
{
	return mName;
}

Processing * OutPortBase::GetProcessing()
{
	return mProcessing;
}

OutPortBase::InPortsList::iterator OutPortBase::BeginConnectedInPorts()
{
	return mConnectedInPortsList.begin();
}

OutPortBase::InPortsList::iterator OutPortBase::EndConnectedInPorts()
{
	return mConnectedInPortsList.end();
}

} // namespace CLAM

