#include "OutPort.hxx"
#include "InPort.hxx"

namespace CLAM
{

bool OutPort::IsConnectedTo( InPort &in )
{
	if ( !IsAttached() )
		return false;

	if ( GetNode () )
		return ( GetNode() == in.GetNode() );
	else
		return ( GetProcessingData() == in.GetProcessingData() );

}


OutPort::OutPort(const std::string &n,
				 Processing *o,
				 int length,
				 int hop ) : 
	Port(n,o,length,hop)
{}

} //namespace
