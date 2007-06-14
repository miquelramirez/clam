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

#include "OutControl.hxx"
#include "InControl.hxx"
#include "Processing.hxx"

namespace CLAM {

// Creation/Destruction

OutControl::OutControl(const std::string& name, Processing* parent, const bool publish) :
	mName(name), mParent(parent)
{
	if (parent && publish) 	parent->RegisterOutControl(this);
}
		
OutControl::~OutControl()
{
	while (!mLinks.empty())
		RemoveLink(*mLinks.front());
}
// Methods

std::list<InControl*>::iterator OutControl::BeginInControlsConnected()
{
	return mLinks.begin();
}

std::list<InControl*>::iterator OutControl::EndInControlsConnected()
{
	return mLinks.end();
}



void OutControl::AddLink(InControl& in)
{
	mLinks.push_back(&in);
	in.OutControlInterface_AddLink(*this);
}
void OutControl::RemoveLink(InControl& in)
{
	mLinks.remove( &in );
	in.OutControlInterface_RemoveLink(*this);
}
int OutControl::SendControl(TControlData val)
{
	int ret=0;
	std::list<InControl*>::iterator it;
	for (it=mLinks.begin(); it!=mLinks.end(); it++) 
	{
		ret = (*it)->DoControl(val);
	}
	// TODO: depracate controls with return value.
	return ret;
}

bool OutControl::IsConnected()
{
	return ! mLinks.empty();
}

bool OutControl::IsConnectedTo( InControl & in)
{
	std::list<InControl*>::iterator it;
	for (it=mLinks.begin(); it!=mLinks.end(); it++) 
		if ((*it) == &in)
			return true;

	return false;
}




}; //namespace CLAM

