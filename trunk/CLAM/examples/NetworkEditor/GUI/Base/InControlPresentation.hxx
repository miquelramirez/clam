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

#ifndef __INCONTROLPRESENTATION_HXX__
#define __INCONTROLPRESENTATION_HXX__

#include "Presentation.hxx"
#include "Slotv1.hxx"
#include <string>

namespace CLAMVM
{
	class ControlModel;
}

namespace NetworkGUI
{

class InControlPresentation : public CLAMVM::Presentation
{
protected:
	std::string mName;
public:
	InControlPresentation();
	virtual ~InControlPresentation();
	virtual void AttachTo(CLAMVM::ControlModel & );
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual const std::string & GetName(){return mName;}

protected:
	virtual void OnNewName(const std::string& name) = 0;

public: //slots
	SigSlot::Slotv1<const std::string &> SetName;
};

} // namespace NetworkGUI

#endif // __INCONTROLPRESENTATION_HXX__
