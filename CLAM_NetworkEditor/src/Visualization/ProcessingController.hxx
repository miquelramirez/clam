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

#ifndef __ProcessingController_hxx__
#define __ProcessingController_hxx__

#include "ModelController.hxx"
#include "Slotv1.hxx"

#include <string>
#include <list>

namespace CLAM
{
	class Processing;
	class ProcessingConfig;
}

namespace CLAMVM
{

class ProcessingController : public ModelController 
{
public:
	typedef std::list<std::string> NamesList;
		
private:
	CLAM::Processing* mObserved;

protected:
	
	NamesList mInPortNames;
	NamesList mOutPortNames;
	NamesList mInControlNames;
	NamesList mOutControlNames;
	
	void ConfigureProcessing( const CLAM::ProcessingConfig & );
public:
	ProcessingController();
	std::string GetObservedClassName();
	const CLAM::ProcessingConfig & GetObservedConfig();
	const char* GetClassName() const
	{
		return "ProcessingController";
	}
	bool Update()
	{
		return true;
	}
	bool Publish();
	bool BindTo( CLAM::Processing& obj );

	NamesList::iterator BeginInPortNames();
	NamesList::iterator EndInPortNames();
	
	NamesList::iterator BeginOutPortNames();
	NamesList::iterator EndOutPortNames();
	
	NamesList::iterator BeginInControlNames();
	NamesList::iterator EndInControlNames();
	
	NamesList::iterator BeginOutControlNames();
	NamesList::iterator EndOutControlNames();
	
	SigSlot::Slotv1< const CLAM::ProcessingConfig & > SlotConfigureProcessing;

};

} // namespace CLAMVM

#endif // __ProcessingController_hxx__ 
