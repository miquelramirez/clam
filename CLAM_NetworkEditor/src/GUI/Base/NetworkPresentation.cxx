
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
	SetConnection.Wrap( this, &NetworkPresentation::OnNewConnection );
	SetRemoveConnection.Wrap( this, &NetworkPresentation::OnRemoveConnection );
	SetRemoveProcessing.Wrap( this, &NetworkPresentation::OnRemoveProcessing );
	AddNewProcessing.Wrap( this, &NetworkPresentation::OnAddNewProcessing );
	ChangeState.Wrap( this, &NetworkPresentation::OnNewChangeState );
}

void NetworkPresentation::OnNewChangeState( bool newState )
{
	if (newState != mNetworkState)
	{
		mNetworkState = newState;
		SChangeState.Emit( mNetworkState );
	}
}

void NetworkPresentation::OnRemoveConnection(  ConnectionPresentation * con)
{
	mConnectionPresentations.remove(con);
	con->Hide();

	RemoveConnectionFromGUI.Emit( con->GetOutName(), con->GetInName() );
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
		OnRemoveConnection( *it );
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
	model.AcquireConnection.Connect( SetConnection );
	SChangeState.Connect( model.ChangeState );
	AddProcessing.Connect( model.AddNewProcessing );
	
	CreateNewConnectionFromGUI.Connect( model.CreateNewConnection );
	RemoveConnectionFromGUI.Connect( model.RemoveConnection );
	RemoveProcessingFromGUI.Connect( model.RemoveProcessing );
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

void NetworkPresentation::OnAddNewProcessing( const std::string & name, 
						 CLAM::Processing * proc)
{
	AddProcessing.Emit(name,proc);
}



} // namespace NetworkGUI
