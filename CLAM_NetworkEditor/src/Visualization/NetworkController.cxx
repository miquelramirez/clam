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

#include "MIDIManager.hxx"

#include "NetworkController.hxx"
#include "Network.hxx"
#include "InPort.hxx"
#include "Processing.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "Processing.hxx"
#include "OutControl.hxx"
#include "XMLStorage.hxx"
#include "NetworkPresentation.hxx"

namespace CLAMVM
{
	
void NetworkController::AttachToNetworkPresentation( NetworkGUI::NetworkPresentation * netpresentation)
{
	mPresentation = netpresentation;
}


NetworkController::NetworkController()
{
	mObserved=0;
	mLoopCondition=false;
	mThread=true; // realtime
	mPresentation=0;
		
	SlotCreatePortConnection.Wrap( this, &NetworkController::CreatePortConnection );
	SlotRemovePortConnection.Wrap( this, &NetworkController::RemovePortConnection );
	
	SlotCreateControlConnection.Wrap( this, &NetworkController::CreateControlConnection );
	
	SlotRemoveProcessing.Wrap( this, &NetworkController::RemoveProcessing );
	SlotConfigureProcessing.Wrap( this, &NetworkController::ConfigureProcessing );

	SlotRebuildProcessingPresentationAttachedTo.Wrap( this, &NetworkController::RebuildProcessingPresentationAttachedTo );
	SlotRemoveAllConnections.Wrap( this, &NetworkController::RemoveAllConnections );
	SlotProcessingNameChanged.Wrap( this, &NetworkController::ProcessingNameChanged );
	
	SlotChangeState.Wrap( this, &NetworkController::ChangeState );
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
	CLAM::MIDIManager::Current().Start();	// this is a provisional hack
	printf( "Starting MIDIManager... (provisional hack) \n");

	while(mLoopCondition)
	{
		ExecuteEvents();
		mObserved->DoProcessings();
	}

	CLAM::MIDIManager::Current().Stop();	// this is a provisional hack
	printf( "Stopping MIDIManager... (provisional hack) \n");


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
	mPresentation->ChangeConnectionPresentationNames( oldName, newName );
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

void NetworkController::ChangeState( bool start)
{
	if (start) // start the network
	{
		if(mThread.IsRunning())
			return;

		mObserved->ReconfigureAllProcessings();
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
	mPresentation->CreatePortConnectionPresentation(out, in);	
}

void NetworkController::ExecuteCreateControlConnection( const std::string & out , const std::string & in )
{	
	if(mObserved->ConnectControls(out, in))
		RegisterControlConnection( out, in );
}

void NetworkController::RegisterControlConnection( const std::string & out , const std::string & in )
{	
	mControlConnections.push_back( Connection( out, in ));
	mPresentation->CreateControlConnectionPresentation( out, in );
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

std::string BaseName(const std::string& xmlfile)
{
	int dotPos = xmlfile.find_last_of('.');
	if (dotPos == 0) return xmlfile;
	std::string result= xmlfile.substr(0, dotPos);
	return result;
}
void NetworkController::LoadNetworkFrom( const std::string & xmlfile)
{
	Clear();
	mPresentation->Clear();
	CLAM::XMLStorage storage;
	storage.Restore( *mObserved, xmlfile );

	BindTo( *mObserved );

	mPresentation->SetUpWidgetsPositions( BaseName(xmlfile) );

	Publish();
}

void NetworkController::SaveNetworkTo( const std::string & xmlfile)
{
	CLAM::XMLStorage::Dump( *mObserved, "network", xmlfile );
	
	mPresentation->SaveWidgetsPositions( BaseName(xmlfile) );
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
			mPresentation->RemoveConnectionPresentation( completeOutName, *namesIn );	
		}
	}
	for(namesIt=proc->BeginInPortNames(); namesIt!=proc->EndInPortNames(); namesIt++)
	{
		std::string completeInName( name + "." + *namesIt );
		CLAM::InPortBase & inPort = mObserved->GetInPortByCompleteName( completeInName );
		if(inPort.GetAttachedOutPort())
		{
			std::string outName("");
			outName += mObserved->GetNetworkId( inPort.GetAttachedOutPort()->GetProcessing() );
			outName += ".";
			outName += inPort.GetAttachedOutPort()->GetName();
			
			RemovePortConnection( outName, completeInName );
			mPresentation->RemoveConnectionPresentation( outName, completeInName );
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
			mPresentation->RemoveConnectionPresentation( completeOutName, *namesIn );	
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
			mPresentation->RemoveConnectionPresentation( (*connectionIt).GetOut(), (*connectionIt).GetIn() ); 
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
	mPresentation->RebuildProcessingPresentationAttachedTo( 
			mObserved->GetNetworkId( proc ), controller );
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

void NetworkController::AddProcessing2Remove( const std::string & name, CLAM::Processing * proc )
{
	std::string id = name;

	if ( mObserved->HasProcessing( id ) )
			{
			std::cerr << "Processing Already exists!" << std::endl;
			std::string prefix = proc->GetClassName();
			id = mObserved->GetUnusedName( prefix );
			}
	mObserved->AddProcessing(id, proc);
	mPresentation->CreateProcessingPresentation( id, CreateProcessingController(id, proc));
}

std::string NetworkController::AddProcessing( const std::string &key )
{
	std::string name = mObserved->AddProcessing( key );
	CLAM::Processing& proc = mObserved->GetProcessing( name );
	mPresentation->CreateProcessingPresentation( name, CreateProcessingController(name, &proc));
	return name;
}

ProcessingController* NetworkController::CreateProcessingController( const std::string & name, CLAM::Processing * proc )
{
	ProcessingControllersMap::iterator i = mProcessingControllers.find( name );
	if(i!=mProcessingControllers.end())
		CLAM_ASSERT(false, "NetworkController::CreateProcessingController() Trying to add a processing controller with a repeated name (key)" );

	ProcessingController* procContr = new ProcessingController;
	procContr->SignalProcessingNameChanged.Connect( SlotProcessingNameChanged );
	procContr->SignalRemoveAllConnections.Connect( SlotRemoveAllConnections );
	procContr->SignalRebuildProcessingPresentationAttachedTo.Connect( SlotRebuildProcessingPresentationAttachedTo );
	procContr->SignalConfigureProcessing.Connect( SlotConfigureProcessing );

	procContr->BindTo(*proc);
	mProcessingControllers.insert( ProcessingControllersMap::value_type( name, procContr));
	return procContr;
}

std::string NetworkController::GetName()
{
	if(!mObserved)
		return "network controller unbinded";
	return mObserved->GetName();
}
bool NetworkController::BindTo( CLAM::Network& obj )
{
	mObserved = &obj;
	
	CLAM::Network::ProcessingsMap::const_iterator it;
	for (it=mObserved->BeginProcessings(); it!=mObserved->EndProcessings(); it++)
		CreateProcessingController( it->first,  it->second );

	ProcessingControllersMap::iterator itp;
	for (itp=BeginProcessingControllers(); itp!=EndProcessingControllers(); itp++)
		mPresentation->CreateProcessingPresentation( itp->first, itp->second);

	for (it=mObserved->BeginProcessings(); it!=mObserved->EndProcessings(); it++)
	{
		CLAM::Processing * producer = it->second;
		CLAM::OutPortRegistry::Iterator itOutPort;
		for (itOutPort=producer->GetOutPorts().Begin(); itOutPort!=producer->GetOutPorts().End(); itOutPort++)
		{	
			if ((*itOutPort)->HasConnections())
			{
				std::string completeOutName( it->first + "." + (*itOutPort)->GetName() );

				CLAM::Network::NamesList connected = mObserved->GetInPortsConnectedTo( completeOutName );
				CLAM::Network::NamesList::iterator namesIn;
				for(namesIn=connected.begin(); namesIn!=connected.end(); namesIn++)
					RegisterPortConnection( completeOutName, *namesIn);
			}
		}

		CLAM::OutControlRegistry::Iterator itOutControl;
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

