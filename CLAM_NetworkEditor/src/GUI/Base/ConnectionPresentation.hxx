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

#ifndef __CONNECTIONPRESENTATION_HXX__
#define __CONNECTIONPRESENTATION_HXX__

#include "Presentation.hxx"
#include "Slotv1.hxx"
#include "Signalv1.hxx"
#include <string>

namespace CLAM
{
	class Processing;
}

namespace CLAMVM
{
	class ConnectionAdapter;
}

namespace NetworkGUI
{

class ConnectionPresentation : public CLAMVM::Presentation
{
public:
	ConnectionPresentation();
	virtual ~ConnectionPresentation();
	virtual void AttachTo(CLAMVM::ConnectionAdapter & );
	virtual void Show() = 0;
	virtual void Hide() = 0;
	void SetOutName( const std::string & );
	void SetInName( const std::string & );

	const std::string &GetInName()
	{
		return mInName;
	}
	const std::string & GetOutName()
	{
		return mOutName;
	}

protected:
	std::string mOutName;
	std::string mInName;
	
public: //slots
	SigSlot::Signalv1< ConnectionPresentation* > SignalRemoveConnection;
	SigSlot::Slotv1< const std::string & > SlotSetOutName;
	SigSlot::Slotv1< const std::string & > SlotSetInName;

};

} // namespace NetworkGUI

#endif // __CONNECTIONPRESENTATION_HXX__
