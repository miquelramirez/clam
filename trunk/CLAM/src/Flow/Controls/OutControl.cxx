/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "ControlLinker.hxx"
#include "Processing.hxx"

namespace CLAM {

// Creation/Destruction

OutControl::OutControl(std::string name, Processing* parent, const bool publish) :
	mName(name)
{
	if (publish) parent->PublishOutControl(this);
}
		
/*OutControl::OutControlIterator()()
{
}
*/
OutControl::~OutControl() {}
/*OutControl::OutControl(const OutControl& src)
{
	MTG_ASSERT(false, "long text saying that you are in the wrong way... (todo)")
}
*/
// Methods

void OutControl::AddLink(InControl* in)
{
	mLinks.push_back(in);
}
void OutControl::RemoveLink(InControl* in)
{
	mLinks.remove(in);
}
int OutControl::SendControl(TControlData val)
{
	int ret=0;
	std::list<InControl*>::iterator it;
	for (it=mLinks.begin(); it!=mLinks.end(); it++) 
	{
		ret = (*it)->DoControl(val);
	}
	// TODO: decide if returning just the last result is ok
	return ret;
}

//Redefined Methods
//  OutControl::OutControlIterator OutControl::GetOutControls() const
//  {
//  	return 0;  //TODO: return a single element iterator?
//  }
//  OutControl::InControlIterator OutControl::GetInControls() const
//  {
//  	return 0;  //TODO: works well as a null iterator?
//  }

}; //namespace CLAM
