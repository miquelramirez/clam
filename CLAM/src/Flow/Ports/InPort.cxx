
#include "InPort.hxx"
#include "OutPort.hxx"
#include "Processing.hxx"

namespace CLAM
{

InPortBase::InPortBase( const std::string & name, Processing * proc )
	: mAttachedOutPort(0),
	  mName(name),
	  mProcessing(proc)
{
	if(proc)
		proc->PublishInPort(this);
}

InPortBase::~InPortBase()
{
}

OutPortBase * InPortBase::GetAttachedOutPort() 
{
	return mAttachedOutPort;
}

void InPortBase::SetAttachedOutPort( OutPortBase* out )
{
	mAttachedOutPort = out;
}

const std::string & InPortBase::GetName()
{
	return mName;
}

Processing * InPortBase::GetProcessing()
{
	return mProcessing;
}

void InPortBase::Disconnect()
{	
	CLAM_DEBUG_ASSERT(mAttachedOutPort, "InPortBase::Disconnect() - InPort is not connected" );
	mAttachedOutPort->DisconnectFromIn( *this );
}

} // namespace CLAM

