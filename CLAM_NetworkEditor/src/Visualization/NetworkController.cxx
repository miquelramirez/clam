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

#include "NetworkController.hxx"
#include "Network.hxx"
#include "Node.hxx"
#include "InPort.hxx"
#include "Processing.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "Processing.hxx"
#include "OutControl.hxx"
#include "XMLStorage.hxx"

namespace CLAMVM
{
	
NetworkController::NetworkController()
	: mObserved(0),
	  mLoopCondition(false),
	  mThread( true ) // realtime
{
	SlotCreatePortConnection.Wrap( this, &NetworkController::CreatePortConnection );
	SlotRemovePortConnection.Wrap( this, &NetworkController::RemovePortConnection );
	
	SlotCreateControlConnection.Wrap( this, &NetworkController::CreateControlConnection );
	SlotRemoveControlConnection.Wrap( this, &NetworkController::RemoveControlConnection );
	
	SlotRemoveProcessing.Wrap( this, &NetworkController::RemoveProcessing );
	SlotConfigureProcessing.Wrap( this, &NetworkController::ConfigureProcessing );

	SlotRebuildProcessingPresentationAttachedTo.Wrap( this, &NetworkController::RebuildProcessingPresentationAttachedTo );
	SlotRemoveAllConnections.Wrap( this, &NetworkController::RemoveAllConnections );
	SlotAddProcessing.Wrap( this, &NetworkController::AddProcessing );
	SlotProcessingNameChanged.Wrap( this, &NetworkController::ProcessingNameChanged );
	
	SlotChangeState.Wrap( this, &NetworkController::ChangeState );
	SlotSaveNetwork.Wrap( this, &NetworkController::SaveNetwork );
	SlotLoadNetwork.Wrap( this, &NetworkController::LoadNetwork );
	SlotClear.Wrap( this, &NetworkController::Clear );


	mThread.SetThreadCode( makeMemberFunctor0( *this, NetworkController, ProcessingLoop ) );
	mThread.SetupPriorityPolicy();
}

void NetworkController::ExecuteEvents()
{	
	ConnectionsList::iterator it;
	if ( mPortsToConnect.size() != 0)
	{
		for (it=mPortsToConnect.begin(); it!=mPortsToConnect.end(); it++)
			ExecuteCreatePortConnection( (*it).GetOut(), (*it).GetIn() );
		mPortsToConnect.clear();
	}
	
	if( mControlsToConnect.size() != 0 )
	{
		for (it=mControlsToConnect.begin(); it!=mControlsToConnect.end(); it++)
			ExecuteCreateControlConnection( (*it).GetOut(), (*it).GetIn() );
		mControlsToConnect.clear();
	}
	
	if ( mPortsToDisconnect.size() != 0)
	{
		for (it=mPortsToDisconnect.begin(); it!=mPortsToDisconnect.end(); it++)
			ExecuteRemovePortConnection( (*it).GetOut(), (*it).GetIn() );
		mPortsToDisconnect.clear();		
	}
	
	if( mControlsToDisconnect.size() != 0 )
	{

		for (it=mControlsToDisconnect.begin(); it!=mControlsToDisconnect.end(); it++)
			ExecuteRemoveControlConnection( (*it).GetOut(), (*it).GetIn() );
		mControlsToDisconnect.clear();		
	}
	if( mProcessingsToRemove.size() != 0)
	{
		ProcessingsList::iterator itp;
		for(itp=mProcessingsToRemove.begin(); itp!=mProcessingsToRemove.end(); itp++)
			ExecuteRemoveProcessing( *itp );
		mProcessingsToRemove.clear();
	}
}

void NetworkController::ProcessingLoop()
{
	while(mLoopCondition)
	{
		ExecuteEvents();
		mObserved->DoProcessings();
	}
}

void NetworkController::ProcessingNameChanged( const std::string & newName, ProcessingController * controller )
{
	std::string oldName("");
	ProcessingControllersMap::iterator it;
	for( it=mProcessingControllers.begin(); it!=mProcessingControllers.end(); it++ )
	{
		if( it->second == controller )
		{
			oldName = it->first;
			break;
		}
	}

	if(!ChangeKeyMap( oldName, newName )) // it should not modify anything
	{
		controller->SetName( oldName );
		return;
	}

	// change key map in network
	mObserved->ChangeKeyMap( oldName, newName );
	SignalChangeConnectionPresentationNames.Emit( oldName, newName );
}

bool NetworkController::ChangeKeyMap( const std::string & oldName, const std::string & newName )
{
	if( mProcessingControllers.find( newName ) != mProcessingControllers.end() ) // newName is being used
		return false;
	
	ProcessingControllersMap::iterator it = mProcessingControllers.find( oldName );
	ProcessingController * controller = it->second;
	mProcessingControllers.erase( it );
	mProcessingControllers.insert( ProcessingControllersMap::value_type( newName, controller ) );
	return true;
}

void NetworkController::ChangeState( bool state)
{
	if (state) // start the network
	{
		if(mThread.IsRunning())
			return;

		mObserved->Start();
		mLoopCondition = true;
		
		mThread.Start();
		ProcessingControllersMap::iterator it;
		for( it=mProcessingControllers.begin(); it!=mProcessingControllers.end(); it++ )
			it->second->SignalChangeState.Emit( it->second->GetProcessingExecState(), 
						            it->second->GetProcessingStatus() );	
	}
	else // stop the network
	{		
		if(!mThread.IsRunning())
			return;
		
		mLoopCondition = false;
		mThread.Stop();
		mObserved->Stop();

		ProcessingControllersMap::iterator it;
		for( it=mProcessingControllers.begin(); it!=mProcessingControllers.end(); it++ )
			it->second->SignalChangeState.Emit( it->second->GetProcessingExecState(), 
							    it->second->GetProcessingStatus() );	

	}
}

void NetworkController::CreatePortConnection( const std::string & out, const std::string& in)
{	
	if (mLoopCondition)
		mPortsToConnect.push_back( Connection( out, in ));
	else
		ExecuteCreatePortConnection(out, in);
}

void NetworkController::CreateControlConnection( const std::string & out, const std::string& in)
{	
	if (mLoopCondition)
		mControlsToConnect.push_back( Connection( out, in ));
	else
		ExecuteCreateControlConnection(out, in);
}

void NetworkController::ExecuteCreatePortConnection( const std::string & out , const std::string & in )
{
	if(mObserved->ConnectPorts(out, in))
		RegisterPortConnection( out, in );
}

void NetworkController::RegisterPortConnection( const std::string & out, const std::string & in )
{
	mPortConnections.push_back( Connection( out, in ));
	SignalCreatePortConnectionPresentation.Emit( out, in );
}

void NetworkController::ExecuteCreateControlConnection( const std::string & out , const std::string & in )
{	
	if(mObserved->ConnectControls(out, in))
		RegisterControlConnection( out, in );
}

void NetworkController::RegisterControlConnection( const std::string & out , const std::string & in )
{	
	mControlConnections.push_back( Connection( out, in ));
	SignalCreateControlConnectionPresentation.Emit( out, in );
}

void NetworkController::RemovePortConnection( const std::string & outPort, const std::string & inPort )
{
	if (mLoopCondition)
		mPortsToDisconnect.push_back( Connection( outPort, inPort ));
	else
		ExecuteRemovePortConnection(outPort, inPort);
}


void NetworkController::RemoveControlConnection( const std::string & outControl, const std::string & inControl )
{
	if (mLoopCondition)
		mControlsToDisconnect.push_back( Connection( outControl, inControl ));
	else
		ExecuteRemoveControlConnection(outControl, inControl);
}

void NetworkController::LoadNetwork( const std::string & file)
{
	Clear();
	SignalClearPresentation.Emit();
	CLAM::XMLStorage storage;
	storage.Restore( *mObserved, file );

	BindTo( *mObserved );

	Publish();
}

void NetworkController::SaveNetwork( const std::string & file)
{
	CLAM::XMLStorage::Dump( *mObserved, "network", file );
}

void NetworkController::RemoveProcessing(const std::string & name )
{
	if (mLoopCondition)
		mProcessingsToRemove.push_back( name );
	else
		ExecuteRemoveProcessing( name );
}

void NetworkController::RemoveAllPortConnections( const std::string & name )
{	
	ProcessingControllersMap::iterator it = mProcessingControllers.find( name );
	ProcessingController * proc = it->second;
	
	ProcessingController::NamesList::iterator namesIt;	
	for(namesIt=proc->BeginOutPortNames(); namesIt!=proc->EndOutPortNames(); namesIt++)
	{
		std::string completeOutName( name + "." + *namesIt );
		CLAM::Network::NamesList connected = mObserved->GetInPortsConnectedTo( completeOutName );
		CLAM::Network::NamesList::iterator namesIn;
		for(namesIn=connected.begin(); namesIn!=connected.end(); namesIn++)
		{
			RemovePortConnection( completeOutName, *namesIn );
			SignalRemoveConnectionPresentation.Emit( completeOutName, *namesIn );	
		}
	}
	for(namesIt=proc->BeginInPortNames(); namesIt!=proc->EndInPortNames(); namesIt++)
	{
		std::string completeInName( name + "." + *namesIt );
		CLAM::InPort & inPort = mObserved->GetInPortByCompleteName( completeInName );
		if(inPort.IsAttached())
		{
			std::string outName("");
			outName += mObserved->GetNetworkId( inPort.GetNode()->GetWriter()->GetProcessing() );
			outName += ".";
			outName += inPort.GetNode()->GetWriter()->GetName();
			
			RemovePortConnection( outName, completeInName );
			SignalRemoveConnectionPresentation.Emit( outName, completeInName );	
		}

	}	
}

void NetworkController::RemoveAllControlConnections( const std::string & name )
{
	ProcessingControllersMap::iterator it = mProcessingControllers.find( name );
	ProcessingController * proc = it->second;

	ProcessingController::NamesList::iterator namesIt;	
	for(namesIt=proc->BeginOutControlNames(); namesIt!=proc->EndOutControlNames(); namesIt++)
	{	
		std::string completeOutName( name + "." + *namesIt );
		
		CLAM::Network::NamesList connected = mObserved->GetInControlsConnectedTo( completeOutName );
		CLAM::Network::NamesList::iterator namesIn;
		for(namesIn=connected.begin(); namesIn!=connected.end(); namesIn++)
		{
			RemoveControlConnection( completeOutName, *namesIn );
			SignalRemoveConnectionPresentation.Emit( completeOutName, *namesIn );	
		}
	}

	for(namesIt=proc->BeginInControlNames(); namesIt!=proc->EndInControlNames(); namesIt++)
	{	
		std::string completeInName( name + "." + *namesIt);

		ConnectionsList connectionsToRemove;
		ConnectionsList::iterator connectionIt;
		for( connectionIt=mControlConnections.begin(); connectionIt!=mControlConnections.end(); connectionIt++)
			if( ((*connectionIt).GetIn()) == completeInName )
				connectionsToRemove.push_back( *connectionIt );
		
		for( connectionIt=connectionsToRemove.begin(); connectionIt!=connectionsToRemove.end(); connectionIt++)
		{
			RemoveControlConnection( (*connectionIt).GetOut(), (*connectionIt).GetIn() );
			SignalRemoveConnectionPresentation.Emit( (*connectionIt).GetOut(), (*connectionIt).GetIn() ); 
		}
	}
}

void NetworkController::ExecuteRemoveProcessing( const std::string & name )
{

	ProcessingControllersMap::iterator it = mProcessingControllers.find( name );
	if(it==mProcessingControllers.end())
			CLAM_ASSERT(false, "NetworkControllers::ExecuteRemoveProcessing() Trying to remove a processing controller that is not included in the network controller" );

	ProcessingController * proc = it->second;

	RemoveAllPortConnections( name );
	RemoveAllControlConnections( name );

	mProcessingControllers.erase( name );
	mObserved->RemoveProcessing( name );
	delete proc;
}


void NetworkController::RemoveAllConnections(  CLAM::Processing * proc )
{
	std::string name = mObserved->GetNetworkId( proc );
	RemoveAllPortConnections( name );
	RemoveAllControlConnections( name );
}

void NetworkController::RebuildProcessingPresentationAttachedTo( ProcessingController * controller, CLAM::Processing * proc )
{
	SignalRebuildProcessingPresentationAttachedTo.Emit(  mObserved->GetNetworkId( proc ), controller );
}

void NetworkController::ConfigureProcessing( CLAM::Processing * proc, const CLAM::ProcessingConfig & cfg )
{
	mObserved->ConfigureProcessing( mObserved->GetNetworkId( proc ), cfg );
}

void NetworkController::ExecuteRemovePortConnection( const std::string & out , const std::string & in )
{
	if(mObserved->DisconnectPorts(out, in))
	{
		ConnectionsList::iterator it;
		for(it=mPortConnections.begin(); it!=mPortConnections.end(); it++)
			if( ((*it).GetIn() == in) && ((*it).GetOut()== out) )
			{
				mPortConnections.erase(it);
				return;
			}
	}
			
	CLAM_ASSERT(false, "NetworkController::ExecuteRemovePortConnection()"
			"Trying to remove a port connection that has not been created" );
}

void NetworkController::ExecuteRemoveControlConnection( const std::string & out , const std::string & in )
{	
	if(mObserved->DisconnectControls(out, in))
	{
		ConnectionsList::iterator it;
		for(it=mControlConnections.begin(); it!=mControlConnections.end(); it++)
			if( ((*it).GetIn() == in) && ((*it).GetOut()== out) )
			{
				mControlConnections.erase(it);
				return;
			}
	}
			
	CLAM_ASSERT(false, "NetworkController::ExecuteRemoveControlConnection()"
			"Trying to remove a port connection that has not been created" );	
}

NetworkController::~NetworkController()
{
	Clear();
}

void NetworkController::AddProcessing( const std::string & name, 
					      CLAM::Processing * proc )
{
	mObserved->AddProcessing(name, proc);
	SignalCreateProcessingPresentation.Emit( name, CreateProcessingController(name, proc));
}

ProcessingController* NetworkController::CreateProcessingController( const std::string & name, CLAM::Processing * proc )
{
	ProcessingControllersMap::iterator i = mProcessingControllers.find( name );
	if(i!=mProcessingControllers.end())
		CLAM_ASSERT(false, "NetworkController::CreateProcessingController() Trying to add a processing controller with a repeated name (key)" );

	ProcessingController* controller = new ProcessingController;
	controller->SignalProcessingNameChanged.Connect( SlotProcessingNameChanged );
	controller->SignalRemoveAllConnections.Connect( SlotRemoveAllConnections );
	controller->SignalRebuildProcessingPresentationAttachedTo.Connect( SlotRebuildProcessingPresentationAttachedTo );
	controller->SignalConfigureProcessing.Connect( SlotConfigureProcessing );

	controller->BindTo(*proc);
	mProcessingControllers.insert( ProcessingControllersMap::value_type( name, controller));
	return controller;
}

std::string NetworkController::GetName()
{
	if(!mObserved)
		return "network controller unbinded";
	return mObserved->GetName();
}

bool NetworkController::Publish()
{
	CLAM_ASSERT(mObserved, "Trying to publish an unbinded network controller" );
	return true;
}

bool NetworkController::BindTo( CLAM::Network& obj )
{
	mObserved = dynamic_cast< CLAM::Network* > (&obj);
	
	if( !mObserved )
		return false;

	CLAM::Network::ProcessingsMap::const_iterator it;
	for (it=mObserved->BeginProcessings(); it!=mObserved->EndProcessings(); it++)
		CreateProcessingController( it->first,  it->second );

	ProcessingControllersMap::iterator itp;
	for (itp=BeginProcessingControllers(); itp!=EndProcessingControllers(); itp++)
		SignalCreateProcessingPresentation.Emit( itp->first, itp->second);

	for (it=mObserved->BeginProcessings(); it!=mObserved->EndProcessings(); it++)
	{
		CLAM::Processing * producer = it->second;
		CLAM::PublishedOutPorts::Iterator itOutPort;
		for (itOutPort=producer->GetOutPorts().Begin(); itOutPort!=producer->GetOutPorts().End(); itOutPort++)
		{	
			if ((*itOutPort)->GetNode())
			{
				std::string completeOutName( it->first + "." + (*itOutPort)->GetName() );

				CLAM::Network::NamesList connected = mObserved->GetInPortsConnectedTo( completeOutName );
				CLAM::Network::NamesList::iterator namesIn;
				for(namesIn=connected.begin(); namesIn!=connected.end(); namesIn++)
					RegisterPortConnection( completeOutName, *namesIn);
			}
		}

		CLAM::PublishedOutControls::Iterator itOutControl;
		for (itOutControl=producer->GetOutControls().Begin(); itOutControl!=producer->GetOutControls().End(); itOutControl++)
		{	
			if((*itOutControl)->IsConnected())
			{
				std::string completeOutName( it->first + "." + (*itOutControl)->GetName());

				CLAM::Network::NamesList connected = mObserved->GetInControlsConnectedTo( completeOutName );
				CLAM::Network::NamesList::iterator namesIn;
				for(namesIn=connected.begin(); namesIn!=connected.end(); namesIn++)
					RegisterControlConnection( completeOutName, *namesIn);
			}
		}
	}
	return true;
}

bool NetworkController::Update()
{
	return true;
}

void NetworkController::Clear()
{
	ChangeState( false );
	ProcessingControllersMap::iterator it;
	for(it=mProcessingControllers.begin(); it!=mProcessingControllers.end(); it++)
		delete it->second;
	mProcessingControllers.clear();

	mControlConnections.clear();
	mPortConnections.clear();

	if(mObserved)
		mObserved->Clear();
}

} // namespace CLAMVM

