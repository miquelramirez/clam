
#include "NetworkPresentation.hxx"
#include "NetworkModel.hxx"
#include "ProcessingAdapter.hxx"
#include "ConnectionAdapter.hxx"
#include "ProcessingPresentation.hxx"
#include "ConnectionPresentation.hxx"
#include "OutPortPresentation.hxx"
#include "InPortPresentation.hxx"
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

void NetworkPresentation::OnRemoveConnection( const std::string & out, const std::string & in, ConnectionPresentation * con)
{
	mConnectionPresentations.remove(con);
	ProcessingPresentation & procOut = GetProcessingPresentation( GetProcessingIdentifier( out ));
	ProcessingPresentation & procIn = GetProcessingPresentation( GetProcessingIdentifier( in ));
	RemoveConnectionFromGUI.Emit(procOut.GetNameFromNetwork()+"."+GetLastIdentifier(out), 
				     procIn.GetNameFromNetwork()+"."+GetLastIdentifier(in));
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
	model.AcquireName.Connect(SetName);
	model.AcquireProcessing.Connect(SetProcessing);
	model.AcquireConnection.Connect(SetConnection);
	SChangeState.Connect(model.ChangeState);
	AddProcessing.Connect(model.AddNewProcessing);
	
	CreateNewConnectionFromGUI.Connect(model.CreateNewConnection);
	RemoveConnectionFromGUI.Connect(model.RemoveConnection);
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

ProcessingPresentation& NetworkPresentation::GetProcessingPresentation( const std::string & name )
{
	ProcessingPresentationIterator it;
	for(it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
	{
		if ((*it)->GetName() ==  name)
		{
			return **it;
		}
	}
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
