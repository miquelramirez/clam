
#include "NetworkController.hxx"
#include "Network.hxx"
#include "Node.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include <iostream>

namespace CLAMVM
{
	
NetworkController::NetworkController()
	: mObserved(0)
{
	CreateNewConnection.Wrap( this, &NetworkController::OnNewConnectionFromGUI );
	RemoveConnection.Wrap( this, &NetworkController::OnRemoveConnectionFromGUI );
	AddNewProcessing.Wrap( this, &NetworkController::NewProcessingFromGUI );
	ChangeState.Wrap( this, &NetworkController::OnNewChangeState );
}
	

void NetworkController::OnNewChangeState( bool state)
{
	if (state) // start the network
	{
		mObserved->Start();
		std::cout << "starting network" << std::endl;
		for (int i=0; i<400; i++)
		{
			std::cout << "doing" << std::endl;
			mObserved->DoProcessings();
		}

	}
	else // stop the network
	{			
		std::cout << "stopping network" << std::endl;
		mObserved->Stop();
	}
}

void NetworkController::OnNewConnectionFromGUI( const std::string & out, const std::string& in)
{
	if(mObserved->ConnectPorts(out, in))
	{

	// now we must to create a new gui connection
//	OutPort & outPort = mObserved->GetOutPortByCompleteName(out);
//	InPort & inPort = mObserved->GetInPortByCompleteName(in);
		ConnectionAdapter* conAdapter = new ConnectionAdapter;
		conAdapter->BindTo( mObserved->GetOutPortByCompleteName(out), 
				    mObserved->GetInPortByCompleteName(in) );
		mConnectionAdapters.push_back( conAdapter );
		AcquireConnection.Emit( conAdapter );
	}
}

void NetworkController::OnRemoveConnectionFromGUI(const std::string & out , const std::string & in)
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
//				mConnectionAdapters.remove(con);
//				delete con;
				return;   
			}
		}
	}
}

NetworkController::~NetworkController()
{
	ProcessingAdapterIterator it;
	for ( it=mProcessingAdapters.begin(); it!=mProcessingAdapters.end(); it++)
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

	ProcessingAdapter* adapter = new ProcessingAdapter;

	adapter->BindTo(*proc);
	mProcessingAdapters.push_back(adapter);
	AcquireProcessing.Emit(adapter, name);
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
		// agafar els ports d'entrada
		   // per cada port d'entrada, donat el de sortida, crear adapter lligat a ells
		   // getpresentation per nom de processing.port i fer que emetin senyal quan es moguin
		   // de tal manera que es vagi actualitzant la posicio
		CLAM::NodeBase * node = *itNodes;
		const CLAM::OutPort* out = node->GetWriter();
		std::list<CLAM::InPort*> inPortList = node->GetReaders();
		std::list<CLAM::InPort*>::iterator itInPort;
		for (itInPort=inPortList.begin(); itInPort!=inPortList.end(); itInPort++)
		{
			ConnectionAdapter* conAdapter = new ConnectionAdapter;
			const CLAM::InPort* in = *itInPort;
			conAdapter->BindTo( *out, *in );
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
