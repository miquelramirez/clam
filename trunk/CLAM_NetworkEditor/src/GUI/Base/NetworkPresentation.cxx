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

#include "NetworkPresentation.hxx"
#include "NetworkController.hxx"
#include "ProcessingController.hxx"
//#include "ConnectionAdapter.hxx"
#include "ProcessingPresentation.hxx"
#include "ConnectionPresentation.hxx"
#include "ConnectionPointPresentation.hxx"
#include "Processing.hxx"

namespace NetworkGUI
{

NetworkPresentation::NetworkPresentation()
{
	SlotRemovePortConnection.Wrap( this, &NetworkPresentation::RemovePortConnection);
	SlotCreatePortConnectionPresentation.Wrap( this, &NetworkPresentation::CreatePortConnectionPresentation);
	SlotCreatePortConnection.Wrap( this, &NetworkPresentation::CreatePortConnection);


	SlotRemoveConnectionPresentation.Wrap( this, &NetworkPresentation::RemoveConnectionPresentation);
	
	SlotRemoveControlConnection.Wrap( this, &NetworkPresentation::RemoveControlConnection);
	SlotCreateControlConnectionPresentation.Wrap( this, &NetworkPresentation::CreateControlConnectionPresentation);
	SlotCreateControlConnection.Wrap( this, &NetworkPresentation::CreateControlConnection);

	SlotCreateProcessingPresentation.Wrap( this, &NetworkPresentation::CreateProcessingPresentation );
	SlotAddProcessing.Wrap( this, &NetworkPresentation::AddProcessing );
	SlotRemoveProcessing.Wrap( this, &NetworkPresentation::RemoveProcessing );
	SlotRebuildProcessingPresentationAttachedTo.Wrap( this, &NetworkPresentation::RebuildProcessingPresentationAttachedTo );
	
	SlotChangeState.Wrap( this, &NetworkPresentation::ChangeState );
	SlotClear.Wrap(this, &NetworkPresentation::Clear );

	SlotChangeConnectionPresentationNames.Wrap( this, &NetworkPresentation::ChangeConnectionPresentationNames );
}

void NetworkPresentation::ChangeState( bool newState )
{
	SignalChangeState.Emit ( newState );
}

void NetworkPresentation::RemoveConnectionPresentation( const std::string & out, const std::string & in)
{
	ConnectionPresentationIterator it;
	ConnectionPresentation * con = 0;
	for( it=mConnectionPresentations.begin(); it!=mConnectionPresentations.end(); it++)
	{
		if( ((*it)->GetOutName() == out) && ((*it)->GetInName() == in) )
		{
			con = *it;
			break;
		}
	}
	CLAM_ASSERT(con, "NetworkPresentation::RemoveConnectionPresentation: Connection doesn't exist" );
	
	mConnectionPresentationsToRemove.push_back( con );
	con->Hide();
}

void NetworkPresentation::RemovePortConnection(  ConnectionPresentation * con)
{
	mConnectionPresentationsToRemove.push_back(con);
	con->Hide();

	SignalRemovePortConnection.Emit( con->GetOutName(), con->GetInName() );
}

void NetworkPresentation::RemoveControlConnection(  ConnectionPresentation * con)
{
	mConnectionPresentationsToRemove.push_back(con);
	con->Hide();

	SignalRemoveControlConnection.Emit( con->GetOutName(), con->GetInName() );
}

void NetworkPresentation::UpdatePresentations()
{
	ConnectionPresentationIterator itc;
	for(itc=mConnectionPresentationsToRemove.begin(); itc!=mConnectionPresentationsToRemove.end(); itc++)
	{
		mConnectionPresentations.remove(*itc);
		delete *itc;
	}
	
	mConnectionPresentationsToRemove.clear();
	
	ProcessingPresentationIterator it;
	for(it=mProcessingPresentationsToRemove.begin(); it!=mProcessingPresentationsToRemove.end(); it++)
	{
		mProcessingPresentations.remove(*it);
		delete *it;
	}
	mProcessingPresentationsToRemove.clear();
}


void NetworkPresentation::RemoveProcessing( ProcessingPresentation * proc)
{
	mProcessingPresentationsToRemove.push_back(proc);
	SignalRemoveProcessing.Emit( proc->GetName() ); 
}

void NetworkPresentation::RebuildProcessingPresentationAttachedTo( const std::string & name, CLAMVM::ProcessingController * controller )
{	

	ProcessingPresentationIterator it;
	for( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++ )
	{
		if((*it)->GetName()==name)
		{
			(*it)->UpdateListOfPortsAndControls( *controller );
			(*it)->Show();
			return;
		}
	}
	CLAM_ASSERT( false, "NetworkPresentation::RebuildProcessingPresentationAttachedTo : name not found in processing presentations list" );

}

NetworkPresentation::~NetworkPresentation()
{
	ConnectionPresentationIterator itc;
	for(itc=mConnectionPresentations.begin(); itc!=mConnectionPresentations.end(); itc++)
		delete *itc;

	ProcessingPresentationIterator it;
	for(it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
		delete *it;
}

void NetworkPresentation::AttachTo(CLAMVM::NetworkController & controller)
{
	SetName( controller.GetName() );
	controller.SignalCreateProcessingPresentation.Connect( SlotCreateProcessingPresentation );
	CLAMVM::NetworkController::ProcessingControllersMap::iterator it;	
	for(it=controller.BeginProcessingControllers(); it!=controller.EndProcessingControllers(); it++ )
		CreateProcessingPresentation( it->first, it->second );
		
	SignalChangeState.Connect( controller.SlotChangeState );
	
	SignalAddProcessing.Connect( controller.SlotAddProcessing );
	SignalRemoveProcessing.Connect( controller.SlotRemoveProcessing );
	controller.SignalRebuildProcessingPresentationAttachedTo.Connect( SlotRebuildProcessingPresentationAttachedTo );
	controller.SignalClearPresentation.Connect( SlotClear );
	
	SignalCreatePortConnection.Connect( controller.SlotCreatePortConnection );
	SignalRemovePortConnection.Connect( controller.SlotRemovePortConnection );
	SignalCreateControlConnection.Connect( controller.SlotCreateControlConnection );
	SignalRemoveControlConnection.Connect( controller.SlotRemoveControlConnection );
	controller.SignalCreateControlConnectionPresentation.Connect( SlotCreateControlConnectionPresentation );
	controller.SignalCreatePortConnectionPresentation.Connect( SlotCreatePortConnectionPresentation );
	controller.SignalRemoveConnectionPresentation.Connect( SlotRemoveConnectionPresentation );
	controller.SignalChangeConnectionPresentationNames.Connect( SlotChangeConnectionPresentationNames );

	SignalClear.Connect( controller.SlotClear );
	SignalSaveNetworkTo.Connect( controller.SlotSaveNetwork );
	SignalLoadNetworkFrom.Connect( controller.SlotLoadNetwork );
}

void NetworkPresentation::ChangeConnectionPresentationNames( const std::string & oldName, const std::string & newName )
{
	ConnectionPresentationIterator it;
	for(it=mConnectionPresentations.begin(); it!=mConnectionPresentations.end(); it++ )
	{
		if(GetProcessingIdentifier((*it)->GetInName()) == oldName )
			(*it)->SetInName( newName + "." + GetLastIdentifier( (*it)->GetInName()) );
		if(GetProcessingIdentifier((*it)->GetOutName()) == oldName )
			(*it)->SetOutName( newName + "." + GetLastIdentifier( (*it)->GetOutName()) );
	}
}

ConnectionPointPresentation & NetworkPresentation::GetOutPortPresentationByCompleteName(const std::string & name)
{
	ProcessingPresentation& proc = GetProcessingPresentation( GetProcessingIdentifier(name) );
	return proc.GetOutPortPresentation( GetLastIdentifier(name) );
}

ConnectionPointPresentation & NetworkPresentation::GetInPortPresentationByCompleteName(const std::string &name)
{	
	ProcessingPresentation& proc = GetProcessingPresentation( GetProcessingIdentifier(name) );
	return proc.GetInPortPresentation( GetLastIdentifier(name) );
}

ConnectionPointPresentation & NetworkPresentation::GetOutControlPresentationByCompleteName(const std::string & name)
{
	ProcessingPresentation& proc = GetProcessingPresentation( GetProcessingIdentifier(name) );
	return proc.GetOutControlPresentation( GetLastIdentifier(name) );
}

ConnectionPointPresentation & NetworkPresentation::GetInControlPresentationByCompleteName(const std::string &name)
{	
	ProcessingPresentation& proc = GetProcessingPresentation( GetProcessingIdentifier(name) );
	return proc.GetInControlPresentation( GetLastIdentifier(name) );
}

ProcessingPresentation& NetworkPresentation::GetProcessingPresentation( const std::string & name )
{
	ProcessingPresentationIterator it;
	for(it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
       		if ((*it)->GetName() ==  name)
				return **it;
	CLAM_ASSERT( false, "NetworkPresentation::GetProcessingPresentation. Object not found." );
}

std::string NetworkPresentation::GetProcessingIdentifier( const std::string& str )
{
	std::size_t length = PositionOfLastIdentifier(str)  - PositionOfProcessingIdentifier(str);
	return str.substr( PositionOfProcessingIdentifier(str), length);
}

char NetworkPresentation::NamesIdentifiersSeparator()
{ 	
	return '.'; 	
}

std::size_t NetworkPresentation::PositionOfLastIdentifier( const std::string & str ) 
{
	std::size_t result = str.find_last_of( NamesIdentifiersSeparator() );
	return result;
}
std::size_t NetworkPresentation::PositionOfProcessingIdentifier( const std::string& str )
{
	std::size_t endPos = PositionOfLastIdentifier(str)-1;
	std::size_t last_ofResult = str.find_last_of( NamesIdentifiersSeparator(), endPos );
	return last_ofResult == std::string::npos ? 0 : last_ofResult+1;
}

std::string NetworkPresentation::GetLastIdentifier( const std::string& str )
{
	return str.substr( PositionOfLastIdentifier(str)+1 );
}


void NetworkPresentation::CreateControlConnection( const std::string & out, const std::string & in )
{
	SignalCreateControlConnection.Emit( out, in ) ;
}

void NetworkPresentation::CreatePortConnection( const std::string & out, const std::string & in )
{
	SignalCreatePortConnection.Emit( out, in ) ;
}

void NetworkPresentation::AddProcessing( const std::string & name, CLAM::Processing * proc)
{
	SignalAddProcessing.Emit(name,proc);
}

void NetworkPresentation::Clear()
{

	ConnectionPresentationIterator itc;
	for(itc=mConnectionPresentations.begin(); itc!=mConnectionPresentations.end(); itc++)
		delete *itc;
	mConnectionPresentations.clear();
	mConnectionPresentationsToRemove.clear();
	
	ProcessingPresentationIterator it;
	for(it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
		delete *it;
	mProcessingPresentations.clear();
	mProcessingPresentationsToRemove.clear();
	
	SignalClear.Emit();
}



} // namespace NetworkGUI
