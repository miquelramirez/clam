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

#include "ProcessingController.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"

#include <iostream> // TODO: remove

namespace CLAMVM
{

ProcessingController::ProcessingController()
	: mObserved(0)
{
	SlotConfigureProcessing.Wrap( this, &ProcessingController::ConfigureProcessing );
	SlotProcessingNameChanged.Wrap( this, &ProcessingController::ProcessingNameChanged );
	SlotSendOutControlValue.Wrap( this, &ProcessingController::SendOutControlValue );
}

void ProcessingController::ConfigureProcessing( const CLAM::ProcessingConfig & cfg) 
{
	// HasMutableInterface!
	if(mObserved->ModifiesPortsAndControlsAtConfiguration())
		SignalRemoveAllConnections.Emit( mObserved );

	bool wasRunning = false;
	if (mObserved->GetExecState() == CLAM::Processing::Running)
	{
		wasRunning = true;
		mObserved->Stop();
	}

	SignalConfigureProcessing.Emit( mObserved, cfg );
	SignalChangeState.Emit( mObserved->GetExecState(), mObserved->GetStatus() );

	if(mObserved->ModifiesPortsAndControlsAtConfiguration())
	{
		UpdateListOfPortsAndControls();
		SignalRebuildProcessingPresentationAttachedTo.Emit( this, mObserved );
	}

	if(wasRunning)
		mObserved->Start();
}

void ProcessingController::ProcessingNameChanged( const std::string & newName )
{
	SignalProcessingNameChanged.Emit( newName, this );
}
	
bool ProcessingController::Publish()
{
	CLAM_ASSERT( mObserved, "Trying to publish an unbinded processing controller" );
	return true;
}

std::string ProcessingController::GetObservedClassName()
{
	if(mObserved)
		return mObserved->GetClassName();
	return "unbinded processing controller";
}

void ProcessingController::UpdateListOfPortsAndControls()
{
	mInPortNames.clear();
	mOutPortNames.clear();
	mInControlNames.clear();
	mOutControlNames.clear();

	CLAM::PublishedInPorts::ConstIterator itPortIn;
	for (itPortIn = mObserved->GetInPorts().Begin(); itPortIn != mObserved->GetInPorts().End(); itPortIn++)
		mInPortNames.push_back((*itPortIn)->GetName());
		
	CLAM::PublishedOutPorts::ConstIterator itPortOut;
	for (itPortOut = mObserved->GetOutPorts().Begin(); itPortOut != mObserved->GetOutPorts().End(); itPortOut++)
		mOutPortNames.push_back((*itPortOut)->GetName());

	CLAM::PublishedInControls::ConstIterator itCtrlIn;
	for (itCtrlIn = mObserved->GetInControls().Begin(); itCtrlIn != mObserved->GetInControls().End(); itCtrlIn++)
		mInControlNames.push_back((*itCtrlIn)->GetName());

	CLAM::PublishedOutControls::ConstIterator itCtrlOut;
	for (itCtrlOut = mObserved->GetOutControls().Begin(); itCtrlOut != mObserved->GetOutControls().End(); itCtrlOut++)
		mOutControlNames.push_back((*itCtrlOut)->GetName());
}

bool ProcessingController::BindTo( CLAM::Processing& obj )
{
	mObserved = dynamic_cast< CLAM::Processing* > (&obj);
	
	if ( !mObserved )
		return false;
	UpdateListOfPortsAndControls();
	return true;
}

const CLAM::ProcessingConfig & ProcessingController::GetObservedConfig()
{
	return mObserved->GetConfig();
}

ProcessingController::NamesList::iterator ProcessingController::BeginInPortNames()
{
	return mInPortNames.begin();
}
	
ProcessingController::NamesList::iterator ProcessingController::EndInPortNames()
{
	return mInPortNames.end();
}
	
ProcessingController::NamesList::iterator ProcessingController::BeginOutPortNames()
{
	return mOutPortNames.begin();
}
	
ProcessingController::NamesList::iterator ProcessingController::EndOutPortNames()
{
	return mOutPortNames.end();
}

ProcessingController::NamesList::iterator ProcessingController::BeginInControlNames()
{
	return mInControlNames.begin();
}
	
ProcessingController::NamesList::iterator ProcessingController::EndInControlNames()
{
	return mInControlNames.end();
}
	
ProcessingController::NamesList::iterator ProcessingController::BeginOutControlNames()
{
	return mOutControlNames.begin();
}
	
ProcessingController::NamesList::iterator ProcessingController::EndOutControlNames()
{
	return mOutControlNames.end();
}

void ProcessingController::SetName( const std::string & name )
{
	SignalChangeProcessingPresentationName.Emit( name );
}

void ProcessingController::SendOutControlValue( const std::string & name, CLAM::TControlData value )
{
	mObserved->GetOutControls().Get( name ).SendControl( value );
}

} //namespace CLAMVM

