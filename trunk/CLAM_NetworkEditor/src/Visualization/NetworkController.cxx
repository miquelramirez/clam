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
#include <iostream>

namespace CLAMVM
{
	
NetworkController::NetworkController()
	: mObserved(0),
	  mLoopCondition(false)
{
	SlotCreateNewPortConnection.Wrap( this, &NetworkController::CreateNewPortConnection );
	SlotCreateNewControlConnection.Wrap( this, &NetworkController::CreateNewControlConnection );
	SlotRemovePortConnection.Wrap( this, &NetworkController::RemovePortConnection );
	SlotRemoveControlConnection.Wrap( this, &NetworkController::RemoveControlConnection );
	SlotRemoveProcessing.Wrap( this, &NetworkController::RemoveProcessing );
	SlotAddProcessing.Wrap( this, &NetworkController::AddProcessing );
	SlotChangeState.Wrap( this, &NetworkController::ChangeState );
	SlotSaveNetwork.Wrap( this, &NetworkController::SaveNetwork );
	SlotLoadNetwork.Wrap( this, &NetworkController::LoadNetwork );
	SlotClear.Wrap( this, &NetworkController::Clear );
//	SlotCreateNewPresentation.Wrap( this, &NetworkController::CreateNewPresentation );
//	SlotRemoveProcessingController.Wrap( this, &NetworkController::RemoveProcessingController );
//	SlotRebuildProcessingStructure.Wrap( this, &NetworkController::OnRebuildProcessingStructure );
}

void NetworkController::ExecuteEvents()
{
	if ( mPortsToConnect.size() != 0)
	{
		ConnectionsMap::iterator it;
		for (it=mPortsToConnect.begin(); it!=mPortsToConnect.end(); it++)
			ConnectPorts( it->second, it->first );
		mPortsToConnect.clear();

		for (it=mControlsToConnect.begin(); it!=mControlsToConnect.end(); it++)
			ConnectControls( it->second, it->first );
		mControlsToConnect.clear();
	}
	if ( mPortsToDisconnect.size() != 0)
	{
		ConnectionsMap::iterator it;
		for (it=mPortsToDisconnect.begin(); it!=mPortsToDisconnect.end(); it++)
			DisconnectPorts( it->second, it->first );
		mPortsToDisconnect.clear();		

		for (it=mControlsToDisconnect.begin(); it!=mControlsToDisconnect.end(); it++)
			DisconnectControls( it->second, it->first );
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
/*
void NetworkController::RemoveProcessingController( ProcessingController * proc )
{
	ProcessingControllersMapIterator it;
	for(it=mProcessingControllers.begin();it!=mProcessingControllers.end();it++)
	{
		if(it->second == (ProcessingController*)proc )
		{		
			mProcessingControllers.erase(it);
			return;
		}
	}
}
*/
void NetworkController::CreateNewPortConnection( const std::string & out, const std::string& in)
{	
	if (mLoopCondition)
		mPortsToConnect.insert( ConnectionsMap::value_type( in, out ) );
	else
		ConnectPorts(out, in);
}

void NetworkController::CreateNewControlConnection( const std::string & out, const std::string& in)
{	
	if (mLoopCondition)
		mControlsToConnect.insert( ConnectionsMap::value_type( in, out ) );
	else
		ConnectControls(out, in);
}

void NetworkController::ConnectPorts( const std::string & out , const std::string & in )
{
	std::cout << "add port connection" << std::endl;
	
	if(mObserved->ConnectPorts(out, in))
	{
		ConnectionAdapterTmpl< CLAM::OutPort, CLAM::InPort> * 
			conAdapter = new ConnectionAdapterTmpl< CLAM::OutPort, CLAM::InPort >;
		
		conAdapter->BindTo( mObserved->GetOutPortByCompleteName(out), 
				    mObserved->GetInPortByCompleteName(in), (const CLAM::Network&)*mObserved );
		mConnectionAdapters.push_back( (ConnectionAdapter*)conAdapter );
		SignalAcquirePortConnection.Emit( (ConnectionAdapter*)conAdapter );
	}
}

void NetworkController::ConnectControls( const std::string & out , const std::string & in )
{
	std::cout << "add control connection" << std::endl;
	
	if(mObserved->ConnectControls(out, in))
	{
		ConnectionAdapterTmpl< CLAM::OutControl, CLAM::InControl> * 
			conAdapter = new ConnectionAdapterTmpl< CLAM::OutControl, CLAM::InControl >;
		
		conAdapter->BindTo( mObserved->GetOutControlByCompleteName(out), 
				    mObserved->GetInControlByCompleteName(in), (const CLAM::Network&)*mObserved );
		mConnectionAdapters.push_back( (ConnectionAdapter*)conAdapter );
		SignalAcquireControlConnection.Emit( (ConnectionAdapter*)conAdapter );
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
		DisconnectPorts(outPort, inPort);
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
		DisconnectControls(outControl, inControl);
	}

}

void NetworkController::LoadNetwork( const std::string & file)
{
	Clear();
	CLAM::XMLStorage storage;
	storage.Restore( *mObserved, file );
	Publish();
	
}

void NetworkController::SaveNetwork( const std::string & file)
{
	CLAM::XMLStorage storage;
	storage.UseIndentation(true);
	storage.Dump( *mObserved, "network", file );
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
	// TODO: it must delete all the connection controllers related to this controller?
	ProcessingControllersMapIterator it = mProcessingControllers.find( name );
	if(it==mProcessingControllers.end())
			CLAM_ASSERT(false, "NetworkControllers::ExecuteRemoveProcessing() Trying to remove a processing controller that is not included in the network controller" );

	ProcessingController * proc = it->second;
	mProcessingControllers.erase( name );
	mObserved->RemoveProcessing( name );
	SignalRemoveProcessingPresentation.Emit( name );
	delete proc;
}

void NetworkController::DisconnectPorts( const std::string & out , const std::string & in )
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

void NetworkController::DisconnectControls( const std::string & out , const std::string & in )
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
/*
void NetworkController::OnRebuildProcessingStructure( CLAM::Processing * proc )
{
	std::string name = mObserved->GetNetworkId(proc);
	SignalRemoveProcessingToGUI.Emit( name );
	
	CreateProcessingController( name, proc );
}
*/
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
//	controller->SignalCreateNewPresentation.Connect(SlotCreateNewPresentation);
//	controller->SignalRebuildProcessingStructure.Connect(SlotRebuildProcessingStructure);
//	controller->SignalRemoveProcessingController.Connect(SlotRemoveProcessingController);
	mProcessingControllers.insert( ProcessingControllersMap::value_type( name, controller));
	return controller;
//	SignalCreateProcessingPresentation( name, controller );
//	SignalAcquireProcessing.Emit(controller, name);
}
/*
void NetworkController::CreateNewPresentation( ProcessingController * controller, const std::string & name )
{
	SignalAcquireProcessing.Emit((ProcessingController*)controller, name);
}
*/
std::string NetworkController::GetName()
{
	if(!mObserved)
		return "network controller unbinded";
	return mObserved->GetName();
}
bool NetworkController::Publish()
{
	CLAM_ASSERT(mObserved, "Trying to publish an unbinded network controller" );

//	SignalAcquireName.Emit(mObserved->GetName());
//	CLAM::Network::ProcessingsMap::const_iterator it;
//	for (it=mObserved->BeginProcessings(); it!=mObserved->EndProcessings(); it++)
//	{
//		CLAM::Processing * producer = it->second;
//		CreateProcessingController( it->first,  it->second );
//	}
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
		mProcessingControllers.erase( it->first );
		delete controller;
		SignalRemoveProcessingPresentation.Emit( it->first );
	}

	mProcessingControllers.clear();

	ConnectionAdapterIterator itc;
	for ( itc=mConnectionAdapters.begin(); itc!=mConnectionAdapters.end(); itc++)
		delete *itc;
	mConnectionAdapters.clear();
	if(mObserved)
		mObserved->Clear();
}

} //namespace CLAMVM
