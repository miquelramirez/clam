
#include "NetworkController.hxx"
#include "Network.hxx"
#include "Node.hxx"
#include "InPort.hxx"
#include "Processing.hxx"
#include "OutPort.hxx"
#include <iostream>

namespace CLAMVM
{
	
NetworkController::NetworkController()
	: mObserved(0),
	  mLoopCondition(false)
{
	CreateNewConnection.Wrap( this, &NetworkController::OnNewConnectionFromGUI );
	RemoveConnection.Wrap( this, &NetworkController::OnRemoveConnectionFromGUI );
	RemoveProcessing.Wrap( this, &NetworkController::OnRemoveProcessingFromGUI );
	AddNewProcessing.Wrap( this, &NetworkController::NewProcessingFromGUI );
	ChangeState.Wrap( this, &NetworkController::OnNewChangeState );
}

void NetworkController::ExecuteEvents()
{
	if ( mToConnect.size() != 0)
	{
		ConnectionsMap::iterator it;
		for (it=mToConnect.begin(); it!=mToConnect.end(); it++)
			ConnectPorts( it->second, it->first );
		mToConnect.clear();
	}
	if ( mToDisconnect.size() != 0)
	{
		ConnectionsMap::iterator it;
		for (it=mToDisconnect.begin(); it!=mToDisconnect.end(); it++)
			DisconnectPorts( it->second, it->first );
		mToDisconnect.clear();		
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

void NetworkController::OnNewConnectionFromGUI( const std::string & out, const std::string& in)
{	
	if (mLoopCondition)
		mToConnect.insert( ConnectionsMap::value_type( in, out ) );
	else
		ConnectPorts(out, in);
}

void NetworkController::ConnectPorts( const std::string & out , const std::string & in )
{
	std::cout << "add connection" << std::endl;
	if(mObserved->ConnectPorts(out, in))
	{
		ConnectionAdapter* conAdapter = new ConnectionAdapter;
		conAdapter->BindTo( mObserved->GetOutPortByCompleteName(out), 
				    mObserved->GetInPortByCompleteName(in), (const CLAM::Network&)*mObserved );
		mConnectionAdapters.push_back( conAdapter );
		AcquireConnection.Emit( conAdapter );
	}
}

void NetworkController::OnRemoveConnectionFromGUI( const std::string & outPort, 
						   const std::string & inPort )
{
	if (mLoopCondition)
	{
		if (!mToDisconnect.insert( ConnectionsMap::value_type( inPort, outPort ) ).second )
			CLAM_ASSERT(false, "NetworkController::OnRemoveConnectionFromGUI() Trying to remove connection with repeated key" );
	}
	else
	{
		DisconnectPorts(outPort, inPort);
	}

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
			ConnectionAdapter * con = (*itc);
			if (con->ConnectsInPort(inPort))
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
		std::list<CLAM::InPort*> inPortList = node->GetReaders();
		std::list<CLAM::InPort*>::iterator itInPort;
		for (itInPort=inPortList.begin(); itInPort!=inPortList.end(); itInPort++)
		{
			ConnectionAdapter* conAdapter = new ConnectionAdapter;
			const CLAM::InPort* in = *itInPort;
			conAdapter->BindTo( *out, *in, (const CLAM::Network&)*mObserved);
			mConnectionAdapters.push_back( conAdapter );
			AcquireConnection.Emit( conAdapter );
		}


	}	
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

} //namespace CLAMVM
