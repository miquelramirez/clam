
#include "ConnectionAdapter.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "Processing.hxx"

namespace CLAMVM
{

ConnectionAdapter::ConnectionAdapter()
	: mOutObserved(0), mInObserved(0)
{
}

ConnectionAdapter::~ConnectionAdapter()
{
}

bool ConnectionAdapter::Publish()
{
	if ((!mOutObserved)  || (!mInObserved))
		return false;
	
	std::string outName("");
	outName += mOutObserved->GetProcessing()->GetName();
	outName += ".";
	outName += mOutObserved->GetName();       

	std::string inName("");
	inName += mInObserved->GetProcessing()->GetName();
	inName += ".";
	inName += mInObserved->GetName();
	
	AcquireNames.Emit( outName, inName );
}

bool ConnectionAdapter::BindTo( const CLAM::OutPort& out, const CLAM::InPort& in )
{
	mOutObserved = dynamic_cast< const CLAM::OutPort* > (&out);
	mInObserved = dynamic_cast< const CLAM::InPort* > (&in);
	
	if ((!mOutObserved)  || (!mInObserved))
		return false;
	return true;
}

bool ConnectionAdapter::ConnectsInPort( CLAM::InPort & in)
{
	return (&in == mInObserved);
}


} // namespace CLAMVM
