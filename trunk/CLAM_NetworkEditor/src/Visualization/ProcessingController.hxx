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
#include "Slotv2.hxx"
#include "Signalv3.hxx"
#include "Signalv2.hxx"
#include "Signalv1.hxx"
#include "Processing.hxx"

#include <string>
#include <list>

namespace CLAMVM
{

class ProcessingController : public ModelController 
{
public:	
	typedef CLAM::Processing::ExecState ProcessingExecState;
	typedef std::list<std::string> NamesList;
		
private:
	CLAM::Processing * mObserved;

protected:
	
	NamesList mInPortNames;
	NamesList mOutPortNames;
	NamesList mInControlNames;
	NamesList mOutControlNames;

	/** 
	 * In this method new config from GUI is passed to Network; if its state is running the processing is stopped before
	 */
	void ConfigureProcessing( const CLAM::ProcessingConfig & );
	
	void ProcessingNameChanged( const std::string & );
	void SendOutControlValue( const std::string &, CLAM::TControlData );

public:
	ProcessingController();
	virtual ~ProcessingController(){}
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

	ProcessingExecState GetProcessingExecState()
	{
		return mObserved->GetExecState();
	}

	const std::string & GetProcessingStatus()
	{
		return mObserved->GetStatus();
	}
	

	void SetName( const std::string & );

	void UpdateListOfPortsAndControls();
	
	/** 
	 * We create a list if ports and controls for each processing
	 */
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
	SigSlot::Signalv2< CLAM::Processing * , const CLAM::ProcessingConfig & > SignalConfigureProcessing;
	SigSlot::Slotv1< const std::string & > SlotProcessingNameChanged;
	SigSlot::Slotv2< const std::string &, CLAM::TControlData > SlotSendOutControlValue;
	SigSlot::Signalv2< const std::string &, ProcessingController * > SignalProcessingNameChanged;
	SigSlot::Signalv1< const std::string & > SignalChangeProcessingPresentationName;
	SigSlot::Signalv2< ProcessingExecState, const std::string & > SignalChangeState;
	SigSlot::Signalv1< CLAM::Processing * > SignalRemoveAllConnections;
	SigSlot::Signalv2< ProcessingController *, CLAM::Processing * > SignalRebuildProcessingPresentationAttachedTo;
};

} // namespace CLAMVM

#endif // __ProcessingController_hxx__ 
