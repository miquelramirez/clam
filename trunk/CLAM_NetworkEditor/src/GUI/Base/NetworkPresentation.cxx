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
#include "NetworkModel.hxx"
#include "ProcessingController.hxx"
#include "ConnectionAdapter.hxx"
#include "ProcessingPresentation.hxx"
#include "ConnectionPresentation.hxx"
#include "OutPortPresentation.hxx"
#include "InPortPresentation.hxx"
#include "OutControlPresentation.hxx"
#include "InControlPresentation.hxx"
#include "Processing.hxx"

namespace NetworkGUI
{

NetworkPresentation::NetworkPresentation()
	: mNetworkState( false )
{
	SetName.Wrap( this, &NetworkPresentation::OnNewName );
	SetProcessing.Wrap( this, &NetworkPresentation::OnNewProcessing );
	SetPortConnection.Wrap( this, &NetworkPresentation::OnNewPortConnection );
	SetControlConnection.Wrap( this, &NetworkPresentation::OnNewControlConnection );
	SetRemovePortConnection.Wrap( this, &NetworkPresentation::OnRemovePortConnection );
	SetRemoveControlConnection.Wrap( this, &NetworkPresentation::OnRemoveControlConnection );
	SetRemoveProcessing.Wrap( this, &NetworkPresentation::OnRemoveProcessing );
	AddNewProcessing.Wrap( this, &NetworkPresentation::OnAddNewProcessing );
	ChangeState.Wrap( this, &NetworkPresentation::OnNewChangeState );
	Clear.Wrap(this, &NetworkPresentation::OnClear );
}

void NetworkPresentation::OnNewChangeState( bool newState )
{
	if (newState != mNetworkState)
	{
		mNetworkState = newState;
		SChangeState.Emit( mNetworkState );
	}
}

void NetworkPresentation::OnRemovePortConnection(  ConnectionPresentation * con)
{
	mConnectionPresentations.remove(con);
	con->Hide();

	RemovePortConnectionFromGUI.Emit( con->GetOutName(), con->GetInName() );
}

void NetworkPresentation::OnRemoveControlConnection(  ConnectionPresentation * con)
{
	mConnectionPresentations.remove(con);
	con->Hide();

	RemoveControlConnectionFromGUI.Emit( con->GetOutName(), con->GetInName() );
}

void NetworkPresentation::OnRemoveProcessing( ProcessingPresentation * proc)
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
			OnRemovePortConnection( *it );
		else
			OnRemoveControlConnection(*it);
		    
	}
	mProcessingPresentations.remove( proc );
	RemoveProcessingFromGUI.Emit( proc->GetNameFromNetwork() );
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

void NetworkPresentation::AttachTo(CLAMVM::NetworkModel & model)
{
	model.AcquireName.Connect( SetName );
	model.AcquireProcessing.Connect( SetProcessing );
	model.AcquirePortConnection.Connect( SetPortConnection );
	model.AcquireControlConnection.Connect( SetControlConnection );
	SChangeState.Connect( model.ChangeState );
	AddProcessing.Connect( model.AddNewProcessing );
	
	CreateNewPortConnectionFromGUI.Connect( model.CreateNewPortConnection );
	CreateNewControlConnectionFromGUI.Connect( model.CreateNewControlConnection );
	RemovePortConnectionFromGUI.Connect( model.RemovePortConnection );
	RemoveControlConnectionFromGUI.Connect( model.RemoveControlConnection );
	RemoveProcessingFromGUI.Connect( model.RemoveProcessing );
	ClearSignal.Connect( model.Clear );
	SaveNetworkTo.Connect( model.SaveNetwork );
	LoadNetworkFrom.Connect( model.LoadNetwork );
}


OutPortPresentation & NetworkPresentation::GetOutPortPresentationByCompleteName(const std::string & name)
{
	ProcessingPresentation& proc = GetProcessingPresentation( GetProcessingIdentifier(name) );
	return proc.GetOutPortPresentation( GetLastIdentifier(name) );
}

InPortPresentation & NetworkPresentation::GetInPortPresentationByCompleteName(const std::string &name)
{	
	ProcessingPresentation& proc = GetProcessingPresentation( GetProcessingIdentifier(name) );
	return proc.GetInPortPresentation( GetLastIdentifier(name) );
}

OutControlPresentation & NetworkPresentation::GetOutControlPresentationByCompleteName(const std::string & name)
{
	ProcessingPresentation& proc = GetProcessingPresentation( GetProcessingIdentifier(name) );
	return proc.GetOutControlPresentation( GetLastIdentifier(name) );
}

InControlPresentation & NetworkPresentation::GetInControlPresentationByCompleteName(const std::string &name)
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

void NetworkPresentation::OnAddNewProcessing( const std::string & name, CLAM::Processing * proc)
{
	AddProcessing.Emit(name,proc);
}

void NetworkPresentation::OnClear()
{
	ProcessingPresentationIterator it;
	for(it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
		delete *it;
	mProcessingPresentations.clear();

	ConnectionPresentationIterator itc;
	for(itc=mConnectionPresentations.begin(); itc!=mConnectionPresentations.end(); itc++)
		delete *itc;
	mConnectionPresentations.clear();

	ClearSignal.Emit();
}



} // namespace NetworkGUI
