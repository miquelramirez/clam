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
#include "ConnectionAdapter.hxx"
#include "ProcessingPresentation.hxx"
#include "ConnectionPresentation.hxx"
#include "ConnectionPointPresentation.hxx"
#include "Processing.hxx"

namespace NetworkGUI
{

NetworkPresentation::NetworkPresentation()
	: mNetworkState( false )
{
//	SlotSetName.Wrap( this, &NetworkPresentation::SetName );
//	SlotSetProcessing.Wrap( this, &NetworkPresentation::SetProcessing );
	SlotSetPortConnection.Wrap( this, &NetworkPresentation::SetPortConnection );
	SlotSetControlConnection.Wrap( this, &NetworkPresentation::SetControlConnection );
	SlotSetRemovePortConnection.Wrap( this, &NetworkPresentation::SetRemovePortConnection );
	SlotSetRemoveControlConnection.Wrap( this, &NetworkPresentation::SetRemoveControlConnection );
	SlotRemoveProcessingPresentation.Wrap( this, &NetworkPresentation::RemoveProcessingPresentation );

	SlotCreateProcessingPresentation.Wrap( this, &NetworkPresentation::CreateProcessingPresentation );
	SlotAddProcessing.Wrap( this, &NetworkPresentation::AddProcessing );
	SlotRemoveProcessing.Wrap( this, &NetworkPresentation::RemoveProcessing );

	SlotChangeState.Wrap( this, &NetworkPresentation::ChangeState );
	SlotClear.Wrap(this, &NetworkPresentation::Clear );
}

void NetworkPresentation::ChangeState( bool newState )
{
	if (newState != mNetworkState)
	{
		mNetworkState = newState;
		SignalChangeState.Emit( mNetworkState );
	}
}

void NetworkPresentation::SetRemovePortConnection(  ConnectionPresentation * con)
{
	mConnectionPresentations.remove(con);
	con->Hide();

	SignalRemovePortConnectionFromGUI.Emit( con->GetOutName(), con->GetInName() );
}

void NetworkPresentation::SetRemoveControlConnection(  ConnectionPresentation * con)
{
	mConnectionPresentations.remove(con);
	con->Hide();

	SignalRemoveControlConnectionFromGUI.Emit( con->GetOutName(), con->GetInName() );
}

void NetworkPresentation::RemoveProcessingPresentation( const std::string & name )
{
	GetProcessingPresentation(name).Hide();
	ProcessingPresentation * proc = &(GetProcessingPresentation(name));
	std::list<ConnectionPresentation*> toRemove;
	
	ConnectionPresentationIterator it;
	for(it=mConnectionPresentations.begin(); it!=mConnectionPresentations.end(); it++)
	{
		const std::string & connection = (*it)->GetInName();
		if( GetProcessingIdentifier(connection) == proc->GetNameFromNetwork() )
		{
 			toRemove.push_back(*it);
		}
		else
		{
			const std::string & connection2 = (*it)->GetOutName();
			if( GetProcessingIdentifier(connection2) == proc->GetNameFromNetwork() )
			{
				toRemove.push_back(*it);
			}
		}
	}
	for(it=toRemove.begin(); it!=toRemove.end(); it++)
	{
		if (proc->HasInPort(GetLastIdentifier((*it)->GetInName())) || proc->HasOutPort(GetLastIdentifier((*it)->GetOutName())))
			SetRemovePortConnection( *it );
		else
			SetRemoveControlConnection(*it);
		    
	}
	mProcessingPresentations.remove( proc );

}

void NetworkPresentation::RemoveProcessing( ProcessingPresentation * proc)
{
	std::list<ConnectionPresentation*> toRemove;
	
	ConnectionPresentationIterator it;
	for(it=mConnectionPresentations.begin(); it!=mConnectionPresentations.end(); it++)
	{
		const std::string & connection = (*it)->GetInName();
		if( GetProcessingIdentifier(connection) == proc->GetNameFromNetwork() )
		{
 			toRemove.push_back(*it);
		}
		else
		{
			const std::string & connection2 = (*it)->GetOutName();
			if( GetProcessingIdentifier(connection2) == proc->GetNameFromNetwork() )
			{
				toRemove.push_back(*it);
			}
		}
	}
	for(it=toRemove.begin(); it!=toRemove.end(); it++)
	{
		if (proc->HasInPort(GetLastIdentifier((*it)->GetInName())) || proc->HasOutPort(GetLastIdentifier((*it)->GetOutName())))
			SetRemovePortConnection( *it );
		else
			SetRemoveControlConnection(*it);
		    
	}
//	mProcessingPresentations.remove( proc );
	SignalRemoveProcessing.Emit( proc->GetNameFromNetwork() );
}

NetworkPresentation::~NetworkPresentation()
{
	ProcessingPresentationIterator it;
	for(it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
		delete *it;
	ConnectionPresentationIterator itc;
	for(itc=mConnectionPresentations.begin(); itc!=mConnectionPresentations.end(); itc++)
		delete *itc;
}

void NetworkPresentation::AttachTo(CLAMVM::NetworkController & controller)
{
//	controller.SignalAcquireName.Connect( SlotSetName );

	SetName( controller.GetName() );
//	controller.SignalAcquireProcessing.Connect( SlotSetProcessing );
	controller.SignalCreateProcessingPresentation.Connect( SlotCreateProcessingPresentation );
	CLAMVM::NetworkController::ProcessingControllersMapIterator it;	
	for(it=controller.BeginProcessingControllers(); it!=controller.EndProcessingControllers(); it++ )
		CreateProcessingPresentation( it->first, it->second );
		
		
	controller.SignalAcquirePortConnection.Connect( SlotSetPortConnection );
	controller.SignalAcquireControlConnection.Connect( SlotSetControlConnection );
	SignalChangeState.Connect( controller.SlotChangeState );
	SignalAddProcessing.Connect( controller.SlotAddProcessing );
	
	SignalCreateNewPortConnectionFromGUI.Connect( controller.SlotCreateNewPortConnection );
	SignalCreateNewControlConnectionFromGUI.Connect( controller.SlotCreateNewControlConnection );
	SignalRemovePortConnectionFromGUI.Connect( controller.SlotRemovePortConnection );
	SignalRemoveControlConnectionFromGUI.Connect( controller.SlotRemoveControlConnection );
	SignalRemoveProcessing.Connect( controller.SlotRemoveProcessing );
	SignalClear.Connect( controller.SlotClear );
	SignalSaveNetworkTo.Connect( controller.SlotSaveNetwork );
	SignalLoadNetworkFrom.Connect( controller.SlotLoadNetwork );
	controller.SignalRemoveProcessingPresentation.Connect( SlotRemoveProcessingPresentation );
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
       		if ((*it)->GetNameFromNetwork() ==  name)
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

	SignalClear.Emit();
}



} // namespace NetworkGUI
