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

OutControl::OutControl(const std::string& name, Processing* parent, const bool publish)
	: BaseTypedOutControl(name, parent, publish)
{
}
bool OutControl::IsLinkable(const BaseTypedInControl& in)
{
	return typeid(TControlData) == in.ControlType();

}
		
void OutControl::SendControl(TControlData val)
{
	Peers::iterator it;
	for (it=mLinks.begin(); it!=mLinks.end(); it++) 
	{
		((FloatInControl*)(*it))->DoControl(val);
	}
}



}; //namespace CLAM

