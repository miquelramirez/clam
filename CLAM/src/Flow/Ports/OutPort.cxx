
#include "OutPort.hxx"
#include "Processing.hxx"

namespace CLAM
{

OutPortBase::OutPortBase( const std::string & name, Processing * proc )
	: mName(name),
	  mProcessing(proc)
{
	if(proc)
		proc->PublishOutPort(this);
}

OutPortBase::~OutPortBase()
{
}

const std::string & OutPortBase::GetName()
{
	return mName;
}

Processing * OutPortBase::GetProcessing()
{
	return mProcessing;
}

OutPortBase::InPortsList::iterator OutPortBase::BeginConnectedInPorts()
{
	return mConnectedInPortsList.begin();
}

OutPortBase::InPortsList::iterator OutPortBase::EndConnectedInPorts()
{
	return mConnectedInPortsList.end();
}

} // namespace CLAM

