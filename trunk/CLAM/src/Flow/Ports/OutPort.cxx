#include "OutPort.hxx"
#include "InPort.hxx"

namespace CLAM
{

bool OutPort::IsConnectedTo( InPort &in )
{
	if ( !IsAttached() )
		return false;

	if ( GetProcessingData() )
		return ( GetProcessingData() == in.GetProcessingData() );
	else // must be attached by node
		return ( GetNode() == in.GetNode() );
}


OutPort::OutPort(const std::string &n,
				 Processing *o,
				 int length,
				 int hop ) : 
	Port(n,o,length,hop)
{}

} //namespace
