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
//	SlotProcessingControllerNeedsRebuild.Wrap( this, &NetworkController::ProcessingControllerNeedsRebuild );
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
	if ( mPortsToConnect.size() != 0)
	{
		ConnectionsMap::iterator it;
		for (it=mPortsToConnect.begin(); it!=mPortsToConnect.end(); it++)
			ExecuteCreatePortConnection( it->second, it->first );
		mPortsToConnect.clear();

		for (it=mControlsToConnect.begin(); it!=mControlsToConnect.end(); it++)
			ExecuteCreateControlConnection( it->second, it->first );
		mControlsToConnect.clear();
	}
	if ( mPortsToDisconnect.size() != 0)
	{
		ConnectionsMap::iterator it;
		for (it=mPortsToDisconnect.begin(); it!=mPortsToDisconnect.end(); it++)
			ExecuteRemovePortConnection( it->second, it->first );
		mPortsToDisconnect.clear();		

		for (it=mControlsToDisconnect.begin(); it!=mControlsToDisconnect.end(); it++)
			ExecuteRemoveControlConnection( it->second, it->first );
		mControlsToDisconnect.clear();		
	}
	if( mProcessingsToRemove.size() != 0)
	{
		ProcessingsList::iterator it;
		for(it=mProcessingsToRemove.begin(); it!=mProcessingsToRemove.end(); it++)
			ExecuteRemoveProcessing( *it );
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
	ProcessingControllersMapIterator it;
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
	
	ProcessingControllersMapIterator it = mProcessingControllers.find( oldName );
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
		ProcessingControllersMapIterator it;
		for( it=mProcessingControllers.begin(); it!=mProcessingControllers.end(); it++ )
			it->second->SignalChangeState.Emit( it->second->GetProcessingExecState(), it->second->GetProcessingStatus() );	
	}
	else // stop the network
	{		
		if(!mThread.IsRunning())
			return;
		
		mLoopCondition = false;
		mThread.Stop();
		mObserved->Stop();

		ProcessingControllersMapIterator it;
		for( it=mProcessingControllers.begin(); it!=mProcessingControllers.end(); it++ )
			it->second->SignalChangeState.Emit( it->second->GetProcessingExecState(), it->second->GetProcessingStatus() );	

	}
}

void NetworkController::CreatePortConnection( const std::string & out, const std::string& in)
{	
	if (mLoopCondition)
		mPortsToConnect.insert( ConnectionsMap::value_type( in, out ) );
	else
		ExecuteCreatePortConnection(out, in);
}

void NetworkController::CreateControlConnection( const std::string & out, const std::string& in)
{	
	if (mLoopCondition)
		mControlsToConnect.insert( ConnectionsMap::value_type( in, out ) );
	else
		ExecuteCreateControlConnection(out, in);
}

void NetworkController::ExecuteCreatePortConnection( const std::string & out , const std::string & in )
{
	if(mObserved->ConnectPorts(out, in))
	{
		ConnectionAdapter * connection = CreatePortConnectionAdapter( mObserved->GetOutPortByCompleteName(out), mObserved->GetInPortByCompleteName(in)); 
		SignalCreatePortConnectionPresentation.Emit( (ConnectionAdapter*)connection );
	}
}

ConnectionAdapter * NetworkController::CreatePortConnectionAdapter( const CLAM::OutPort & out, const CLAM::InPort & in )
{
	ConnectionAdapterTmpl< CLAM::OutPort, CLAM::InPort> * adapter = new ConnectionAdapterTmpl< CLAM::OutPort, CLAM::InPort >;
	adapter->BindTo( out, in, (const CLAM::Network&)*mObserved );
	mConnectionAdapters.push_back( (ConnectionAdapter*)adapter );
	return adapter;
}

void NetworkController::ExecuteCreateControlConnection( const std::string & out , const std::string & in )
{
	if(mObserved->ConnectControls(out, in))
	{	
		ConnectionAdapter * connection = CreateControlConnectionAdapter( mObserved->GetOutControlByCompleteName(out), mObserved->GetInControlByCompleteName(in)); 
		SignalCreateControlConnectionPresentation.Emit( (ConnectionAdapter*)connection );
	}
}

ConnectionAdapter * NetworkController::CreateControlConnectionAdapter( const CLAM::OutControl & out, const CLAM::InControl & in )
{
	ConnectionAdapterTmpl< CLAM::OutControl, CLAM::InControl> * adapter = new ConnectionAdapterTmpl< CLAM::OutControl, CLAM::InControl >;
	adapter->BindTo( out, in, (const CLAM::Network&)*mObserved );
	mConnectionAdapters.push_back( (ConnectionAdapter*)adapter );
	return adapter;
}

