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

#include <iostream> // TODO: remove

namespace CLAMVM
{
	
NetworkController::NetworkController()
	: mObserved(0),
	  mLoopCondition(false)
{
	SlotCreatePortConnection.Wrap( this, &NetworkController::CreatePortConnection );
	SlotRemovePortConnection.Wrap( this, &NetworkController::RemovePortConnection );
	
	SlotCreateControlConnection.Wrap( this, &NetworkController::CreateControlConnection );
	SlotRemoveControlConnection.Wrap( this, &NetworkController::RemoveControlConnection );
	
	SlotRemoveProcessing.Wrap( this, &NetworkController::RemoveProcessing );
	SlotAddProcessing.Wrap( this, &NetworkController::AddProcessing );
	
	SlotChangeState.Wrap( this, &NetworkController::ChangeState );
	SlotSaveNetwork.Wrap( this, &NetworkController::SaveNetwork );
	SlotLoadNetwork.Wrap( this, &NetworkController::LoadNetwork );
	SlotClear.Wrap( this, &NetworkController::Clear );
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

void NetworkController::ChangeState( bool state)
{
	if (state) // start the network
	{

		mObserved->Start();
		mLoopCondition = true;
		mThread.SetThreadCode( makeMemberFunctor0( *this, NetworkController, ProcessingLoop ) );
		
		mThread.Start();

	}
	else // stop the network
	{			
		mLoopCondition = false;
		mThread.Stop();
		mObserved->Stop();
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
		ConnectionAdapterTmpl< CLAM::OutPort, CLAM::InPort> * 
			conAdapter = new ConnectionAdapterTmpl< CLAM::OutPort, CLAM::InPort >;
		
		conAdapter->BindTo( mObserved->GetOutPortByCompleteName(out), 
				    mObserved->GetInPortByCompleteName(in), (const CLAM::Network&)*mObserved );
		mConnectionAdapters.push_back( (ConnectionAdapter*)conAdapter );
		SignalCreatePortConnectionPresentation.Emit( (ConnectionAdapter*)conAdapter );
	}
}

void NetworkController::ExecuteCreateControlConnection( const std::string & out , const std::string & in )
{
	if(mObserved->ConnectControls(out, in))
	{
		ConnectionAdapterTmpl< CLAM::OutControl, CLAM::InControl> * 
			conAdapter = new ConnectionAdapterTmpl< CLAM::OutControl, CLAM::InControl >;
		
		conAdapter->BindTo( mObserved->GetOutControlByCompleteName(out), 
				    mObserved->GetInControlByCompleteName(in), (const CLAM::Network&)*mObserved );
		mConnectionAdapters.push_back( (ConnectionAdapter*)conAdapter );
		SignalCreateControlConnectionPresentation.Emit( (ConnectionAdapter*)conAdapter );
	}
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

	ProcessingControllersMapIterator it;
	for (it=BeginProcessingControllers(); it!=EndProcessingControllers(); it++)
	{
		SignalCreateProcessingPresentation.Emit( it->first, it->second);
	}
	// connections!
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

void NetworkController::ExecuteRemoveProcessing( const std::string & name )
{

	ProcessingControllersMapIterator it = mProcessingControllers.find( name );
	if(it==mProcessingControllers.end())
			CLAM_ASSERT(false, "NetworkControllers::ExecuteRemoveProcessing() Trying to remove a processing controller that is not included in the network controller" );

	ProcessingController * proc = it->second;
	ProcessingController::NamesList::iterator namesIt;	
	for(namesIt=proc->BeginOutPortNames(); namesIt!=proc->EndOutPortNames(); namesIt++)
	{
		std::string completeOutName("");
		completeOutName += name;
		completeOutName += ".";
		completeOutName += *namesIt;
			
		std::cout << "removing connections to port: " << completeOutName << std::endl;
		CLAM::Network::NamesList connected = mObserved->GetInPortsConnectedTo( completeOutName );
		CLAM::Network::NamesList::iterator namesIn;
		for(namesIn=connected.begin(); namesIn!=connected.end(); namesIn++)
		{
			std::cout << "removing port connection: " << completeOutName << " : " << *namesIn << std::endl;
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
	
	for(namesIt=proc->BeginOutControlNames(); namesIt!=proc->EndOutControlNames(); namesIt++)
	{	
		std::string completeOutName("");
		completeOutName += name;
		completeOutName += ".";
		completeOutName += *namesIt;
		
		std::cout << "removing connections to control: " << completeOutName << std::endl;
		CLAM::Network::NamesList connected = mObserved->GetInControlsConnectedTo( completeOutName );
		CLAM::Network::NamesList::iterator namesIn;
		for(namesIn=connected.begin(); namesIn!=connected.end(); namesIn++)
		{
			std::cout << "removing control connection: " << completeOutName << " : " << *namesIn << std::endl;
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

		std::cout << "removing connections to control: " << completeInName << std::endl;

		// in controls should have references to its connected out controls
		ConnectionAdapterIterator connectionIt;
		for( connectionIt=mConnectionAdapters.begin(); connectionIt!=mConnectionAdapters.end(); connectionIt++)
		{
			if( (*connectionIt)->GetInName() == completeInName )
			{	
				std::cout << "removing control connection: " << completeInName << " : " << (*connectionIt)->GetOutName() << std::endl;
				RemoveControlConnection( (*connectionIt)->GetOutName(), completeInName );
				std::cout << "removing" << std::endl;
				SignalRemoveConnectionPresentation.Emit( (*connectionIt)->GetOutName(), completeInName );
				std::cout << "ok" << std::endl;
			}
		}
	}

	mProcessingControllers.erase( name );
	mObserved->RemoveProcessing( name );
	delete proc;
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
			if (con->ConnectsInElement(inControl))
			{
				mConnectionAdapters.remove(con);
				delete con;
				return;   
			}
		}
	}
}



NetworkController::~NetworkController()
{
	mLoopCondition = false;
	mThread.Stop();
	if(mObserved)
		mObserved->Stop();

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
/*
	CLAM::Network::ProcessingsMap::const_iterator it;
	for (it=mObserved->BeginProcessings(); it!=mObserved->EndProcessings(); it++)
	{
		CLAM::Processing * producer = it->second;
		CLAM::PublishedOutPorts::Iterator itOutPort;
	
		for (itOutPort=producer->GetOutPorts().Begin(); 
		     itOutPort!=producer->GetOutPorts().End(); 
		     itOutPort++)

		{	
			if (!(*itOutPort)->GetNode())
				break;

			CLAM::Network::InPortsList consumers;
			consumers = mObserved->GetInPortsConnectedTo( **itOutPort );
			CLAM::Network::InPortsList::iterator itInPort;
			
			for ( itInPort=consumers.begin(); itInPort!=consumers.end(); itInPort++ )
			{
				ConnectionAdapterTmpl<CLAM::OutPort, CLAM::InPort>* conAdapter = new ConnectionAdapterTmpl<CLAM::OutPort, CLAM::InPort>;
				conAdapter->BindTo(  **itOutPort, **itInPort, (const CLAM::Network&)*mObserved);
				mConnectionAdapters.push_back( (ConnectionAdapter*)conAdapter );
				SignalAcquirePortConnection.Emit( (ConnectionAdapter*)conAdapter );
			}		
		}
		CLAM::PublishedOutControls::Iterator itOutControl;

		for( itOutControl=producer->GetOutControls().Begin();
		     itOutControl!=producer->GetOutControls().End();
		     itOutControl++)
		{
			CLAM::OutControl * sender = *itOutControl;
			std::list<CLAM::InControl*>::iterator itInControl;
			for( itInControl=sender->BeginInControlsConnected();
			     itInControl!=sender->EndInControlsConnected();
			     itInControl++)
			{
				ConnectionAdapterTmpl<CLAM::OutControl, CLAM::InControl>* conAdapter = 
					new ConnectionAdapterTmpl<CLAM::OutControl, CLAM::InControl>;
				conAdapter->BindTo( *sender, **itInControl, (const CLAM::Network&)*mObserved);
				mConnectionAdapters.push_back((ConnectionAdapter*)conAdapter);
				SignalAcquireControlConnection.Emit( (ConnectionAdapter*)conAdapter);
			}
		}

		
	}
	*/
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

	mLoopCondition = false;
	mThread.Stop();

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
