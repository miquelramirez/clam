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
#include "Slotv2.hxx"
#include "Signalv1.hxx"
#include <string>

namespace CLAM
{
	class Processing;
}

namespace CLAMVM
{
	class ConnectionModel;
}

namespace NetworkGUI
{

class ConnectionPresentation : public CLAMVM::Presentation
{
public:
	ConnectionPresentation();
	virtual ~ConnectionPresentation();
	virtual void AttachTo(CLAMVM::ConnectionModel & );
	virtual void Show() = 0;
	virtual void Hide() = 0;

	const std::string &GetInName()
	{
		return mInName;
	}
	const std::string & GetOutName()
	{
		return mOutName;
	}

/*	CLAM::Processing * GetInProcessing()
	{
		return mInProc;
	}
	CLAM::Processing * GetOutProcessing()
	{
		return mInProc;
	}
*/

protected:
	virtual void OnNewNames( const std::string &, const std::string &);

//	CLAM::Processing * mOutProc;
//	CLAM::Processing * mInProc;
	std::string mOutName;
	std::string mInName;
	
public: //slots
	SigSlot::Slotv2< const std::string &, const std::string &> SetNames;
	SigSlot::Signalv1< ConnectionPresentation* > RemoveConnection;

};

} // namespace NetworkGUI

#endif // __CONNECTIONPRESENTATION_HXX__