void NetworkController::RemovePortConnection( const std::string & outPort, 
						       const std::string & inPort )
{
	if (mLoopCondition)
	{
		if (!mPortsToDisconnect.insert( ConnectionsMap::value_type( inPort, outPort ) ).second )
			CLAM_ASSERT(false, "NetworkController::RemovePortConnection() Trying to remove connection with repeated key" );
	}
	else
	{
		ExecuteRemovePortConnection(outPort, inPort);
	}

}


void NetworkController::RemoveControlConnection( const std::string & outControl, 
						       const std::string & inControl )
{
	if (mLoopCondition)
	{
		if (!mControlsToDisconnect.insert( ConnectionsMap::value_type( inControl, outControl ) ).second )
			CLAM_ASSERT(false, "NetworkController::RemoveControlConnection() Trying to remove connection with repeated key" );
	}
	else
	{
		ExecuteRemoveControlConnection(outControl, inControl);
	}

}

void NetworkController::LoadNetwork( const std::string & file)
{
	CLAM::XMLStorage storage;
	storage.Restore( *mObserved, file );

	BindTo( *mObserved );

	ProcessingControllersMapIterator itp;
	for (itp=BeginProcessingControllers(); itp!=EndProcessingControllers(); itp++)
	{
		SignalCreateProcessingPresentation.Emit( itp->first, itp->second);
	}

	CLAM::Network::ProcessingsMap::const_iterator it;
	for (it=mObserved->BeginProcessings(); it!=mObserved->EndProcessings(); it++)
	{
		CLAM::Processing * producer = it->second;
		CLAM::PublishedOutPorts::Iterator itOutPort;
	
		for (itOutPort=producer->GetOutPorts().Begin(); itOutPort!=producer->GetOutPorts().End(); itOutPort++)
		{	
			if (!(*itOutPort)->GetNode())
				break;

			std::string completeOutName( it->first );
			completeOutName += ".";
			completeOutName += (*itOutPort)->GetName();

			CLAM::Network::NamesList connected = mObserved->GetInPortsConnectedTo( completeOutName );
			CLAM::Network::NamesList::iterator namesIn;
			for(namesIn=connected.begin(); namesIn!=connected.end(); namesIn++)
			{
				ConnectionAdapter * connection = CreatePortConnectionAdapter( mObserved->GetOutPortByCompleteName(completeOutName), 
												 mObserved->GetInPortByCompleteName(*namesIn)); 
				SignalCreatePortConnectionPresentation.Emit( (ConnectionAdapter*)connection );
			}

		}

		CLAM::PublishedOutControls::Iterator itOutControl;
	
		for (itOutControl=producer->GetOutControls().Begin(); itOutControl!=producer->GetOutControls().End(); itOutControl++)
		{	
			if(!((*itOutControl)->IsConnected()))
				break;

			std::string completeOutName( it->first );
			completeOutName += ".";
			completeOutName += (*itOutControl)->GetName();

			CLAM::Network::NamesList connected = mObserved->GetInControlsConnectedTo( completeOutName );
			CLAM::Network::NamesList::iterator namesIn;
			for(namesIn=connected.begin(); namesIn!=connected.end(); namesIn++)
			{
				ConnectionAdapter * connection = CreateControlConnectionAdapter( mObserved->GetOutControlByCompleteName(completeOutName), 
												 mObserved->GetInControlByCompleteName(*namesIn)); 
				SignalCreateControlConnectionPresentation.Emit( (ConnectionAdapter*)connection );
			}

		}

	}
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
	ProcessingControllersMapIterator it = mProcessingControllers.find( name );
	ProcessingController * proc = it->second;


	ProcessingController::NamesList::iterator namesIt;	
	for(namesIt=proc->BeginOutPortNames(); namesIt!=proc->EndOutPortNames(); namesIt++)
	{
		std::string completeOutName("");
		completeOutName += name;
		completeOutName += ".";
		completeOutName += *namesIt;
		
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
		std::string completeInName("");
		completeInName += name;
		completeInName += ".";
		completeInName += *namesIt;

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
	ProcessingControllersMapIterator it = mProcessingControllers.find( name );
	ProcessingController * proc = it->second;

	ProcessingController::NamesList::iterator namesIt;	
	for(namesIt=proc->BeginOutControlNames(); namesIt!=proc->EndOutControlNames(); namesIt++)
	{	
		std::string completeOutName("");
		completeOutName += name;
		completeOutName += ".";
		completeOutName += *namesIt;
		
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
		std::string completeInName("");
		completeInName += name;
		completeInName += ".";
		completeInName += *namesIt;


		// in controls should have references to its connected out controls
		std::list< ConnectionAdapter* > connectionsToRemove;
		ConnectionAdapterIterator connectionIt;
		for( connectionIt=mConnectionAdapters.begin(); connectionIt!=mConnectionAdapters.end(); connectionIt++)
		{
			if( (*connectionIt)->GetInName() == completeInName )
			{	
				connectionsToRemove.push_back( *connectionIt );
			}
		}
		
		for( connectionIt=connectionsToRemove.begin(); connectionIt!=connectionsToRemove.end(); connectionIt++)
		{
			std::string completeOutName("");
			completeOutName += (*connectionIt)->GetOutName();
			RemoveControlConnection( completeOutName, completeInName );
			SignalRemoveConnectionPresentation.Emit( completeOutName, completeInName); 

		}
	}
}

void NetworkController::ExecuteRemoveProcessing( const std::string & name )
{

	ProcessingControllersMapIterator it = mProcessingControllers.find( name );
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

	// remove all connections to processing and communicate it to gui
	RemoveAllPortConnections( name );
	RemoveAllControlConnections( name );
}

void NetworkController::RebuildProcessingPresentationAttachedTo( ProcessingController * controller, CLAM::Processing * proc )
{
	std::string name = mObserved->GetNetworkId( proc );

	// emit signal to delete processing presentation 
	SignalRebuildProcessingPresentationAttachedTo.Emit( name, controller );
}

void NetworkController::ExecuteRemovePortConnection( const std::string & out , const std::string & in )
{
	if(mObserved->DisconnectPorts(out, in))
	{
		//remove connection from inport
		CLAM::InPort & inPort = mObserved->GetInPortByCompleteName(in);
		ConnectionAdapterIterator itc;
		for ( itc=mConnectionAdapters.begin(); itc!=mConnectionAdapters.end(); itc++)
		{
			ConnectionAdapterTmpl<CLAM::OutPort, CLAM::InPort> * con = (ConnectionAdapterTmpl<CLAM::OutPort, CLAM::InPort>*)(*itc);
			if (con->ConnectsInElement(inPort))
			{
				mConnectionAdapters.remove(con);
				delete con;
				return;   
			}
		}
	}
}

void NetworkController::ExecuteRemoveControlConnection( const std::string & out , const std::string & in )
{
	if(mObserved->DisconnectControls(out, in))
	{
		//remove connection from inport
		CLAM::InControl & inControl = mObserved->GetInControlByCompleteName(in);
		ConnectionAdapterIterator itc;
		for ( itc=mConnectionAdapters.begin(); itc!=mConnectionAdapters.end(); itc++)
		{
			ConnectionAdapterTmpl<CLAM::OutControl, CLAM::InControl> * con = (ConnectionAdapterTmpl<CLAM::OutControl, CLAM::InControl>*)(*itc);
			if (con && con->ConnectsInElement(inControl))
			{				
				mConnectionAdapters.erase( itc );
				delete con;
				return;   
			}
		}
	}
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
	ProcessingControllersMapIterator i = mProcessingControllers.find( name );
	if(i!=mProcessingControllers.end())
		CLAM_ASSERT(false, "NetworkController::CreateProcessingController() Trying to add a processing controller with a repeated name (key)" );

	ProcessingController* controller = new ProcessingController;
	controller->SignalProcessingNameChanged.Connect( SlotProcessingNameChanged );
	controller->SignalRemoveAllConnections.Connect( SlotRemoveAllConnections );
	controller->SignalRebuildProcessingPresentationAttachedTo.Connect( SlotRebuildProcessingPresentationAttachedTo );

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
	{
		CLAM::Processing * producer = it->second;
		CreateProcessingController( it->first,  it->second );
	}
	return true;
}

bool NetworkController::Update()
{
	return true;
}

void NetworkController::Clear()
{

	if(mThread.IsRunning())
	{
		mLoopCondition = false;
		mThread.Stop();
	}

	ProcessingControllersMapIterator it;
	for(it=mProcessingControllers.begin(); it!=mProcessingControllers.end(); it++)
	{
		CLAMVM::ProcessingController * controller = it->second;
		delete controller;
	}

	mProcessingControllers.clear();

	ConnectionAdapterIterator itc;
	for ( itc=mConnectionAdapters.begin(); itc!=mConnectionAdapters.end(); itc++)
		delete *itc;
	mConnectionAdapters.clear();

	if(mObserved)
		mObserved->Clear();
}

} // namespace CLAMVM
