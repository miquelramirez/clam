
#include "NetworkController.hxx"
#include "Network.hxx"
#include "Node.hxx"
#include "InPort.hxx"
#include "Processing.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include "Storage.hxx"
#include <iostream>

namespace CLAMVM
{
	
NetworkController::NetworkController()
	: mObserved(0),
	  mLoopCondition(false)
{
	CreateNewPortConnection.Wrap( this, &NetworkController::OnNewPortConnectionFromGUI );
	CreateNewControlConnection.Wrap( this, &NetworkController::OnNewControlConnectionFromGUI );
	RemovePortConnection.Wrap( this, &NetworkController::OnRemovePortConnectionFromGUI );
	RemoveControlConnection.Wrap( this, &NetworkController::OnRemoveControlConnectionFromGUI );
	RemoveProcessing.Wrap( this, &NetworkController::OnRemoveProcessingFromGUI );
	AddNewProcessing.Wrap( this, &NetworkController::NewProcessingFromGUI );
	ChangeState.Wrap( this, &NetworkController::OnNewChangeState );
	SaveNetwork.Wrap( this, &NetworkController::OnSaveNetwork );
	LoadNetwork.Wrap( this, &NetworkController::OnLoadNetwork );
	Clear.Wrap( this, &NetworkController::OnClear );
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
			RemoveProcessingFromNetwork( *it );
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

void NetworkController::OnNewChangeState( bool state)
{
	if (state) // start the network
	{

		mObserved->Start();
		mLoopCondition = true;
		std::cout << "starting network" << std::endl;
		mThread.SetThreadCode( makeMemberFunctor0( *this, NetworkController, ProcessingLoop ) );
		
		mThread.Start();

	}
	else // stop the network
	{			
		std::cout << "stopping network" << std::endl;
		mLoopCondition = false;
		mThread.Stop();
		mObserved->Stop();
	}
}

void NetworkController::OnNewPortConnectionFromGUI( const std::string & out, const std::string& in)
{	
	if (mLoopCondition)
		mPortsToConnect.insert( ConnectionsMap::value_type( in, out ) );
	else
		ConnectPorts(out, in);
}

void NetworkController::OnNewControlConnectionFromGUI( const std::string & out, const std::string& in)
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
		AcquirePortConnection.Emit( (ConnectionAdapter*)conAdapter );
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
		AcquireControlConnection.Emit( (ConnectionAdapter*)conAdapter );
	}
}

void NetworkController::OnRemovePortConnectionFromGUI( const std::string & outPort, 
						       const std::string & inPort )
{
	if (mLoopCondition)
	{
		if (!mPortsToDisconnect.insert( ConnectionsMap::value_type( inPort, outPort ) ).second )
			CLAM_ASSERT(false, "NetworkController::OnRemovePortConnectionFromGUI() Trying to remove connection with repeated key" );
	}
	else
	{
		DisconnectPorts(outPort, inPort);
	}

}


void NetworkController::OnRemoveControlConnectionFromGUI( const std::string & outControl, 
						       const std::string & inControl )
{
	if (mLoopCondition)
	{
		if (!mControlsToDisconnect.insert( ConnectionsMap::value_type( inControl, outControl ) ).second )
			CLAM_ASSERT(false, "NetworkController::OnRemoveControlConnectionFromGUI() Trying to remove connection with repeated key" );
	}
	else
	{
		DisconnectControls(outControl, inControl);
	}

}

void NetworkController::OnLoadNetwork( const std::string & file)
{
	OnClear();
	std::cout << "loading network from: " << file << std::endl;
	CLAM::XMLStorage storage;
	storage.Restore( *mObserved, file );
	Publish();
	
}

void NetworkController::OnSaveNetwork( const std::string & file)
{
	std::cout << "saving network to: " << file << std::endl;
	CLAM::XMLStorage storage;
	storage.UseIndentation(true);
	storage.Dump( *mObserved, "network", file );
}

void NetworkController::OnRemoveProcessingFromGUI(const std::string & proc)
{
	if (mLoopCondition)
		mProcessingsToRemove.push_back( proc );
	else
		RemoveProcessingFromNetwork( proc );
}

void NetworkController::RemoveProcessingFromNetwork( const std::string & proc )
{
	mObserved->RemoveProcessing( proc );
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
//	mObserved->Stop();

	ProcessingControllerIterator it;
	for ( it=mProcessingControllers.begin(); it!=mProcessingControllers.end(); it++)
		delete *it;
	ConnectionAdapterIterator itc;
	for ( itc=mConnectionAdapters.begin(); itc!=mConnectionAdapters.end(); itc++)
		delete *itc;
	
}


void NetworkController::NewProcessingFromGUI( const std::string & name, 
					      CLAM::Processing * proc )
{
	mObserved->AddProcessing(name, proc);
	AddProcessing(name, proc);
}

void NetworkController::AddProcessing( const std::string & name, CLAM::Processing * proc )
{

	ProcessingController* controller = new ProcessingController;

	controller->BindTo(*proc);
	mProcessingControllers.push_back(controller);
	AcquireProcessing.Emit(controller, name);
}
	
bool NetworkController::Publish()
{
	if ( !mObserved )
		return false;

	AcquireName.Emit(mObserved->GetName());
	CLAM::Network::ProcessingsMap::const_iterator it;
	for (it=mObserved->BeginProcessings(); it!=mObserved->EndProcessings(); it++)
	{
		AddProcessing( it->first,  it->second );
	}

	CLAM::Network::Nodes::const_iterator itNodes;
	for(itNodes=mObserved->BeginNodes(); itNodes!=mObserved->EndNodes(); itNodes++)
	{
		CLAM::NodeBase * node = *itNodes;
		const CLAM::OutPort* out = node->GetWriter();
//		std::list<CLAM::InPort*> inPortList = node->GetReaders();
//		std::list<CLAM::InPort*>::iterator itInPort;
		CLAM::NodeBase::ReaderIterator it;
		for (it=node->BeginReaders(); it!=node->EndReaders(); it++)
		{
			ConnectionAdapterTmpl<CLAM::OutPort, CLAM::InPort>* conAdapter = new ConnectionAdapterTmpl<CLAM::OutPort, CLAM::InPort>;
			const CLAM::InPort* in = *it;
			conAdapter->BindTo( *out, *in, (const CLAM::Network&)*mObserved);
			mConnectionAdapters.push_back( (ConnectionAdapter*)conAdapter );
			AcquirePortConnection.Emit( (ConnectionAdapter*)conAdapter );
		}


	}	

	// TODO: Is possible to detect the network of control connections?

	return true;
}

bool NetworkController::BindTo( CLAM::Network& obj )
{
	mObserved = dynamic_cast< CLAM::Network* > (&obj);
	
	if( !mObserved )
		return false;
	return true;
}

bool NetworkController::Update()
{
	return true;
}

void NetworkController::OnClear()
{	
	mLoopCondition = false;
	mThread.Stop();

	ProcessingControllerIterator it;
	for ( it=mProcessingControllers.begin(); it!=mProcessingControllers.end(); it++)
		delete *it;
	mProcessingControllers.clear();

	ConnectionAdapterIterator itc;
	for ( itc=mConnectionAdapters.begin(); itc!=mConnectionAdapters.end(); itc++)
		delete *itc;
	mConnectionAdapters.clear();
	mObserved->Clear();
}

} //namespace CLAMVM
