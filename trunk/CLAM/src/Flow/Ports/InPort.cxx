#include "InPort.hxx"
#include "OutPort.hxx"

namespace CLAM
{

InPort::InPort(const std::string &n,
			   Processing *o,
			   int length,
			   int hop,
			   bool inplace) : 
	Port(n,o,length,hop),
	mCanDoInplace(inplace)
{}

bool InPort::IsConnectedTo( OutPort& out )
{
	return out.IsConnectedTo( *this );
}

} // namespace
