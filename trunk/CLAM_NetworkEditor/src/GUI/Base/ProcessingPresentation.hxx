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

#ifndef __ProcessingPresentation_hxx__
#define __ProcessingPresentation_hxx__


#include <list>
#include <string>

#include "Presentation.hxx"
#include "ProcessingConfigPresentation.hxx"
#include "ProcessingController.hxx"
#include "Slotv1.hxx"
#include "Signalv1.hxx"

namespace CLAM
{
	class ProcessingConfig;
}

namespace NetworkGUI
{

class ConnectionPointPresentation;

class ProcessingPresentation : public CLAMVM::Presentation
{
protected:
	std::string mProcessingStatus;
	CLAMVM::ProcessingController::ProcessingExecState mProcessingState;
	typedef std::list< ConnectionPointPresentation * > ConnectionPointPresentationsList;

	ConnectionPointPresentationsList mInPortPresentations;
	ConnectionPointPresentationsList mOutPortPresentations;
	ConnectionPointPresentationsList mInControlPresentations;
	ConnectionPointPresentationsList mOutControlPresentations;
	std::string mName;
	std::string mObservedClassName;

	ProcessingConfigPresentation * mConfig;
public:
	ProcessingPresentation(const std::string& name= "unnamed");
	virtual ~ProcessingPresentation();
	void AttachTo(CLAMVM::ProcessingController & );
	virtual void Show() = 0;
	virtual void Hide() = 0;
	const std::string & GetName(){return mName;}
	ConnectionPointPresentation & GetOutPortPresentation( const std::string& );
	ConnectionPointPresentation & GetInPortPresentation( const std::string& );
	ConnectionPointPresentation & GetOutControlPresentation( const std::string& );
	ConnectionPointPresentation & GetInControlPresentation( const std::string& );
	bool HasInPort( const std::string& name);
	bool HasOutPort( const std::string& name);
	virtual void ChangeProcessingPresentationName( const std::string & name );
	void UpdateListOfPortsAndControls( CLAMVM::ProcessingController & controller );

protected:
	void SetConfig( const CLAM::ProcessingConfig & );
	void ConfigureProcessing( const CLAM::ProcessingConfig & );
	virtual void SetObservedClassName( const std::string& ) = 0;
	virtual void SetInPort( const std::string & ) = 0;
	virtual void SetOutPort( const std::string & ) = 0;
	virtual void SetInControl( const std::string & ) = 0;
	virtual void SetOutControl( const std::string & ) = 0;
	void ChangeState( CLAMVM::ProcessingController::ProcessingExecState state, const std::string & status );
	virtual void UpdatePresentation() = 0;
	

public:	//signals & slots
	SigSlot::Signalv1< ProcessingPresentation* > SignalRemoveProcessing;
	SigSlot::Signalv1< const CLAM::ProcessingConfig & > SignalConfigureProcessing;
	SigSlot::Signalv1< const std::string & > SignalProcessingNameChanged;
	SigSlot::Slotv1< const CLAM::ProcessingConfig &> SlotConfigureProcessing;
	SigSlot::Slotv1< const std::string & > SlotChangeProcessingPresentationName;
	SigSlot::Slotv2< CLAMVM::ProcessingController::ProcessingExecState, const std::string & > SlotChangeState; 
};


} //namespace NetworkGUI

#endif //  __ProcessingPresentation_hxx__

